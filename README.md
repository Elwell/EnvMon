This repo was originally forked from the Pawsey EnvMon tools, but has since been expanded to cover
a range of environment sensors, loosely based on MQTT publish/subscribe

Publishers / Sensors
--------------------
I'm using Wemos (ESP8266 nodemcu - see http://wemos.cc/ for details) based modules for convenience 
(they already have microUSB and voltage regulators attached) together with DHT22 shields for temp/humidity.
Please note that you'll need to include mqtt support when you build the firmware, you'll also need luatool
and esptool to upload files and flash new firmware.

Not included here is my init.lua, as it contains the wifi connection info. A sanitized one is pasted below:
```lua
-- Basic Connection in init.lua, anything more complex called later
-- AE 2016-06-08

print("Connecting to WiFi from init")
wifi.setmode(wifi.STATION)
wifi.sta.config("ESSID HERE","PASSWORD HERE")
--wifi.sta.connect()
tmr.alarm(1, 1000, 1, function()
    if wifi.sta.getip()== nil then
        print("IP unavaiable, Waiting...")
    else
        tmr.stop(1)
        print("ChipID is: " .. node.chipid())
        print("The module MAC address is: " .. wifi.ap.getmac())
        print("Config done, IP is "..wifi.sta.getip())
        dofile('wemos_dht.lua')
    end
end)
```


Broker
------
Mosquitto (see http://mosquitto.org)

Subscribers
-----------
subcurses.py -- Python mosquitto client that displays the topic and value in a basic curses interface

websockets gauges
