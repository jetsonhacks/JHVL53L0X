#!/bin/bash
# Install prerequisites for JHVL53L0X library
sudo apt-get install libi2c-dev i2c-tools -y
# Setup permissions to run i2c in user space
sudo bash -c 'echo KERNEL==\"i2c-[0-7]\",MODE=\"0666\" > /etc/udev/rules.d/90-i2c.rules'
sudo udevadm control --reload-rules
sudo udevadm trigger

