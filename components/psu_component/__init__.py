import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import polling_component  # ✅ Important
from esphome import automation
from esphome.const import CONF_ID

psu_component_ns = cg.namespace('psu_component')
PSUComponent = psu_component_ns.class_('PSUComponent', cg.Component, polling_component.PollingComponent)

CONF_RELAY_PIN = "relay_pin"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(PSUComponent),
    cv.Required(CONF_RELAY_PIN): cv.pin,
}).extend(cv.COMPONENT_SCHEMA).extend(polling_component.POLLING_COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await cg.register_component(var, config)
    await polling_component.register_polling_component(var, config)

    relay_pin = await cg.gpio_pin_expression(config[CONF_RELAY_PIN])
    cg.add(var.set_relay_pin(relay_pin))
