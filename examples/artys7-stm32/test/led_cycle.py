from time import sleep
from panduza import *

io_interfaces = {}

def create_interfaces_line(list, group_name):
    for io_name in list:
        io_interfaces[io_name] = "pza/rpi4_test/" + group_name + "/" + io_name


artys7_group_name = "artys7"
artys7_out = ["IO_E18", "IO_F13", "IO_E13","IO_H15", "IO_J15", "IO_G17", "IO_F15", "IO_E15", "IO_F18", "IO_E14"]
create_interfaces_line(artys7_out, artys7_group_name)

stm_group_name = "STM32"
stm_out = ["PA5"]
create_interfaces_line(stm_out, stm_group_name)

Core.LoadAliases(
{
    "broker_1":
    {
        "url": "localhost",
        "port": 1883,
        "interfaces": io_interfaces
    }
})
# print(Core.Aliases.keys())
io_keys = Core.Aliases.keys()

io_list = []

for io_name in io_keys:
    io_name = Io(alias=io_name)
    io_name.direction.set("out", ensure=True)
    io_list.append(io_name)

while True:
    for io_name in io_list:
        io_name.value.set(1, ensure=True)
        io_name.value.set(0, ensure=True)
