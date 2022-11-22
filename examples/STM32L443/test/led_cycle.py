from time import sleep
from panduza import *

Core.LoadAliases(
{
    "broker_1":
    {
        "url": "localhost",
        "port": 1883,
        "interfaces": 
        {
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
