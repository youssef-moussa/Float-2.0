// communication.h - ESP-NOW communication for Float Device
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <esp_now.h>
#include <WiFi.h>
#include "config.h"

// Communication status
extern bool commInitialized;
extern bool startCommandReceived;

// Function declarations
bool initCommunication();
void sendDataToStation(String data);
bool isStartCommandReceived();
void clearStartCommand();

#endif // COMMUNICATION_H