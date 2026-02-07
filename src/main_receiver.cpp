#include <WiFi.h>
#include <esp_now.h>

typedef struct {
  float depth;
  float pressure;
  bool emergency;
} DataPacket;

DataPacket incoming;

void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
  // Make sure data length matches
Serial.println(String("Length is ") + len);
Serial.println(String("Incoming is ") + sizeof(incoming));

  // if(len != sizeof(incoming)) {
  //   Serial.println("Received wrong data size");
  //   return;
  // }

  // Copy bytes into struct
  memcpy(&incoming, data, sizeof(incoming));

  // Print values
  Serial.println("Data Received:");
  Serial.print("Depth: ");
  Serial.print(incoming.depth);
  Serial.println(" m");

  Serial.print("Pressure: ");
  Serial.print(incoming.pressure);
  Serial.println(" kPa");

  Serial.print("Emergency: ");
  Serial.println(incoming.emergency ? "YES " : "NO ");
  Serial.println("------------------------");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);        // Must be STA mode
  esp_now_init();             // Initialize ESP-NOW
  esp_now_register_recv_cb(onReceive); // Register callback
}

void loop() {
  // Nothing needed here, all handled by callback
}
