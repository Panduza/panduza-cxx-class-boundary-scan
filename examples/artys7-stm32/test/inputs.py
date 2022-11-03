from panduza import *
import paho.mqtt.client as paho

mqtt_address = "localhost"

def on_message(client, userdata, message):
    print("message received  ",str(message.payload.decode("utf-8")),"topic",message.topic)

Core.LoadAliases(
{
    "broker_1":
    {
        "url": "localhost",
        "port": 1883,
        "interfaces": 
        {
            "IO_J16" : "pza/rpi4_test/artys7/IO_J16",
            "IO_H13" : "pza/rpi4_test/artys7/IO_H13",
            "IO_H14" : "pza/rpi4_test/artys7/IO_H14",
            "IO_H18" : "pza/rpi4_test/artys7/IO_H18",
            "IO_G15" : "pza/rpi4_test/artys7/IO_G15",
            "IO_K16" : "pza/rpi4_test/artys7/IO_K16",
            "IO_G18" : "pza/rpi4_test/artys7/IO_G18",
            "IO_M5" : "pza/rpi4_test/artys7/IO_M5",
            "PC13" : "pza/rpi4_test/STM32/PC13"
        }
    }
})

io_keys = Core.Aliases.keys()

io_list = []

client = paho.Client("test")
client.connect(mqtt_address)
client.loop_start()

for io_name in io_keys:
    io_name = Io(alias=io_name)
    io_name.direction.set("in", ensure=True)

for topic in Core.Aliases:
    client.subscribe(Core.BaseTopicFromAlias(topic) + "/atts/value")
    client.on_message = on_message
