/************ SURFACE ESP32 - ESP NOW RECEIVER ************/

#include <WiFi.h>
#include <esp_now.h>

typedef struct struct_message {
  float depth;
  float pressure;
  bool emergency;
} struct_message;

struct_message incomingData;

// Callback when data is received
void onDataRecv(const esp_now_recv_info *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  Serial.println("📥 Data Received:");
  Serial.print("Depth: ");
  Serial.print(incomingData.depth);
  Serial.println(" m");

  Serial.print("Pressure: ");
  Serial.print(incomingData.pressure);
  Serial.println(" kPa");

  Serial.print("Emergency: ");
  Serial.println(incomingData.emergency ? "YES 🚨" : "NO ✅");
  Serial.println("-----------------------");
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

  // Register receive callback
  esp_now_register_recv_cb(onDataRecv);

  Serial.println("✅ Surface ESP Ready - Waiting for data...");
}

void loop() {
  // Nothing here, everything handled in callback
}
