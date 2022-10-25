from panduza import *

io_interfaces = {}

def create_interfaces_line(list, group_name):
    for io_name in list:
        io_interfaces[io_name] = "pza/rpi4_test/" + group_name + "/" + io_name


artys7_group_name = "artys7"
artys7_out = ["IO_J16", "IO_H13", "IO_H14", "IO_H18"]
# "IO_G15", "IO_K16", , "IO_G18", "IO_M5"

create_interfaces_line(artys7_out, artys7_group_name)

artya7_group_name = "artya7"
artya7_out = ["IO_B9", "IO_B8", "IO_A8", "IO_C11"]
# "IO_D9", "IO_C9", , "IO_C10", "IO_A10"

create_interfaces_line(artya7_out, artya7_group_name)

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

for io_name in io_keys:
    io_name = Io(alias=io_name)
    io_name.direction.set("in", ensure=True)

print("Every Switch and button are set to input, their states are visible in any mqtt client (eg: MQTT Explorer)")