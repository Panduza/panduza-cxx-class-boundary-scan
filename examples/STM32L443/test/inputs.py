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

while True:
    continue

# print("Every Switch and button are set to input, their states are visible in any mqtt client (eg: MQTT Explorer)")