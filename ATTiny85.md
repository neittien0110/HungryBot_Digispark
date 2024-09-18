# Hungry Bot Recycle Bin

## Overview

- This source code used for [Digispark Pro](https://www.kickstarter.com/projects/digistump/digispark-pro-tiny-arduino-ready-mobile-and-usb-de)\
  ![Alt text](./images/attiny85.png)\
  [Xem hướng dẫn](https://neittien0110.github.io/MCU/ATTiny/Digispark_Kickstarter_ATTiny85.html)

_Chú ý: không cắm PIN 3 (USB D+) khi nạp code._

## Bill of Material

|---|---|---|
|Name|Qty|Mapping|
|---|---|---|
|Digispark Default | 1 (*) ||
|Ultrasonic Distance Sensor SR-04 | 1 (*) |ECHO = PIN 0 , TRIG = PIN 1|
|Servo SG90s | 1 (*) | PIN_SERVOHAND = 3|
|Led 5mm | 1 |  PIN_LED = 1|
|Light resistor | 1 | 220 Ohm|

- [3D printed parts](https://www.thingiverse.com/thing:5160440)

*: mandatory

  ![Digispark (Default 16 mhz)](./images/attiny85_board_in_arduinoide.png)

## HungryBotShield

### Board cắm tự nối dây

![HungryBotShield Manual PCB](./images/manual_shield.png)

### Board mạch in

![HungryBotShield Schematic](./images/pcb_shield-2.png)
![HungryBotShield PCB](./images/pcb_shield-3.png)

## Video

- [Demo](https://youtu.be/PHz48JWzXpA)
