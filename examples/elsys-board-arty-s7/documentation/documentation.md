# Documentation

- [Documentation](#documentation)
  - [1. Introduction](#1-introduction)
  - [2. First Comments and Requirements](#2-first-comments-and-requirements)
  - [3. Examples](#3-examples)
    - [3.1. Testing with one device](#31-testing-with-one-device)
      - [3.1.1. Introduction](#311-introduction)
      - [3.1.2. Installation](#312-installation)
      - [3.1.3. Running the project](#313-running-the-project)
      - [3.1.4. Testing](#314-testing)
    - [3.3. Daisy-chaining, testing with two devices](#33-daisy-chaining-testing-with-two-devices)
  - [4 Common issues](#4-common-issues)
    - [4.1 "No probe detected"](#41-no-probe-detected)
    - [4.3 "No device detected"](#43-no-device-detected)

## 1. Introduction

This file will define the different situation the program was developed for.

For each situation, the hardware used, the pinning, the test and the file needed will be defined on their own subpart.
Folder will also be available to share some program that was used to be able to run the project.

## 2. First Comments and Requirements

Do be able to start the program, a few things will be needed.

- The whole project was developed and tested on **Ubuntu**, which allow an easiest installation of Docker and some dependencies.

- You will need to install Docker and docker-compose, otherwise, it won't work. To start the program, we are pulling images from the platform repo and mosquitto, which need theses libraries. You can install [Docker here](https://docs.docker.com/engine/install/ubuntu/) and [docker-compose here (French link)](https://doc.ubuntu-fr.org/docker-compose).

- One way to send commands to the program was with a software called : **MQTT Explorer**. This will be used in the testing to show some commands and will allows you to test and play a bit with the program.
To install the software please [click here](http://mqtt-explorer.com/).
Other way of testing with python will be defined but they will be presented later.

## 3. Examples

### 3.1. Testing with one device

#### 3.1.1. Introduction

On this part, the test was done with one Device Under Test (DUT). It was the first objectives of this project.
This part will show the wiring with one specific device and describe how to start the project, and some command that will show the program working.

#### 3.1.2. Installation



#### 3.1.3. Running the project

#### 3.1.4. Testing

### 3.3. Daisy-chaining, testing with two devices

## 4 Common issues

### 4.1 "No probe detected"

### 4.3 "No device detected"
