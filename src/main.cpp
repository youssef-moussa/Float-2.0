/************ UNDERWATER ESP32 - ESP NOW SENDER ************/

#include <WiFi.h>
#include <esp_now.h>

/* -------- CHANGE THIS MAC ADDRESS -------- */
uint8_t surfaceMAC[] = {0xCO, 0xCD, 0xD6, 0x84, 0x3D, 0x0C};
/* ---------------------------------------- */

typedef struct struct_message {
  float depth;
  float pressure;
  bool emergency;
} struct_message;

struct_message data;

// Callback after sending
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "SUCCESS ✅" : "FAILED ❌");
}

void setup() {
  Serial.begin(115200);

  // Set ESP32 as Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW init failed");
    return;
  }

  // Register send callback
  esp_now_register_send_cb(onDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, surfaceMAC, 6);
  peerInfo.channel = 0;   // same channel
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Failed to add peer");
    return;
  }

  Serial.println("✅ Underwater ESP Ready");
}

void loop() {
  // Example data (replace later with sensor readings)
  data.depth = 1.75;        // meters
  data.pressure = 118.6;    // kPa
  data.emergency = false;

  esp_err_t result = esp_now_send(surfaceMAC, (uint8_t *)&data, sizeof(data));

  if (result == ESP_OK) {
    Serial.println("📤 Data Sent");
  } else {
    Serial.println("❌ Error Sending Data");
  }

  delay(1000);
}
