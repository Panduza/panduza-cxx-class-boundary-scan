from panduza import *
import time

Core.LoadAliases(
{
    "broker_1":
    {
        "url": "localhost",
        "port": 1883,
        "interfaces": {
            "IO_E18": "pza/rpi4_test/artys7/IO_E18",
            "IO_F13": "pza/rpi4_test/artys7/IO_F13",
            "IO_E13": "pza/rpi4_test/artys7/IO_E13",
            "IO_H15": "pza/rpi4_test/artys7/IO_H15",
            "IO_J15": "pza/rpi4_test/artys7/IO_J15",
            "IO_G17": "pza/rpi4_test/artys7/IO_G17",
            "IO_F15": "pza/rpi4_test/artys7/IO_F15",
            "IO_E15": "pza/rpi4_test/artys7/IO_E15",
            "IO_F18": "pza/rpi4_test/artys7/IO_F18",
            "IO_E14": "pza/rpi4_test/artys7/IO_E14",
            "IO_H5": "pza/rpi4_test/artya7/IO_H5",
            "IO_J5": "pza/rpi4_test/artya7/IO_J5",
            "IO_T9": "pza/rpi4_test/artya7/IO_T9",
            "IO_T10": "pza/rpi4_test/artya7/IO_T10",
            "IO_G6": "pza/rpi4_test/artya7/IO_G6",
            "IO_F6": "pza/rpi4_test/artya7/IO_F6",
            "IO_E1": "pza/rpi4_test/artya7/IO_E1",
            "IO_G3": "pza/rpi4_test/artya7/IO_G3",
            "IO_J4": "pza/rpi4_test/artya7/IO_J4",
            "IO_G4": "pza/rpi4_test/artya7/IO_G4",
            "IO_J3": "pza/rpi4_test/artya7/IO_J3",
            "IO_J2": "pza/rpi4_test/artya7/IO_J2",
            "IO_H4": "pza/rpi4_test/artya7/IO_H4",
            "IO_K1": "pza/rpi4_test/artya7/IO_K1",
            "IO_H6": "pza/rpi4_test/artya7/IO_H6",
            "IO_K2": "pza/rpi4_test/artya7/IO_K2",
        }
    }
})

IO_E18 = Io(alias="IO_E18")
IO_F13 = Io(alias="IO_F13")
IO_E13 = Io(alias="IO_E13")
IO_H15 = Io(alias="IO_H15")
IO_J15 = Io(alias="IO_J15")
IO_G17 = Io(alias="IO_G17")
IO_F15 = Io(alias="IO_F15")
IO_E15 = Io(alias="IO_E15")
IO_F18 = Io(alias="IO_F18")
IO_E14 = Io(alias="IO_E14")
IO_H5 = Io(alias="IO_H5")
IO_J5 = Io(alias="IO_J5")
IO_T9 = Io(alias="IO_T9")
IO_T10 = Io(alias="IO_T10")
IO_G6 = Io(alias="IO_G6")
IO_F6 = Io(alias="IO_F6")
IO_E1 = Io(alias="IO_E1")
IO_G3 = Io(alias="IO_G3")
IO_J4 = Io(alias="IO_J4")
IO_G4 = Io(alias="IO_G4")
IO_J3 = Io(alias="IO_J3")
IO_J2 = Io(alias="IO_J2")
IO_H4 = Io(alias="IO_H4")
IO_K1 = Io(alias="IO_K1")
IO_H6 = Io(alias="IO_H6")
IO_K2 = Io(alias="IO_K2")

IO_E18.direction.set("out", ensure=True)
IO_F13.direction.set("out", ensure=True)
IO_E13.direction.set("out", ensure=True)
IO_H15.direction.set("out", ensure=True)
IO_J15.direction.set("out", ensure=True)
IO_G17.direction.set("out", ensure=True)
IO_F15.direction.set("out", ensure=True)
IO_E15.direction.set("out", ensure=True)
IO_F18.direction.set("out", ensure=True)
IO_E14.direction.set("out", ensure=True)
IO_H5.direction.set("out", ensure=True)
IO_J5.direction.set("out", ensure=True)
IO_T9.direction.set("out", ensure=True)
IO_T10.direction.set("out", ensure=True)
IO_G6.direction.set("out", ensure=True)
IO_F6.direction.set("out", ensure=True)
IO_E1.direction.set("out", ensure=True)
IO_G3.direction.set("out", ensure=True)
IO_J4.direction.set("out", ensure=True)
IO_G4.direction.set("out", ensure=True)
IO_J3.direction.set("out", ensure=True)
IO_J2.direction.set("out", ensure=True)
IO_H4.direction.set("out", ensure=True)
IO_K1.direction.set("out", ensure=True)
IO_H6.direction.set("out", ensure=True)
IO_K2.direction.set("out", ensure=True)

while True :
    IO_K2.value.set(0, ensure=True)
    IO_E18.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_E18.value.set(0, ensure=True)
    IO_F13.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_F13.value.set(0, ensure=True)
    IO_E13.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_E13.value.set(0, ensure=True)
    IO_H15.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_H15.value.set(0, ensure=True)
    IO_J15.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_J15.value.set(0, ensure=True)
    IO_G17.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_G17.value.set(0, ensure=True)
    IO_F15.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_F15.value.set(0, ensure=True)
    IO_E15.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_E15.value.set(0, ensure=True)
    IO_F18.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_F18.value.set(0, ensure=True)
    IO_E14.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_E14.value.set(0, ensure=True)
    IO_H5.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_H5.value.set(0, ensure=True)
    IO_J5.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_J5.value.set(0, ensure=True)
    IO_T9.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_T9.value.set(0, ensure=True)
    IO_T10.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_T10.value.set(0, ensure=True)
    IO_G6.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_G6.value.set(0, ensure=True)
    IO_F6.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_F6.value.set(0, ensure=True)
    IO_E1.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_E1.value.set(0, ensure=True)
    IO_G3.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_G3.value.set(0, ensure=True)
    IO_J4.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_J4.value.set(0, ensure=True)
    IO_G4.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_G4.value.set(0, ensure=True)
    IO_J3.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_J3.value.set(0, ensure=True)
    IO_J2.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_J2.value.set(0, ensure=True)
    IO_H4.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_H4.value.set(0, ensure=True)
    IO_K1.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_K1.value.set(0, ensure=True)
    IO_H6.value.set(1, ensure=True)
    # time.sleep(0.01)
    IO_H6.value.set(0, ensure=True)
    IO_K2.value.set(1, ensure=True)
    # time.sleep(0.01)
