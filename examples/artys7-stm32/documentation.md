# Documentation

- [Documentation](#documentation)
  - [1. Introduction](#1-introduction)
  - [2. First Comments and Requirements](#2-first-comments-and-requirements)
  - [3. Descriptions](#3-descriptions)
    - [3.1. tree.json](#31-treejson)
    - [3.2. Device identification](#32-device-identification)
    - [3.3. "group_name"](#33-group_name)
    - [3.4. AUTODETECT](#34-autodetect)
  - [4. Example : Daisy Chaining](#4-example--daisy-chaining)
    - [4.1. Introduction](#41-introduction)
    - [4.2. Hardware](#42-hardware)
    - [4.3. Wiring](#43-wiring)
    - [4.4. Running the project](#44-running-the-project)
    - [4.5. Testing](#45-testing)
  - [5. Common issues](#5-common-issues)
    - [5.1. "No tree file found"](#51-no-tree-file-found)
    - [5.2 "No probe detected"](#52-no-probe-detected)
    - [5.3. "Probe xxx : not found"](#53-probe-xxx--not-found)
    - [5.4 "No device detected"](#54-no-device-detected)
    - [5.5. "No BSDL File found for device no : x, it's idcode is : xxxxxxxxxx"](#55-no-bsdl-file-found-for-device-no--x-its-idcode-is--xxxxxxxxxx)
  - [6. Additional Information](#6-additional-information)

## 1. Introduction

In this documentation, the setup for the daisy chaining of two devices (Nucleo STM32L443 and ARTY S7) will be described.

## 2. First Comments and Requirements

Do be able to start the program, a few things will be needed.

- The whole project was developed and tested on **Ubuntu**, which allow an easiest installation of Docker and some dependencies.

- You will need to install Docker and docker-compose, otherwise, it won't work. To start the program, we are pulling images from the platform repo and mosquitto, which need theses libraries. You can install [Docker here](https://docs.docker.com/engine/install/ubuntu/) and [docker-compose here (French link)](https://doc.ubuntu-fr.org/docker-compose).

- For this example, the probe that we are using have as its probe name : **FT2232HL_A_FT6EE4EEA** . This probe should be identifiable with a sticker on it. In the case that you can't use this probe, please refer to the issue **5.3**.

- One way to send commands to the program was with a software called : **MQTT Explorer**. This will be used in the testing to show some commands and will allows you to test and play a bit with the program.
To install the software please [click here](http://mqtt-explorer.com/).
Other way of testing with python will be defined but they will be presented later.

This documentation is done while assuming that you have some knowledge with MQTT.

- ***Due to some EM parasite, the breadboard is not used in this example. It leads to some issues with the detection of the devices. Moreover, a certain order is needed to get this example work, which will be defined later during the wiring.***

## 3. Descriptions

### 3.1. tree.json

The panduza platform need one file to work : the **tree.json** file. This file is essential as it contains the configuration and the interfaces that the user wants.

```json
{
    "machine": "rpi4_test",
    "brokers": {
        "boundaryscanner": {
            "addr": "localhost",
            "port": 1883,
            "interfaces": [
                {
                    "name": "%r",
                    "group_name" : "artys7",
                    "driver": "FTX232_JTAG_IO",
                    "settings": {
                        "probe_name" : "FT2232HL A FT6RR4EEA",
                        "idcode" : "0x0362f093",
                        "bsdl_library" : "/etc/panduza/data/BSDL",
                        "pin"  : "%r"
                    },
                    "repeated": [
                        "IO_E18",
                        "IO_F13",
                        "IO_E13",
                        "IO_H15",
                        "IO_J15",
                        "IO_G17",
                        "IO_F15",
                        "IO_E15",
                        "IO_F18",
                        "IO_E14",
                        "IO_G15",
                        "IO_K16",
                        "IO_J16",
                        "IO_H13",
                        "IO_H14",
                        "IO_H18",
                        "IO_G18",
                        "IO_M5"
                    ]
                },
                {
                    "driver" : "FTX232_JTAG_IO",
                    "group_name" : "STM32",
                    "name" : "%r",
                    "repeated" : [],
                    "settings" : {
                       "bsdl_library" : "/etc/panduza/data/BSDL",
                       "idcode" : "0x06435041",
                       "pin" : "%r",
                       "probe_name" : "FT2232HL A FT6RR4EEA"
                    }
                }
            ]
        }
    }
}
```

The json extracted above show an example of a json file with the boundary scan. It contains most of the information needed to run the ARTY S7 device that we will define later.

Theses lines are defined as :

- "machine" : A name to the machine that this program is running on. It will separate this machine to the other machine that could connect to the same broker.
- "brokers" : It will contains the information of the broker and the interfaces that will be connecting to this broker
- "addr" : This is the address of the MQTT broker
- "port" : This will define the port of the MQTT broker
- "interfaces" : It will contains the list of interfaces that will be loaded and connected to the MQTT broker define just above.
- "name" : name of the interface (in the case of the boundary scan, it must not change as the name is changed dynamically)
- "driver" : Identify the interface with this name at the start of the program. Each interface/plugin is compared with the driver name to be able to load them.
- "settings" : contains different settings that is needed to load a plugin.
- "repeated" : specific for the Boundary Scan. It lists the IO that will be loaded

### 3.2. Device identification

To be able to load the device, up to two parameters in the tree's setting can be defined:

- **device_no** : This parameter is giving to the program the position of the device. The device_no start at 0 for the first device from the end of the chain (so the device 0 is the one connected to the TDO of the probe)
- **idcode** : This parameter will give to the program the idcode of the device that we want to load. It must be given as an hexadecimal value for now.

To be able to run the program, when there is only one device, it need at least one of the two parameter. The two parameters can be given but the device_no is prioritized. So it will react as if the idcode is not defined.

When giving only the idcode of the device, in the case that there is two devices with the same idcode, a device number is required on the tree. Otherwise, the program will stop.

### 3.3. "group_name"

A setting new setting was creating to allow to personalise the name of an interface in the topic (pza/machine/**interface**/...).
This will help to define easier topics and personalise the topic with some specific information that the user can understand more.

By default the interface name in the topic is the concatenation of the driver name, the probe name and the device number(in the case of the boundary scan), as the driver can be the same for multiple interfaces.
To add a custom interface name in the topic, a new setting have to be added in the interface in question :

```json
"group_name" : xxx
```

This feature will be use in the example to separate each interface and make the testing easier.

### 3.4. AUTODETECT

To anticipate the implementation to the panduza web interfaces, an autodetect mode is created. When running in autodetect mode, the program will generate every interfaces that can be loaded and add them into a json file in the "platform" folder.

To run the program in autodetect mode, the AUTODETECT environment variable have to be "1". For that, write "AUTODETECT=1" before the start comment :

```sh
 AUTODETECT=1 ./start.sh
```

This will pass the variable to the docker image and then will be retrieve by the program. It will then create a file called **cxx.json** that will contains every interfaces available and prepare some part that will be able to be inserted into the tree.

## 4. Example : Daisy Chaining

### 4.1. Introduction

In this part, two devices will be connected in chain, we call this daisy-chain. The wiring and all the instructions will be presented below.

### 4.2. Hardware

For this example, three pieces of hardware is needed.

- A FT2232H chip probe. In our case we are using a custom made probe with a FT2232HL chip.
  <img src="https://user-images.githubusercontent.com/37267717/188599593-9532414c-c7d0-4548-92b6-b0a76b735fec.jpg" width="400" alt="probe"/>
- Two JTAG compatible device. For this example, we are using a DIGILENT ARTY-S7 and a Nucleo STM32L443 (or also called Nucleo-F103RB).
  <img src="https://user-images.githubusercontent.com/37267717/188600452-3a18e156-d57f-4ab8-8e3e-116c5a209357.jpg" alt="artys7" width="400"/>
  <img src="https://user-images.githubusercontent.com/37267717/197539226-f5314381-0800-44f4-a19d-73e5667b86ca.jpg" alt="artya7" width="400"/>

### 4.3. Wiring

The connection between the probe and Devices Under Test (DUT) is done with the help of the ALTERA connector on the probe and the JTAG connector on the DUTs.
The wiring is done as defined below :

<img src="https://user-images.githubusercontent.com/37267717/194314415-33d16a1b-6a08-4aa8-b2a2-8e2fdf85e110.png" height="300" style="margin:10px" /> <img src="https://user-images.githubusercontent.com/37267717/194314425-092855d2-66f2-4edc-b5a2-67ec47ea2bde.png" height="300" style="margin:10px" />

<img src="https://user-images.githubusercontent.com/37267717/197539546-71946b6c-a918-4c85-a491-eae1b1bd1359.png" width="400" style="margin:10px" />

NB: Note that the Jtag connector is the same for both Digilent devices.

<span style="color:red">WARNING : Due to some EM parasite, a breadboard is not used in this example.</span>
<span style="color:red">WARNING 2 : Due to some STM32 specific issue, please wire the two devices in the order defined in the picture above.</span>

### 4.4. Running the project

To run the project, please run the command "./start.sh"

### 4.5. Testing

## 5. Common issues

If the program exit prematurely, some error can be fix easily, shows as below.

### 5.1. "No tree file found"

Check that the tree file is present in the "**panduza**" folder. The directory from the repo base is : "./artys7-stm32/panduza" .

### 5.2 "No probe detected"

This error means that there is no probe connected to the machine.

If you are in a Virtual Machine, check that you do have linked the probe to the VM.

### 5.3. "Probe xxx : not found"

Verify that the probe name that you are using is the same as the one you have.

In the case you can't use the same probe as the one used in the example (**FT2232HL A FT6RR4EEA**), you can always change the probe name on the tree with the name of your probe.

There is two way to find your probe name.

- The first one is to find it in the BoundaryScan log available in the log folder.

<img src="https://user-images.githubusercontent.com/37267717/197531990-ef959029-825c-4822-8166-08d4003f8356.png" height="150" alt="log probe name location"/>

- The second one is to use the autodetect feature and get the probe name inside.

<img src="https://user-images.githubusercontent.com/37267717/197539948-929b45c5-8f70-4eee-8bf0-9c6e9a2b2f02.png" width="300" alt="log probe name location"/>

### 5.4 "No device detected"

If no device is detected, please Verify the JTAG Connection. The device(s) also have to be powered by its microUSB port.

### 5.5. "No BSDL File found for device no : x, it's idcode is : xxxxxxxxxx"

This means that the program couldn't locate a BSDL file that match the idcode of the device x.

In the case the BSDL file wasn't added to the folder, please add it.
In the other case, please check the Jtag connector.

## 6. Additional Information

If you want to test the device (ArtyS7 and Nucleo STM32) by yourself, here is the pinout of physical GPIO such as button, switch and LEDs.
The ArtyS7 will have on its picture **Spartan-7**
<img src="https://user-images.githubusercontent.com/37267717/197722515-4bcbdf94-367f-4061-8c28-957727274a6f.png" width="500" alt="artyS7 pinout"/>
