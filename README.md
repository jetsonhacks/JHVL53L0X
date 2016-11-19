# JHVL53L0X
VL53L0X Time of Flight Sensor library for Jetson Development Kits

This repository contains a library to interface with the Pololu VL53L0X carrier board and a couple of examples. The library is based on code located in:

Pololu Arduino Library

https://github.com/pololu/vl53l0x-arduino

and:

morikentiger vl53l0x_driver

https://github.com/morikentiger/vl53l0x_driver

PREREQUISITE

libi2c is needed. These can be installed by:

$ sudo apt-get install libi2c-dev i2c-tools -y

I2C needs permissions to run in user space. Defining a udev rule can override this:

$ sudo bash -c 'echo KERNEL==\"i2c-[0-7]\",MODE=\"0666\" > /etc/udev/rules.d/90-i2c.rules'

A convenience file is provided for the above two steps:

$ ./installPre.sh

Two examples are provided. The first example is in the ./example folder. In order to run the example:

$ cd example
$ make
$ ./example

The second example is located in ./example/LidarPlotExample

LidarPlotExample.pro is a QT5 project file which may be used to build the second example.





