// communication.cpp - ESP-NOW communication implementation for Float Device
#include "communication.h"
#include "config.h"

// Global variables
uint8_t stationMAC[6] = {0xC0, 0xCD, 0xD6, 0x84, 0x3D, 0x0C}; //Mac Address of Station
bool startCommandReceived = false;
bool commInitialized = false;

// Peer info for station
esp_now_peer_info_t stationPeer;

// Callback when data is received from Station
void onDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    char receivedData[data_len + 1];
    memcpy(receivedData, data, data_len);
    receivedData[data_len] = '\0';
    
    String message = String(receivedData);
    
    Serial.print("Received from Station: ");
    Serial.println(message);
    
    // Check if it's START command
    if (message == "START") {
        startCommandReceived = true;
        Serial.println("START command received! Beginning mission...");
    }
}

// Callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("Data Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Initialize ESP-NOW communication
bool initCommunication() {
    // Set device as WiFi Station
    WiFi.mode(WIFI_STA);
    
    //Debugging
    // Serial.print("Float MAC Address: ");
    // Serial.println(WiFi.macAddress());
    
    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return false;
    }
    
    Serial.println("ESP-NOW initialized successfully");
    
    // Register callbacks
    esp_now_register_recv_cb(onDataReceive);
    esp_now_register_send_cb(onDataSent);
    
    // Register station as peer
    memcpy(stationPeer.peer_addr, stationMAC, 6);
    stationPeer.channel = CHANNEL;
    stationPeer.encrypt = false;
    
    if (esp_now_add_peer(&stationPeer) != ESP_OK) {
        Serial.println("Failed to add station as peer");
        return false;
    }
    
    Serial.println("Station added as peer successfully");
    
    commInitialized = true;
    return true;
}

// Send data string to station
void sendDataToStation(String data) {
    if (!commInitialized) {
        Serial.println("Communication not initialized!");
        return;
    }
    
    const char* dataStr = data.c_str();
    esp_err_t result = esp_now_send(stationMAC, (uint8_t*)dataStr, strlen(dataStr));
    
    if (result == ESP_OK) {
        Serial.println("Data sent to station");
    } else {
        Serial.println("Error sending data to station");
    }
}

// Check if START command was received
bool isStartCommandReceived() {
    return startCommandReceived;
}

// Clear START command flag (after processing)
void clearStartCommand() {
    startCommandReceived = false;
}
/****************************************************
 * Global pump command variable
 * Default state is STOP for safety.
 ****************************************************/
uint8_t pumpCommand = PUMP_OFF;


/****************************************************
 * handleCommunication()
 * --------------------------------------------------
 * Checks if new data is available from Serial.
 * If a valid command is received:
 *
 * 'F' → Fill (absorb water)
 * 'E' → Empty (release water)
 * 'S' → Stop
 *
 * This function only updates the command.
 * It does NOT directly control the pump.
 ****************************************************/
void handleCommunication()
{
    // Check if data is available from Serial
    if (Serial.available())
    {
        // Read one character from Serial buffer
        char cmd = Serial.read();

        // Decide pump state based on received command
        switch(cmd)
        {
            case 'F':
                pumpCommand = PUMP_FILL;
                break;

            case 'E':
                pumpCommand = PUMP_EMPTY;
                break;

            case 'S':
                pumpCommand = PUMP_OFF;
                break;

            default:
                // Ignore unknown commands
                break;
        }
    }
}