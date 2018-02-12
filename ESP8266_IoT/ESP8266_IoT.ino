/* 
 Copyright (C) 2016, 2018 hidenorly

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

extern "C" {
#include "user_interface.h"
}

#include "base.h"
#include "config.h"

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include "WiFiUtil.h"
#include "WebConfig.h"
#include "NtpUtil.h"
#include "LooperThreadTicker.h"
#include "GpioDetector.h"
#include "RemoteController.h"

#include <FS.h>
#include <Time.h>
#include <TimeLib.h>
#include <NTP.h>

#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)

// --- mode changer
bool initializeProperMode(){
  if( (digitalRead(MODE_PIN) == 0) || (!SPIFFS.exists(WIFI_CONFIG)) ){
    // setup because WiFi AP mode is specified or WIFI_CONFIG is not found.
    setupWiFiAP();
    setup_httpd();
    return false;
  } else {
    setupWiFiClient();
    setup_httpd();  // comment this out if you don't need to have httpd on WiFi client mode
  }
  return true;
}

// --- handler for WiFi client enabled
void onWiFiClientConnected(){
  DEBUG_PRINTLN("WiFi connected.");
  DEBUG_PRINT("IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());
  start_NTP(); // socket related is need to be executed in main loop.
}

class Poller:public LooperThreadTicker
{
  public:
    Poller(int dutyMSec=0):LooperThreadTicker(NULL, NULL, dutyMSec)
    {
    }

    virtual void doCallback(void)
    {
      char s[30];
      time_t n = now();
      sprintf(s, "%04d-%02d-%02d %02d:%02d:%02d", year(n), month(n), day(n), hour(n), minute(n), second(n));
    
      DEBUG_PRINT("UTC : ");
      DEBUG_PRINTLN(s);
  }
};

// --- General setup() function
void setup() {
  // Initialize GPIO
  initializeGPIO();
  
  // Initialize SerialPort
  Serial.begin(115200);

  // Initialize SPI File System
  SPIFFS.begin();

  // Check mode
  delay(1000);
  if(initializeProperMode()){
    static Poller* sPoll=new Poller(1000);
    g_LooperThreadManager.add(sPoll);
  }
}

#define HUMAN_UNDETECT_TIMEOUT 1000*60*15 // 15min

void handleAirConditionerControl(void)
{
  static GpioDetector powerStatus(POWER_DETECT_PIN, false, 3000);
  static GpioDetector humanDetector(HUMAN_DETCTOR_PIN, true, 1000);
  static GpioRemoteController remoteController(KEYGPIOs);

  powerStatus.update();
  humanDetector.update();

  if( powerStatus.getStatus() ){
    // power is On!
    static bool lastHumanStatus = true;
    static unsigned long lastHumanDetected = 0;
    unsigned long n = millis();
    bool curStatus = humanDetector.getStatus();

    if(curStatus){
      lastHumanDetected = n;
      lastHumanStatus = curStatus; // true (human detected)
    }

    if( lastHumanStatus!=curStatus ){
      if( !curStatus ){
        if( (n - lastHumanDetected) > HUMAN_UNDETECT_TIMEOUT ){
          DEBUG_PRINTLN("Human is absent but aircon is on then send power key");
          lastHumanStatus = curStatus; // false (human undetected)
          remoteController.sendKey(IRemoteController::KEY_POWER);
        }
      }
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  handleWiFiClientStatus();
  handleWebServer();
  handleAirConditionerControl();
  g_LooperThreadManager.handleLooperThread();
}
