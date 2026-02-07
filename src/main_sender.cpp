#include <WiFi.h>
#include <esp_now.h>

uint8_t receiverMAC[] = {0xC0, 0xCD, 0xD6, 0x84, 0x3D, 0x0C};

typedef struct {
  float depth;
  bool emergency;
} DataPacket;

DataPacket data;

void onSent(const uint8_t *mac, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Sent" : "Failed");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  esp_now_init();

  esp_now_register_send_cb(onSent);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, receiverMAC, 6);
  peer.channel = 0;
  peer.encrypt = false;

  esp_now_add_peer(&peer);
}

void loop() {
  data.depth = 2.3;
  data.emergency = false;

  esp_now_send(receiverMAC, (uint8_t *)&data, sizeof(data));
  delay(1000);
}
