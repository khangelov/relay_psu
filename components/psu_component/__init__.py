from esphome.components import sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID

CONF_VOLT_IN = "volt_in"
CONF_AMP_IN = "amp_in"
CONF_WATT_IN = "watt_in"
CONF_VOLT_OUT = "volt_out"
CONF_AMP_OUT = "amp_out"
CONF_WATT_OUT = "watt_out"
CONF_INTERNAL_TEMP = "internal_temp"
CONF_FAN_RPM = "fan_rpm"

# Define the namespace and component class
psu_sensor_ns = cg.esphome_ns.namespace("psu_sensor")
PSUSensorComponent = psu_sensor_ns.class_("PSUSensorComponent", cg.PollingComponent)

# Configuration schema for the component
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(PSUSensorComponent),
        cv.Optional(CONF_VOLT_IN): sensor.sensor_schema(unit_of_measurement="V", accuracy_decimals=2),
        cv.Optional(CONF_AMP_IN): sensor.sensor_schema(unit_of_measurement="A", accuracy_decimals=2),
        cv.Optional(CONF_WATT_IN): sensor.sensor_schema(unit_of_measurement="W", accuracy_decimals=2),
        cv.Optional(CONF_VOLT_OUT): sensor.sensor_schema(unit_of_measurement="V", accuracy_decimals=2),
        cv.Optional(CONF_AMP_OUT): sensor.sensor_schema(unit_of_measurement="A", accuracy_decimals=2),
        cv.Optional(CONF_WATT_OUT): sensor.sensor_schema(unit_of_measurement="W", accuracy_decimals=2),
        cv.Optional(CONF_INTERNAL_TEMP): sensor.sensor_schema(unit_of_measurement="°C", accuracy_decimals=2),
        cv.Optional(CONF_FAN_RPM): sensor.sensor_schema(unit_of_measurement="RPM", accuracy_decimals=0),
    }
).extend(cv.polling_component_schema("60s"))  # Update every 60 seconds

async def to_code(config):
    # Create a new instance of PSUSensorComponent
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cg.register_polling_component(var, config)

    # Register each sensor
    for key, attr in [
        (CONF_VOLT_IN, "volt_in_sensor"),
        (CONF_AMP_IN, "amp_in_sensor"),
        (CONF_WATT_IN, "watt_in_sensor"),
        (CONF_VOLT_OUT, "volt_out_sensor"),
        (CONF_AMP_OUT, "amp_out_sensor"),
        (CONF_WATT_OUT, "watt_out_sensor"),
        (CONF_INTERNAL_TEMP, "internal_temp_sensor"),
        (CONF_FAN_RPM, "fan_rpm_sensor"),
    ]:
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, f"set_{attr}")(sens))

    # Add I2C dependency (Wire library)
    cg.add_library("Wire", None)
