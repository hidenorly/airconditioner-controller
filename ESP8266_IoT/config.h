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

#ifndef __CONFIG_H__
#define __CONFIG_H__

// --- config
extern const int MODE_PIN;

// --- config: WIFI
extern const char* WIFI_CONFIG;
extern const char* WIFIAP_PASSWORD;

// --- config: NTP
extern const char* NTP_SERVER;

// --- config: httpd
extern int HTTP_SERVER_PORT;
extern const char* HTML_HEAD;

// --- GPIO initial setup
void initializeGPIO(void);

// --- config: OTA
extern const int OTA_PIN;
#define OTA_PIN_PERIOD	5000

// --- config: Air conditioner controller
extern const int POWER_DETECT_PIN;
extern const int POWER_CONTROL_PIN;
extern const int HUMAN_DETCTOR_PIN;
#include "RemoteController.h"
extern KEYGPIO KEYGPIOs[];

// --- config : Initial value for Air conditioner controller
#define HUMAN_UNDETECT_TIMEOUT 1000*60*30 // 15min
#define AIRCON_POWER_PERIOD 1500


#endif // __CONFIG_H__

