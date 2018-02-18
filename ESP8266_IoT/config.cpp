/* 
 Copyright (C) 2016,2018 hidenorly

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#include "base.h"
#include "config.h"

// --- config
const int MODE_PIN = 0; // GPIO0 {Low: WiFi AP / High: WiFi Client}

// --- config: WIFI
const char* WIFI_CONFIG = "/wifi_config";
const char* WIFIAP_PASSWORD = "1234567890"; // you can see WiFi Mac Address's SSID and this is password for setup SSID/Password from web.

// --- config: NTP
const char* NTP_SERVER = "ntp.nict.jp";

// --- config: httpd
int HTTP_SERVER_PORT = 80;
const char* HTML_HEAD = "<html><head><title>hidenorly's ESP8266</title></head><body>";

// --- config: OTA
const int OTA_PIN = MODE_PIN; // GPIO0

// --- config: Air conditioner controller
const int POWER_DETECT_PIN = 4;
const int POWER_CONTROL_PIN = 5;
#include "GpioDetector.h"
const int HUMAN_DETCTOR_PIN = GpioDetector::GPIO_ADC1;

KEYGPIO KEYGPIOs[] = {
  {IRemoteController::KEY_POWER, POWER_CONTROL_PIN, true, 100},
  {NULL, 0, false, 0}
};


// --- GPIO config
void initializeGPIO(void) {
  // General config : BOOT MODE (GPIO0,2,15) related
  pinMode(0, INPUT);
  pinMode(2, INPUT);
  pinMode(15, INPUT);

  // Project related config
  pinMode(MODE_PIN, INPUT_PULLUP); // GPIO0 is for switching mode Low: WiFi AP Mode (Config) / High: WiFi Client (Normal)

  // If pin is NC, we should set {output / High} or {input / pull up enabled} on the pin.
  pinMode(4, INPUT_PULLUP);   // Optical Detector Input
  setOutputAndValue(5, LOW);  // AirConditioner power control

  setOutputAndValue(12, HIGH);
  setOutputAndValue(13, HIGH);
  setOutputAndValue(14, HIGH);
  setOutputAndValue(16, HIGH);
}

