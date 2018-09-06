View this project on [CADLAB.io](https://cadlab.io/project/1268). 

# WellControll

Board for controlling of well (measuring water level, flow, temperature) over RS485 or USB using ATmega32U4.

Firmware is based on [HardwareSerialRS485 by Michael Jonker](https://github.com/MichaelJonker/HardwareSerialRS485) as multi-master low layer access to shared medium with colision detection.

This board should be integrated into home RS485 network, proxy service to MQTT system is planned for usage of hi level logic service (HomeAssistant....).