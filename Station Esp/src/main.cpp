// Station_Device.ino - Station for Float Device Communication
#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>


uint8_t floatMAC[] = {0xCC, 0x7B, 0x5C, 0xA7, 0x7F, 0xCC};

// Communication channel
#define CHANNEL 1

// Peer info
esp_now_peer_info_t floatPeer;

// Callback when data is received from Float
void onDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    char receivedData[data_len + 1];
    memcpy(receivedData, data, data_len);
    receivedData[data_len] = '\0';  // Null terminate
    
    String message = String(receivedData);
    
    // Forward data to laptop via Serial
    Serial.println("DATA_FROM_FLOAT: " + message);
}

// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("=== Station Device Started ===");
    
    // Set device as WiFi Station
    WiFi.mode(WIFI_STA);
    
    //Debugging
    // Serial.print("Station MAC Address: ");
    // Serial.println(WiFi.macAddress());
    
    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    
    Serial.println("ESP-NOW initialized");
    
    // Register callbacks
    esp_now_register_recv_cb(onDataReceive);
    esp_now_register_send_cb(onDataSent);
    
    // Register float as peer
    memcpy(floatPeer.peer_addr, floatMAC, 6);
    floatPeer.channel = CHANNEL;
    floatPeer.encrypt = false;
    
    if (esp_now_add_peer(&floatPeer) != ESP_OK) {
        Serial.println("Failed to add float as peer");
        return;
    }
    
    Serial.println("Float added as peer");
    Serial.println("Ready! Type 'START' to begin mission");
}

void loop() {
    // Check if data is available from Serial (laptop)
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();  // Remove whitespace
        
        if (command == "START") {
            Serial.println("Sending START command to float...");
            
            // Send START command to float
            const char* startCmd = "START";
            esp_err_t result = esp_now_send(floatMAC, (uint8_t*)startCmd, strlen(startCmd));
            
            if (result == ESP_OK) {
                Serial.println("START command sent successfully");
            } else {
                Serial.println("Error sending START command");
            }
        } else {
            Serial.println("Unknown command. Type 'START' to begin mission.");
        }
    }
    
    delay(10);
}