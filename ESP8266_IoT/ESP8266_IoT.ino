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
#include "OtaManager.h"

#include <FS.h>
#include <Time.h>
#include <TimeLib.h>
#include <NTP.h>

#define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)

// --- UPnP Device
#include "Ssdp.h"
#include "UPnPDeviceWiFiSwitchAirCon.h"

static Ssdp* g_pSsdp=NULL;
static UPnPDevice* g_pSwitch1=NULL;

// --- Aircon
#include "GpioDetector.h"
#include "RemoteController.h"
#include "AirConPowerControl.h"
#include "AirConPowerControlPoller.h"
#include "AirConConfig.h"

static AirConPowerControl* g_pAirPowerControl=NULL;


// --- mode changer
bool initializeProperMode(bool bSPIFFS){
  if( !bSPIFFS || (digitalRead(MODE_PIN) == 0) || (!SPIFFS.exists(WIFI_CONFIG))){
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

  if(g_pSsdp){
    if(!g_pSwitch1){
      g_pSwitch1 = new UPnPDeviceWiFiSwitchAirCon(g_pAirPowerControl, "aircon", 31415, "Belkin:device:**", "38323636-4558-4dda-9188-cda0e6aabbcc", "/setup.xml");
    }
    if(g_pSwitch1){
      g_pSwitch1->enable(true);
      g_pSsdp->addUPnPDevice(g_pSwitch1);
    }
    g_pSsdp->enable(true);
  }
}

GpioDetector g_powerStatus(POWER_DETECT_PIN, false, 3000);
GpioDetector g_humanDetector(HUMAN_DETCTOR_PIN, true, 1000);

void setupAircon(void)
{

  int humanTimeout = HUMAN_UNDETECT_TIMEOUT;
  int powerOnPeriod = AIRCON_POWER_PERIOD;
  AirConConfig::loadPowerControlConfig(humanTimeout, powerOnPeriod);

  if(!g_pAirPowerControl){
    static GpioRemoteController remoteController(KEYGPIOs);
    g_pAirPowerControl = new AirConPowerControl(&remoteController, &g_powerStatus, powerOnPeriod);
  }
  AirConPowerControlPoller* pAirPowerControllerPoller = new AirConPowerControlPoller(g_pAirPowerControl, &g_powerStatus, &g_humanDetector, humanTimeout, 1000);
  if(pAirPowerControllerPoller){
    g_LooperThreadManager.add(pAirPowerControllerPoller);
  }
}


// --- General setup() function
void setup() {
  // Initialize GPIO
  initializeGPIO();
  
  // Initialize SerialPort
  Serial.begin(115200);

  // Initialize SPI File System
  bool bSPIFFS = SPIFFS.begin();
  if(!bSPIFFS){
    SPIFFS.format();
  }

  // Check mode
  delay(1000);
  if(initializeProperMode(bSPIFFS)){
    g_pSsdp = new Ssdp();
    setupAircon();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // WiFi
  handleWiFiClientStatus();
  handleWebServer();

  // OTA
  static OtaManager ota(OTA_PIN, OTA_PIN_PERIOD);
  ota.handle();

  // UPnP
  if(g_pSsdp && g_pSsdp->getEnabled()){
    g_pSsdp->handle();
  }
  if(g_pSwitch1 && g_pSwitch1->getEnabled()){
    g_pSwitch1->handle();
  }

  // Poller
  g_LooperThreadManager.handleLooperThread();
}
