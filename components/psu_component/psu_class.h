#ifndef PSU_DPS_h
#define PSU_DPS_h
#include <Wire.h>
#if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif



class PSU_DPS
{
public:
    float    volt_in(void);
    float    amp_in(void);
    float    watt_in(void);
    float    volt_out(void);
    float    amp_out(void);
    float    watt_out(void);
    float    internal_Temp(void);
    float    Fan_RPM(void);

private:
    TwoWire *i2cPort;
    byte     checkCRC(uint16_t message_from_sensor, uint8_t check_value_from_sensor);
    uint16_t readValue(byte cmd);
};

#endif