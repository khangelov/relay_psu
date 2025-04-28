import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ADDRESS,
    CONF_ID,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_WATT,
    UNIT_CELSIUS,
)

DEPENDENCIES = ["i2c"]

dps1200_sensor_ns = cg.esphome_ns.namespace("dps1200_sensor")
DPS1200Sensor = dps1200_sensor_ns.class_("DPS1200Sensor", cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DPS1200Sensor),
        cv.Optional(CONF_ADDRESS, default=0x58): cv.i2c_address,
        cv.Optional("volt_in"): sensor.sensor_schema(UNIT_VOLT, accuracy_decimals=2),
        cv.Optional("amp_in"): sensor.sensor_schema(UNIT_AMPERE, accuracy_decimals=2),
        cv.Optional("watt_in"): sensor.sensor_schema(UNIT_WATT, accuracy_decimals=2),
        cv.Optional("volt_out"): sensor.sensor_schema(UNIT_VOLT, accuracy_decimals=2),
        cv.Optional("amp_out"): sensor.sensor_schema(UNIT_AMPERE, accuracy_decimals=2),
        cv.Optional("watt_out"): sensor.sensor_schema(UNIT_WATT, accuracy_decimals=2),
        cv.Optional("internal_temp"): sensor.sensor_schema(UNIT_CELSIUS, accuracy_decimals=1),
        cv.Optional("fan_rpm"): sensor.sensor_schema(unit_of_measurement="RPM", accuracy_decimals=0),
    }
).extend(cv.polling_component_schema("15s"))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if CONF_ADDRESS in config:
        cg.add(var.set_address(config[CONF_ADDRESS]))

    for key in [
        "volt_in",
        "amp_in",
        "watt_in",
        "volt_out",
        "amp_out",
        "watt_out",
        "internal_temp",
        "fan_rpm",
    ]:
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, f"set_{key}")(sens))