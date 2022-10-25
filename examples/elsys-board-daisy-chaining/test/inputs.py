from panduza import *
import time

Core.LoadAliases(
{
    "broker_1":
    {
        "url": "localhost",
        "port": 1883,
        "interfaces": {
            "IO_G15": "pza/rpi4_test/artys7/IO_G15",
            "IO_K16": "pza/rpi4_test/artys7/IO_K16",
            "IO_J16": "pza/rpi4_test/artys7/IO_J16",
            "IO_H13": "pza/rpi4_test/artys7/IO_H13",
            "IO_H14": "pza/rpi4_test/artys7/IO_H14",
            "IO_H18": "pza/rpi4_test/artys7/IO_H18",
            "IO_G18": "pza/rpi4_test/artys7/IO_G18",
            "IO_M5": "pza/rpi4_test/artys7/IO_M5",
            "IO_D9": "pza/rpi4_test/artya7/IO_D9",
            "IO_C9": "pza/rpi4_test/artya7/IO_C9",
            "IO_B9": "pza/rpi4_test/artya7/IO_B9",
            "IO_B8": "pza/rpi4_test/artya7/IO_B8",
            "IO_A8": "pza/rpi4_test/artya7/IO_A8",
            "IO_C11": "pza/rpi4_test/artya7/IO_C11",
            "IO_C10": "pza/rpi4_test/artya7/IO_C10",
            "IO_A10": "pza/rpi4_test/artya7/IO_A10"
        }
    }
})

IO_G15 = Io(alias="IO_G15")
IO_K16 = Io(alias="IO_K16")
IO_J16 = Io(alias="IO_J16")
IO_H13 = Io(alias="IO_H13")
IO_H14 = Io(alias="IO_H14")
IO_H18 = Io(alias="IO_H18")
IO_G18 = Io(alias="IO_G18")
IO_M5 = Io(alias="IO_M5")
IO_D9 = Io(alias="IO_D9")
IO_C9 = Io(alias="IO_C9")
IO_B9 = Io(alias="IO_B9")
IO_B8 = Io(alias="IO_B8")
IO_A8 = Io(alias="IO_A8")
IO_C11 = Io(alias="IO_C11")
IO_C10 = Io(alias="IO_C10")
IO_A10 = Io(alias="IO_A10")

IO_G15.direction.set("in", ensure=True)
IO_K16.direction.set("in", ensure=True)
IO_J16.direction.set("in", ensure=True)
IO_H13.direction.set("in", ensure=True)
IO_H14.direction.set("in", ensure=True)
IO_H18.direction.set("in", ensure=True)
IO_G18.direction.set("in", ensure=True)
IO_M5.direction.set("in", ensure=True)
IO_D9.direction.set("in", ensure=True)
IO_C9.direction.set("in", ensure=True)
IO_B9.direction.set("in", ensure=True)
IO_B8.direction.set("in", ensure=True)
IO_A8.direction.set("in", ensure=True)
IO_C11.direction.set("in", ensure=True)
IO_C10.direction.set("in", ensure=True)
IO_A10.direction.set("in", ensure=True)

print("Every Switch and button are set to input, their states are visible in any mqtt client (eg: MQTT Explorer)")