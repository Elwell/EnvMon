This repo contains scripts for the Pawsey Environmental Monitoring system

It is based on a series of Raspberry Pi's (one per cell) which publish their information to a
central MQTT (mosquitto) broker. Various subscribers can then display information as needed.

Publishers
----------
Each Raspberry Pi has the following hardware:
* 1 DHT-22, which is polled using a GPIO pin and powered from the 3.3v bus.
* 1 SheepWalk RPI2 i2c->1-wire bus adaptor
* N DS18B20 temperature sensors positioned around the equipment

Basic setup on the R-Pi includes a Debian installation on the SD card,
with NTP and static IP addressing. Broker addresses need DNS (flexibility vs relying on an external service)


Broker
------
Mosquitto running on a Virtual Machine on the iVEC infrastructure.

Subscribers
-----------
subcurses.py -- Python mosquitto client that displays the topic and value in a basic curses interface
