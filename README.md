# ESP8266_AirConfitionerController

People often foget to turn off air conditioner although you are not in the room.
This solves the problem by detecting air conditioner's power status, human detection, power control by remote controller.

This is WiFi enabled Remote Controller for Air Conditioner based on ESP8266(W-ROOM02), Universal A/C Remote (K-1028E) and optical sensor.

And now control over Alexa is integrated.

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

# How to burn & config WiFi?

1. Build with generic ESP8266 / 4M(1M SPIFFS) / DIO / Flash:40MHz / CPU:80MHz
2. Burn though UART (115200bps) (1. Push Mode button & reset but please keep mode button for a while.)
3. Reset
4. You can see "SSID" & "initial password"
5. Connect to 4 from PC
6. Open http://192.168.4.1/ on your PC
7. Configure WiFi SSID & the password.
(You can also set timeout:3600000 and power button:5000 etc)
8. The device will be restarting automatically and connect to your WiFi AP.

If you configure wrong SSID & the password, you can enter config mode by pushing mode button immediate after reset. You need to push mode button with 1sec after deasseting the reset.

# How to use OTA?

1. Push MODE Button for more than 5 sec.
2. Launch Arduino IDE
3. You can find network port under tool/serialport

Please note that you may need to disable firewall on your Mac and restart Arduino IDE after pushing the button

# How to get current status?

1. Open http://yourDeviceIpAddress/status from your PC

You can see
* current time (synced with NTP server)
* aircon power status
* human detection status

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
