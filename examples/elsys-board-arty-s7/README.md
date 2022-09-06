# ELSYS BOARD ARTY S7 EXAMPLE

This example will be based on the docker image of panduza-cxx-platform that will contain the plugin.

## HARDWARE USED

This projet use a probe and a DUT

- The probe developped internally in ELSYS DESIGN which is a USB-JTAG probe that is based on the FT2232HL chip.

<img src="https://user-images.githubusercontent.com/37267717/188599593-9532414c-c7d0-4548-92b6-b0a76b735fec.jpg" width="300" alt="probe"/>

- The DUT is a ARTY S7.

<img src="https://user-images.githubusercontent.com/37267717/188600452-3a18e156-d57f-4ab8-8e3e-116c5a209357.jpg" alt="artys7" width="400"/>

## JTAG CONNECTION PINOUT

To allow the probe to connect to the DUT, the connection is made via JTAG.
The connection is made as below :
<br/>
<img src="https://user-images.githubusercontent.com/37267717/188600493-8225c4cc-107b-45c7-9919-f281b57ffa34.jpg" width="300" style="margin:10px" /> <img src="https://user-images.githubusercontent.com/37267717/188600479-69a1a7ef-e334-4bbb-b0b5-c415a05effef.jpg" width="300" style="margin:10px" />


## FT2232HL Device name

As defined in the tree.json (panduza/tree.json), the probe name need to be "FT2232HL A FT6RR4EEA".
For this, the Serial number should be "FT6RR4EE" and the Product Description should be "FT2232HL".
This should be editable with the software FT_Prog.

## Starting the program

Please refer to the start of the docker image of the platform for that.
