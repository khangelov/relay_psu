#include "dps1200_sensor.h"
#include "esphome.h"

namespace esphome {
namespace dps1200_sensor {

void DPS1200Sensor::setup() {
  // Initialize I2C (Wire is already included in ESPHome)
  Wire.begin();
}

void DPS1200Sensor::update() {
  uint8_t reg[6] = {0x08, 0x0a, 0x0e, 0x10, 0x1c, 0x1e};
  uint8_t i, cs, regCS;
  uint16_t msg[3];
  float stat;

  for (i = 0; i < 6; i++) {
    cs = (address_ << 1) + reg[i];
    regCS = ((0xff - cs) + 1) & 0xff; // Calculate checksum

    Wire.beginTransmission(address_);
    Wire.write(reg[i]);
    Wire.write(regCS);
    Wire.endTransmission();
    esphome::delay(100); // Delay for 1 millisecond

    Wire.requestFrom((int)address_, 3);
    msg[0] = Wire.read();
    msg[1] = Wire.read();
    msg[2] = Wire.read();
    uint16_t ret = (msg[1] << 8) + msg[0]; // Shift to MSB + LSB

    // Process register data
    switch (i) {
      case 0: // Grid Voltage
        stat = ret / 32.0;
        volt_in->publish_state(stat);
        break;
      case 1: // Grid Current
        stat = ret / 128.0;
        amp_in->publish_state(stat);
        break;
      case 2: // Output Voltage
        stat = ret / 256.0;
        volt_out->publish_state(stat);
        break;
      case 3: // Output Current
        stat = ret / 128.0;
        amp_out->publish_state(stat);
        break;
      case 4: // Internal Temperature
        stat = ret / 32.0;
        internal_temp->publish_state(f2c(stat));
        break;
      case 5: // Fan RPM
        stat = ret;
        fan_rpm->publish_state(stat);
        break;
    }
  }

  // Publish wattage (calculated if needed, currently not implemented in original code)
  // watt_in->publish_state(volt_in->state * amp_in->state);
  // watt_out->publish_state(volt_out->state * amp_out->state);
}

}  // namespace dps1200_sensor

}  // namespace esphome




