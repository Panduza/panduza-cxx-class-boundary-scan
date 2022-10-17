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

rmmod ftdi_sioccc
rmmod usbserial

./panduza-cxx-platform

# mkdir /etc/panduza
# cp /home/builder/panduza-cxx-class-boundary-scan/examples/elsys-board-arty-s7/panduza/tree.json /etc/panduza
