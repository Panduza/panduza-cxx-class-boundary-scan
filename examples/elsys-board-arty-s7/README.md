# ELSYS BOARD ARTY S7 EXAMPLE

This example will be based on the docker image of panduza-cxx-platform that will contain the plugin.

## HARDWARE USED

This projet use a probe and a DUT (Device Under Test)

- The probe developped internally in ELSYS DESIGN which is a USB-JTAG probe that is based on the FT2232HL chip.

<img src="https://user-images.githubusercontent.com/37267717/188599593-9532414c-c7d0-4548-92b6-b0a76b735fec.jpg" width="300" alt="probe"/>

- The DUT is a ARTY S7.

<img src="https://user-images.githubusercontent.com/37267717/188600452-3a18e156-d57f-4ab8-8e3e-116c5a209357.jpg" alt="artys7" width="400"/>

## JTAG CONNECTION PINOUT

To allow the probe to connect to the DUT, the connection is made via JTAG.
The connection is made as below :

<img src="https://user-images.githubusercontent.com/37267717/194314415-33d16a1b-6a08-4aa8-b2a2-8e2fdf85e110.png" height="300" style="margin:10px" /> <img src="https://user-images.githubusercontent.com/37267717/194314425-092855d2-66f2-4edc-b5a2-67ec47ea2bde.png" height="300" style="margin:10px" />
<img src="https://user-images.githubusercontent.com/37267717/194315033-b5991b4f-36b8-4af4-afc0-53c06ae3b6df.png" height="400" style="margin:10px" />

## FT2232HL Probe name

There is two way of defining the probe name. One is to change directly the name of the probe (EEPROM needed in the probe board) to be the same as the one defined on the tree. This can be made with the Software FT_Prog. This software is available on Windows.
For this, the Serial number should be "FT6RR4EE" and the Product Description should be "FT2232HL".
This should be editable with the software FT_Prog.

The other way is to find the name of the probe use and redefine it on the tree. This can be found either on the software FT_Prog or with the command usb-devices on ubuntu.

The probe name should be "*Product* A *SerialNumber*A" with Product and SerialNumber to change. Both "A" and spaces have to be put in the same position.

## Starting the program

Please first verify that you put the same name in the probe name than the name of your probe.

When starting the program from here, the tree used will be the tree.json present in the panduza folder.

The BSDL file is loaded by default from the BoundaryScan folder if the specific file is added (in our case it is).
This BSDL file ( xc7s50_csga324.bsdl )is only working with an ARTY s7 device.

To start the program, please launch the command below from this folder

```sh
 ./start.sh
```

After the start of the platform and the loading of the pins, it is possible to change the direction and the state of a pin with MQTT-Explorer with the specific topics and messages.

## Changing the BSDL file

An option to change the BSDL File name is to change the file name in the tree and add the file in the folder called BoundaryScan. This will allow the program to load directly the bsdl file.

When changing the BSDL file, and DUT, the pin listed on "repeated" on the tree have to be changed for the pin you want to test.

## AUTODETECT

This version of the program contains an autodetection feature. This will create a file that will create a json file with every interfaces that can be loaded.
To be able to use the autodetection, start the program with this command :

```sh
AUTODETECT=1 ./start.sh
```

This will create a flag that will be read by the program and will only run the autodetect and then will stop the program.
The template file will be available in the platform directory.
