#pragma once
#include "esphome.h"

class PSUSensorComponent : public PollingComponent {
 public:
  PSUSensorComponent() : PollingComponent(60000) {}  // Update every 60 seconds

  void setup() override {
    // Initialize I2C
    Wire.begin();
    // Add any PSU sensor initialization code here
  }

  void update() override {
    // Placeholder: Replace with actual I2C read logic from psu.h/psu_class.h
    float volt_in = 0.0;  // Example: Read from I2C
    float amp_in = 0.0;
    float watt_in = volt_in * amp_in;
    float volt_out = 0.0;
    float amp_out = 0.0;
    float watt_out = volt_out * amp_out;
    float internal_temp = 0.0;
    float fan_rpm = 0.0;

    // Publish sensor values
    if (volt_in_sensor_) volt_in_sensor_->publish_state(volt_in);
    if (amp_in_sensor_) amp_in_sensor_->publish_state(amp_in);
    if (watt_in_sensor_) watt_in_sensor_->publish_state(watt_in);
    if (volt_out_sensor_) volt_out_sensor_->publish_state(volt_out);
    if (amp_out_sensor_) amp_out_sensor_->publish_state(amp_out);
    if (watt_out_sensor_) watt_out_sensor_->publish_state(watt_out);
    if (internal_temp_sensor_) internal_temp_sensor_->publish_state(internal_temp);
    if (fan_rpm_sensor_) fan_rpm_sensor_->publish_state(fan_rpm);
  }

  // Setters for sensor pointers
  void set_volt_in_sensor(sensor::Sensor *s) { volt_in_sensor_ = s; }
  void set_amp_in_sensor(sensor::Sensor *s) { amp_in_sensor_ = s; }
  void set_watt_in_sensor(sensor::Sensor *s) { watt_in_sensor_ = s; }
  void set_volt_out_sensor(sensor::Sensor *s) { volt_out_sensor_ = s; }
  void set_amp_out_sensor(sensor::Sensor *s) { amp_out_sensor_ = s; }
  void set_watt_out_sensor(sensor::Sensor *s) { watt_out_sensor_ = s; }
  void set_internal_temp_sensor(sensor::Sensor *s) { internal_temp_sensor_ = s; }
  void set_fan_rpm_sensor(sensor::Sensor *s) { fan_rpm_sensor_ = s; }

 protected:
  sensor::Sensor *volt_in_sensor_{nullptr};
  sensor::Sensor *amp_in_sensor_{nullptr};
  sensor::Sensor *watt_in_sensor_{nullptr};
  sensor::Sensor *volt_out_sensor_{nullptr};
  sensor::Sensor *amp_out_sensor_{nullptr};
  sensor::Sensor *watt_out_sensor_{nullptr};
  sensor::Sensor *internal_temp_sensor_{nullptr};
  sensor::Sensor *fan_rpm_sensor_{nullptr};
};
