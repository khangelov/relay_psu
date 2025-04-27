#include "esphome.h"
#include "psu_class.h"
//#include <Wire.h>
#define LOOP_DELAY 2000

// Enable / Disable defines
#define ADJUST_TMP_F 18 // TODO best way to calibrate?
float intake_tmp_c = 0.0;
float internal_tmp_c = 0.0;

#ifdef ENADD_STATS
// additional vals to read
float volt_in = 0.0;
float amp_in = 0.0;
float watt_in = 0.0;
float volt_out = 0.0;
float amp_out = 0.0;
float watt_out = 0.0;
#endif

uint8_t addy = 0x58; // Supply Address verify w/I2C scanner
uint8_t i; // index for loop
uint8_t reg[6] = {0x08, 0x0a, 0x0e, 0x10, 0x1c, 0x1e}; 
uint8_t cs, regCS;
uint16_t msg[3]; // Received messages from PS
float ret, Stat; // reused calculated values

float f2c(uint16_t temp) {
  return (temp - 32) * 0.5556;
}

class PSUSensor : public PollingComponent {
 public:
  // Constructor with proper setup
  PSUSensor() : PollingComponent(15000) {}

  // Sensor definitions (don't use `new`)
  Sensor *volt_in = new Sensor(); 
  Sensor *amp_in = new Sensor(); 
  Sensor *watt_in = new Sensor(); 
  Sensor *volt_out = new Sensor(); 
  Sensor *amp_out = new Sensor(); 
  Sensor *watt_out = new Sensor();
  Sensor *internal_Temp = new Sensor(); 
  Sensor *Fan_RPM = new Sensor(); 

  void setup() override {
    // Setup I2C communication
    Wire.begin();  // Initialize I2C
  }

  void update() override {
    // Loop through registers to read data
    for (i = 0; i < 6; i++) {
      cs = (addy << 1) + reg[i];
      regCS = ((0xff - cs) + 1) & 0xff;  // Calculate checksum of add+reg

      // Communicate with DPS1200 via I2C
      Wire.beginTransmission(addy);  // Start I2C with DPS1200
      Wire.write(reg[i]);            // Write register address
      Wire.write(regCS);             // Write checksum
      Wire.endTransmission();        // End I2C communication
      delay(1);                      // Short delay between operations

      Wire.requestFrom((int)addy, 3);  
      msg[0] = Wire.read();
      msg[1] = Wire.read();
      msg[2] = Wire.read();
      ret = (msg[1] << 8) + msg[0];  // Shift to MSB + LSB
      
      // Parse each register based on index
      if (i == 0) { 
        Stat = ret / 32; // Grid Voltage
        volt_in->publish_state(Stat); 
      }
      if (i == 1) { 
        Stat = ret / 128; // Grid Current
        amp_in->publish_state(Stat);
      }
      if (i == 2) { 
        Stat = ret / 256; // Output Voltage
        volt_out->publish_state(Stat);  
      }
      if (i == 3) { 
        Stat = ret / 128; // Output Current
        amp_out->publish_state(Stat); 
      }
      if (i == 4) { 
        Stat = ret / 32; // Internal Temp (Fahrenheit)
        internal_Temp->publish_state(f2c(Stat)); 
      }
      if (i == 5) { 
        Stat = ret; // Fan RPM
        Fan_RPM->publish_state(Stat); 
      }
    }
  }
};
