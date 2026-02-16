// Float_Device.ino - Float Device with Communication Testing
#include "config.h"
#include "communication.h"
#include <Arduino.h>

// Test variables
unsigned long lastSendTime = 0;
bool missionStarted = false;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("================================");
    Serial.println("=== Float Device Started ===");
    Serial.println("================================");
    
    // Display configuration
    Serial.println("\n--- Configuration ---");
    Serial.print("Float Height: ");
    Serial.print(FLOAT_HEIGHT);
    Serial.println(" m");
    Serial.print("Target Depth Mission 1: ");
    Serial.print(TARGET_DEPTH_MISSION_1);
    Serial.println(" m");
    Serial.print("Target Depth Mission 2: ");
    Serial.print(TARGET_DEPTH_MISSION_2);
    Serial.println(" m");
    Serial.print("Total Profiles: ");
    Serial.println(TOTAL_PROFILES);
    
    // Initialize communication
    Serial.println("\n--- Initializing Communication ---");
    if (initCommunication()) {
        Serial.println("Communication initialized successfully!");
    } else {
        Serial.println("Communication initialization failed!");
        Serial.println("Check wiring and restart device.");
    }
    
    Serial.println("\n--- Waiting for START command ---");
    Serial.println("System ready. Waiting at surface...\n");

    // Start Serial communication (for debugging or surface control)
    /****************************************************
 * setup()
 * --------------------------------------------------
 * Runs once at system startup.
 * Initializes Serial and pump control pins.
 ****************************************************/
    Serial.begin(115200);

    // Set H-Bridge pins as outputs
    pinMode(PUMP_IN_PIN , OUTPUT);
    pinMode(PUMP_OUT_PIN, OUTPUT);

    // Ensure pump is stopped at startup (safety)
    digitalWrite(PUMP_IN_PIN, LOW);
    digitalWrite(PUMP_OUT_PIN, LOW);
}

void loop() {
    // Check for START command from station
    if (isStartCommandReceived() && !missionStarted) {
        Serial.println("\n================================");
        Serial.println(">>> START COMMAND RECEIVED! <<<");
        Serial.println("================================");
        Serial.println("Mission beginning...\n");
        
        missionStarted = true;
        clearStartCommand();
        lastSendTime = millis();  // Reset timer
    }
    
    // If mission started, send test data every 5 seconds
    if (missionStarted) {
        if (millis() - lastSendTime >= 5000) {
            // Simulate depth data (time in seconds, fake depth reading)
            float fakeDepth = random(0, 250) / 100.0;  // Random depth 0-2.5m
            unsigned long timeSeconds = millis() / 1000;
            
            String dataPacket = String(timeSeconds) + "," + String(fakeDepth, 2);
            
            Serial.print("Sending data: ");
            Serial.println(dataPacket);
            
            sendDataToStation(dataPacket);
            
            lastSendTime = millis();
        }
    }
    
    delay(100);

    // Update pumpCommand if new Serial data received
    handleCommunication();

    // Apply the latest command to the pump
    controlPump(pumpCommand);
}

/****************************************************
 * controlPump(state)
 * --------------------------------------------------
 * Controls motor direction based on given state.
 *
 * Important Safety Rule:
 * IN1 and IN2 must NEVER be HIGH at the same time.
 *
 * Includes direction-change protection:
 * If direction changes, motor is stopped briefly
 * before reversing to protect H-Bridge.
 ****************************************************/
void controlPump(uint8_t state)
{
    // Keeps track of previous state
    static uint8_t lastState = PUMP_OFF;

    // If direction changed → stop briefly first
    if (state != lastState)
    {
        digitalWrite(PUMP_IN_PIN, LOW);
        digitalWrite(PUMP_OUT_PIN, LOW);

        delay(200);  // 200ms safety delay

        lastState = state;
    }

    switch(state)
    {
        case PUMP_FILL:
            // Forward direction
            digitalWrite(PUMP_IN_PIN, HIGH);
            digitalWrite(PUMP_OUT_PIN, LOW);
            break;

        case PUMP_EMPTY:
            // Reverse direction
            digitalWrite(PUMP_IN_PIN, LOW);
            digitalWrite(PUMP_OUT_PIN, HIGH);
            break;

        case PUMP_OFF:
        default:
            // Stop motor
            digitalWrite(PUMP_IN_PIN, LOW);
            digitalWrite(PUMP_OUT_PIN, LOW);
            break;
    }
}
