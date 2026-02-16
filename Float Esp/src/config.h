// config.h - Configuration and pin definitions for Float Device
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ===========================
// HARDWARE PIN DEFINITIONS
// ===========================

// BMP280 Sensor (I2C)
#define BMP280_SDA_PIN 21      // I2C Data pin
#define BMP280_SCL_PIN 22      // I2C Clock pin
#define BMP280_I2C_ADDRESS 0x76 // Default I2C address (or 0x77)

// Flow Rate Sensor
#define FLOW_SENSOR_PIN 34     // Analog input or digital pulse pin (adjust based on your sensor type)

// Pump Control
#define PUMP_INTAKE_PIN 25     // Pin to control intake pump (fill bag)
#define PUMP_OUTTAKE_PIN 26    // Pin to control outtake pump (empty bag)

// Status LED (optional, for debugging)
#define STATUS_LED_PIN 2       // Built-in LED on most ESP32 boards

// ===========================
// SENSOR CALIBRATION VALUES
// ===========================

// BMP280 Depth Sensor
#define SURFACE_PRESSURE_HPA 1013.25  // Standard atmospheric pressure (hPa) - calibrate at surface
#define WATER_DENSITY 1000.0          // Freshwater density (kg/m³) - use 1025 for saltwater
#define GRAVITY 9.81                  // Gravitational acceleration (m/s²)

// Flow Sensor Calibration
#define FLOW_SENSOR_CALIBRATION 7.5   // Pulses per liter (adjust based on your sensor datasheet)
#define MAX_BAG_VOLUME_LITERS 10.0    // Maximum water capacity of your bag

// ===========================
// FLOAT DEVICE DIMENSIONS
// ===========================

#define FLOAT_HEIGHT 0.5              // Height of float device in meters (50 cm)

// ===========================
// MISSION PARAMETERS
// ===========================

// Number of Profiles to Complete
#define TOTAL_PROFILES 2              // Complete 2 full profiles (stays underwater)

// Mission Requirements (depths measured from specified reference points)
#define TARGET_BOTTOM_DEPTH 2.5       // Mission 1: Bottom of float at 2.5m depth
#define TARGET_TOP_DEPTH 0.4          // Mission 2: Top of float at 0.4m depth

// Calculated Target Depths for Sensor (sensor is at TOP of float)
#define TARGET_DEPTH_MISSION_1 (TARGET_BOTTOM_DEPTH - FLOAT_HEIGHT)  // 2.5m from bottom
#define TARGET_DEPTH_MISSION_2 TARGET_TOP_DEPTH                       // 0.4m from top

// Mission Timing
#define HOLD_TIME_MS 30000      // Time to hold at target depth (30 seconds)
#define MISSION_TIMEOUT_MS 600000 // Maximum mission duration (10 minutes for 2 profiles)

// Depth Control Tolerances
#define DEPTH_TOLERANCE 0.15    // Acceptable depth error (±0.15 meters)
#define MAX_DEPTH_LIMIT 3.0     // Emergency max depth before forced ascent (sensor reading)

// ===========================
// PID CONTROLLER PARAMETERS
// ===========================

#define PID_KP 2.0              // Proportional gain
#define PID_KI 0.5              // Integral gain
#define PID_KD 1.0              // Derivative gain
#define PID_OUTPUT_MIN -100     // Minimum PID output
#define PID_OUTPUT_MAX 100      // Maximum PID output

// ===========================
// PUMP CONTROL PARAMETERS
// ===========================

#define PUMP_MIN_RUNTIME_MS 500     // Minimum pump run time to avoid rapid switching
#define PUMP_MAX_RUNTIME_MS 10000   // Maximum continuous pump run time (safety)
#define PUMP_COOLDOWN_MS 1000       // Cooldown between pump cycles

// ===========================
// COMMUNICATION SETTINGS
// ===========================

// ESP-NOW
#define CHANNEL 1                    // WiFi channel for ESP-NOW
extern uint8_t stationMAC[6];  // Declare only, don't define. To be replaced with the mac address of esp at station

// ===========================
// DATA LOGGING SETTINGS
// ===========================

#define LOG_INTERVAL_MS 1000         // Log data every 1 second
#define LOG_FILENAME "/mission_log.txt"
#define MAX_LOG_SIZE_KB 500          // Maximum log file size

// ===========================
// SAFETY PARAMETERS
// ===========================

#define EMERGENCY_SURFACE_DEPTH 0.1  // Depth to consider "surfaced" (sensor reading)
#define LOW_BATTERY_THRESHOLD 3.3    // Voltage threshold for low battery warning (if monitoring)
#define SENSOR_TIMEOUT_MS 5000       // Max time to wait for valid sensor reading

// ===========================
// SYSTEM STATES
// ===========================

enum MissionState {
    IDLE,                       // Waiting for START command at surface
    CALIBRATING,                // Calibrating sensors at surface
    
    // Profile 1
    DESCENDING_MISSION_1_P1,    // Profile 1: Descending to 2.0m (bottom at 2.5m)
    AT_DEPTH_MISSION_1_P1,      // Profile 1: Holding at 2.0m depth
    ASCENDING_TO_MISSION_2_P1,  // Profile 1: Ascending to 0.4m (top at 0.4m)
    AT_DEPTH_MISSION_2_P1,      // Profile 1: Holding at 0.4m depth
    PROFILE_1_COMPLETE,         // Profile 1 done, transition to Profile 2
    
    // Profile 2
    DESCENDING_MISSION_1_P2,    // Profile 2: Descending to 2.0m (bottom at 2.5m)
    AT_DEPTH_MISSION_1_P2,      // Profile 2: Holding at 2.0m depth
    ASCENDING_TO_MISSION_2_P2,  // Profile 2: Ascending to 0.4m (top at 0.4m)
    AT_DEPTH_MISSION_2_P2,      // Profile 2: Holding at 0.4m depth
    PROFILE_2_COMPLETE,         // Profile 2 done, ready to surface
    
    ASCENDING_TO_SURFACE,       // Final ascent after all profiles complete
    SURFACED,                   // All profiles complete, at surface
    EMERGENCY                   // Emergency state - surface immediately
};
// ===== H-Bridge Control Pins =====
// Pump Pins
#define PUMP_IN_PIN     25   
#define PUMP_OUT_PIN    26   

// Pump States
#define PUMP_OFF        0 // Motor stopped
#define PUMP_FILL       1 //(absorb water)
#define PUMP_EMPTY      2 //(release water)

#endif // CONFIG_H