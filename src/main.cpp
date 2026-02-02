#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Shared message structure
typedef struct {
  char text[32];
  uint32_t counter;
  int32_t value;
} MessagePacket;

// ============================================
// SENDER
// ============================================
#ifdef SENDER

// Receiver MAC: C0:CD:D6:84:3D:0C
uint8_t receiverMAC[] = {0xC0, 0xCD, 0xD6, 0x84, 0x3D, 0x0C};

MessagePacket outgoing;
esp_now_peer_info_t peerInfo;
uint32_t sendCount = 0;

void OnDataSent(const uint8_t* mac, esp_now_send_status_t status) {
  Serial.print(status == ESP_NOW_SEND_SUCCESS ? "  [OK] sent" : "  [FAIL] send");
  Serial.println(); 
}

void setup() {
  Serial.begin(115200);
  delay(1500);
  Serial.println("\n=== ESP-NOW SENDER (test) ===\n");

  WiFi.mode(WIFI_STA);
  Serial.print("My MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Add peer failed");
  } else {
    Serial.println("Peer added (broadcast or single MAC)\n");
  }

  Serial.println("Sending messages every 2 sec...\n");
}

void loop() {
  // Use sprintf_P for better compatibility with Arduino/ESP C++
  sprintf(outgoing.text, "Hello #%lu", (unsigned long)sendCount);
  outgoing.counter = sendCount;
  outgoing.value = (int32_t)millis();

  // Use int as return type for compatibility; esp_err_t may not be defined
  int result = esp_now_send(receiverMAC, (uint8_t*)&outgoing, sizeof(outgoing));

  Serial.printf("[%lu] Sent: \"%s\" counter=%lu value=%ld (result=%s)\n",
                millis() / 1000, outgoing.text, (unsigned long)outgoing.counter,
                (long)outgoing.value, result == ESP_OK ? "OK" : "ERR");

  sendCount++;
  delay(2000);
}

#endif

// ============================================
// RECEIVER
// ============================================
#ifdef RECEIVER

MessagePacket incoming;
uint32_t recvCount = 0;

void OnDataRecv(const uint8_t* mac, const uint8_t* data, int len) {
  if (len >= (int)sizeof(MessagePacket)) {
    memcpy(&incoming, data, sizeof(MessagePacket));
    recvCount++;
    Serial.println("\n--- RECEIVED ---");
    Serial.printf("  From: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.printf("  Text:   %s\n", incoming.text);
    Serial.printf("  Counter: %lu\n", (unsigned long)incoming.counter);
    Serial.printf("  Value:   %ld\n", (long)incoming.value);
    Serial.printf("  Total received: %lu\n---\n\n", (unsigned long)recvCount);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1500);
  Serial.println("\n=== ESP-NOW RECEIVER (test) ===\n");

  WiFi.mode(WIFI_STA);
  Serial.print("My MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.println("(Use this MAC in SENDER's receiverMAC[] to target this device)\n");

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Waiting for messages...\n");
}

void loop() {
  delay(100);
}

#endif
