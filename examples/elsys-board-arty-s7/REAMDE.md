# ELSYS BOARD ARTY S7 EXAMPLE

## FOLDER PLACEMENT

To allow the projet to work, panduza-cxx-platform and panduza-cxx-class-boundary-scan have to be placed of a same folder.

folder
  |--- panduza-cxx-platform
  |--- panduza-cxx-class-boundary-scan

## HARDWARE USED

This projet use a probe and a board to test

- The probe developped internally in ELSYS DESIGN which is a USB-JTAG probe that is based on the FT2232HL chip.

<img src="img/probe.jpg" alt="probe" width="400"/>

- The board to test is a ARTY S7.

<img src="img/artys7.jpg" alt="artys7" width="400"/>

## JTAG CONNECTION PINOUT

To allow the probe to connect to the board to test, the connection is made via JTAG.
The connection is made as below (take the direction of the board the picture of the hardware used):
<img src="img/usb-jtag-schema.png" width="200">  <img src="img/link-probe-board.png" width="150">  <img src="img/jtag-artys7-schema.png" width="100">


## FT2232HL Device name

As defined in the tree.json (panduza/tree.json), the probe name need to be "FT2232HL A FT6RR4EEA".
For this, the Serial number should be "FT6RR4EE" and the Product Description should be "FT2232HL".
This should be editable with the software FT_Prog.

## Starting the program

With theses conditions met and the conditions desccribed in the readme from the platform,
the example should work by launching with docker-compose.

The command to launch from this folder is : 

```sh
docker-compose build
docker-compose up
```
