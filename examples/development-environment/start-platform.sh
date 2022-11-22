#!/bin/bash

cd /panduza-cxx-platform
mkdir -p build
cd /panduza-cxx-platform/build
cmake .. && make install 
mkdir -p /usr/share/panduza-cxx/includes/paho.mqtt.c-src
mkdir -p /usr/share/panduza-cxx/includes/paho.mqtt.cpp-src

cp -R _deps/paho.mqtt.c-src /usr/share/panduza-cxx/includes
cp -R _deps/paho.mqtt.cpp-src /usr/share/panduza-cxx/includes

ldconfig

cd /home/builder/panduza-cxx-class-boundary-scan
mkdir -p build
cd /home/builder/panduza-cxx-class-boundary-scan/build
cmake .. && make install

cd /panduza-cxx-platform/build

if lsmod | grep ftdi_sio &> /dev/null ; then
    rmmod ftdi_sio
fi
if lsmod | grep usbserial &> /dev/null ; then
    rmmod usbserial
fi

./panduza-cxx-platform

/bin/bash
