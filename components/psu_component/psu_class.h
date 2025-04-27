#include "psu_class.h"
#include "esphome.h"
#include <Wire.h>

// Define the I2C address for the PSU
#define ADDR_PSU_DPS 0x58

// Function to read a value from the sensor using the I2C bus
uint16_t PSU_DPS::readValue(byte cmd) {
    uint8_t msg[3];
    uint16_t result = 0;

    // Begin I2C transmission
    Wire.beginTransmission(ADDR_PSU_DPS);
    Wire.write(cmd);  // Write the command
    Wire.endTransmission();
    delay(1);  // Short delay for I2C communication

    // Request 3 bytes of data from the sensor
    Wire.requestFrom(ADDR_PSU_DPS, 3);

    if (Wire.available() == 3) {
        msg[0] = Wire.read();  // Read first byte
        msg[1] = Wire.read();  // Read second byte
        msg[2] = Wire.read();  // Read third byte
        result = (msg[1] << 8) | msg[0];  // Combine the bytes into a single 16-bit value
    }
    
    return result;
}

// Function to check CRC (if needed for data validation)
byte PSU_DPS::checkCRC(uint16_t message_from_sensor, uint8_t check_value_from_sensor) {
    uint8_t calc_crc = 0;
    // Add your CRC calculation logic here if needed (check datasheet for CRC algo)
    return calc_crc == check_value_from_sensor;
}

// Methods to get sensor values
float PSU_DPS::volt_in() {
    uint16_t raw = readValue(0x08);  // Read grid voltage register
    return raw / 32.0;  // Apply scale factor
}

float PSU_DPS::amp_in() {
    uint16_t raw = readValue(0x0a);  // Read grid current register
    return raw / 128.0;  // Apply scale factor
}

float PSU_DPS::watt_in() {
    uint16_t raw = readValue(0x0e);  // Read watt input register (not used directly in original code)
    return raw;  // No scale factor, use raw value or apply if necessary
}

float PSU_DPS::volt_out() {
    uint16_t raw = readValue(0x10);  // Read output voltage register
    return raw / 256.0;  // Apply scale factor
}

float PSU_DPS::amp_out() {
    uint16_t raw = readValue(0x1c);  // Read output current register
    return raw / 128.0;  // Apply scale factor
}

float PSU_DPS::watt_out() {
    uint16_t raw = readValue(0x1e);  // Read output wattage register (not used directly)
    return raw;  // No scale factor, use raw value or apply if necessary
}

float PSU_DPS::internal_Temp() {
    uint16_t raw = readValue(0x14);  // Read internal temperature register (example)
    return (raw / 32.0) * 1.8 + 32;  // Convert from Celsius to Fahrenheit
}

float PSU_DPS::Fan_RPM() {
    uint16_t raw = readValue(0x16);  // Read fan RPM register (example)
    return raw;  // Return raw RPM data
}
