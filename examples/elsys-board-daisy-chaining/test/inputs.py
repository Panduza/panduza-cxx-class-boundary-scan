from panduza import *
import paho.mqtt.client as paho

mqtt_address = "localhost"

def on_message(client, userdata, message):
    print("message received  ",str(message.payload.decode("utf-8")),"topic",message.topic)

io_interfaces = {}

def create_interfaces_line(list, group_name):
    for io_name in list:
        io_interfaces[io_name] = "pza/rpi4_test/" + group_name + "/" + io_name


artys7_group_name = "artys7"
artys7_in = ["IO_J16", "IO_H13", "IO_H14", "IO_H18"]
# "IO_G15", "IO_K16", , "IO_G18", "IO_M5"

create_interfaces_line(artys7_in, artys7_group_name)

artya7_group_name = "artya7"
artya7_in = ["IO_B9", "IO_B8", "IO_A8", "IO_C11"]
# "IO_D9", "IO_C9", , "IO_C10", "IO_A10"

create_interfaces_line(artya7_in, artya7_group_name)


# stm_group_name = "STM32"
# stm_in = ["PC13"]
# create_interfaces_line(stm_in, stm_group_name)

Core.LoadAliases(
{
    "broker_1":
    {
        "url": "localhost",
        "port": 1883,
        "interfaces": io_interfaces
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
    # io_name = Io(alias="PC13")
    # print(io_name.value.get())
# print("Every Switch and button are set to input, their states are visible in any mqtt client (eg: MQTT Explorer)")