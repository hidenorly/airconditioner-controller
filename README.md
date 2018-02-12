# ESP8266_AirConfitionerController

People often foget to turn off air conditioner although you are not in the room.
This solves the problem by detecting air conditioner's power status, human detection, power control by remote controller.

This is WiFi enabled Remote Controller for Air Conditioner based on ESP8266(W-ROOM02), Universal A/C Remote (K-1028E) and optical sensor.

System is consisted of

* ESP8266 for main control
* Universal A/C Remote (K-1028E) (Modification required) for power control
* PIR sensor for human detection
* Optical sensor for power status detection

TODO:
* Add MQTT support to control/get status over WiFi.
* Configure time out value over WiFi
(Initial version's timeout is 15 minutes.)

Please note that Air Conditioner power is needed to turn on with K-1028E.

# Dependencies

## General steps to include zip library

On Arduino IDE,

1. Sketch
2. Include library
3. Install Zip library
4. Specify the following zip-ed libraries.

## Time library (by PaulStoffregen-san)

```
$ git clone https://github.com/PaulStoffregen/Time.git Time
$ cd Time
$ git archive HEAD --output=../Time.zip
```

## NTP library (by exabugs-san)

```
$ git clone https://github.com/exabugs/sketchLibraryNTP NTP
$ cd NTP
$ git archive HEAD --output=../NTP.zip
```

# Schematics

| GPIO | Description |
| ---: | :--- |
| RST | External 10k pull up. Push switch to GND for reset. And connect to GPIO16 for wake up by GPIO16 from suspending. |
| EN | External 10k pull up. |
| GPIO0 | External 10k pull up. Push switch to GND for boot mode |
| GPIO2 | External 10k pull up. I2C SDA for future.|
| GPIO15 | External 10k pull down. |
| GPIO4 | Connecting to optical detector. Low active |
| GPIO5 | Through 1.2k registor connecting to 2SC3904's base. <br> The corrector is 6.8 pull up and connecting to K-1028E's power button pattern - 1. <br> The emitter is connecting to K-1028E's power button pattern - 2. |
| GPIO12 | N/A. |
| GPIO13 | N/A. |
| GPIO14 | N/A. I2C SCL for future. |
| ADC | Through 4.7k connecting to PIR Sensor out and 4.7K external pull down. Please note that ADC's range is 0-1.0V. And PIR's output is approx. 1.9V as tester. |

```Config.cpp
// --- config: Air conditioner controller
const int POWER_DETECT_PIN = 4;
const int POWER_CONTROL_PIN = 5;
```
