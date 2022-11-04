from panduza import *

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
            "IO_H5" : "pza/rpi4_test/artya7/IO_H5",
            "IO_J5" : "pza/rpi4_test/artya7/IO_J5",
            "IO_T9" : "pza/rpi4_test/artya7/IO_T9",
            "IO_T10" : "pza/rpi4_test/artya7/IO_T10",
            "IO_G6" : "pza/rpi4_test/artya7/IO_G6",
            "IO_F6" : "pza/rpi4_test/artya7/IO_F6",
            "IO_E1" : "pza/rpi4_test/artya7/IO_E1",
            "IO_G3" : "pza/rpi4_test/artya7/IO_G3",
            "IO_J4" : "pza/rpi4_test/artya7/IO_J4",
            "IO_G4" : "pza/rpi4_test/artya7/IO_G4",
            "IO_J3" : "pza/rpi4_test/artya7/IO_J3",
            "IO_J2" : "pza/rpi4_test/artya7/IO_J2",
            "IO_H4" : "pza/rpi4_test/artya7/IO_H4",
            "IO_K1" : "pza/rpi4_test/artya7/IO_K1",
            "IO_H6" : "pza/rpi4_test/artya7/IO_H6",
            "IO_K2" : "pza/rpi4_test/artya7/IO_K2"
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
        io_name.value.set(0, ensure=True)
