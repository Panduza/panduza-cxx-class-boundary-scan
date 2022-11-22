from time import sleep
from panduza import *

io_interfaces = {}

Core.LoadAliases(
{
    "broker_1":
    {
        "url": "localhost",
        "port": 1883,
        "interfaces": 
        {
            "IO_E18" : "pza/rpi4_test/artys7/IO_E18",
            "IO_F13" : "pza/rpi4_test/artys7/IO_F13",
            "IO_E13" : "pza/rpi4_test/artys7/IO_E13",
            "IO_H15" : "pza/rpi4_test/artys7/IO_H15",
            "IO_J15" : "pza/rpi4_test/artys7/IO_J15",
            "IO_G17" : "pza/rpi4_test/artys7/IO_G17",
            "IO_F15" : "pza/rpi4_test/artys7/IO_F15",
            "IO_E15" : "pza/rpi4_test/artys7/IO_E15",
            "IO_F18" : "pza/rpi4_test/artys7/IO_F18",
            "IO_E14" : "pza/rpi4_test/artys7/IO_E14",
            "PA5" : "pza/rpi4_test/STM32/PA5"
        }
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
        sleep(0.1)
        io_name.value.set(0, ensure=True)
