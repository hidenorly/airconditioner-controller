/* 
 Copyright (C) 2018 hidenorly

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
#include "AirConPowerControl.h"
#include "AirConPowerControlPoller.h"
#include "GpioDetector.h"
#include "RemoteController.h"
#include "AirConConfig.h"
#include "config.h"

AirConPowerControlPoller::AirConPowerControlPoller(int dutyMSec):LooperThreadTicker(NULL, NULL, dutyMSec)
{

}

AirConPowerControlPoller::~AirConPowerControlPoller()
{

}

void AirConPowerControlPoller::doCallback(void)
{
  static GpioDetector powerStatus(POWER_DETECT_PIN, false, 3000);
  static GpioDetector humanDetector(HUMAN_DETCTOR_PIN, true, 1000);
  static GpioRemoteController remoteController(KEYGPIOs);

  static bool bInitialized = false;
  static int humanTimeout = HUMAN_UNDETECT_TIMEOUT;
  int powerOnPeriod = AIRCON_POWER_PERIOD;
  if( !bInitialized ){
    bInitialized = true;
    AirConConfig::loadPowerControlConfig(humanTimeout, powerOnPeriod);
  }
  static AirConPowerControl airconPowerControl(&remoteController, &powerStatus, powerOnPeriod);

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
        if( (n - lastHumanDetected) > humanTimeout ){
          DEBUG_PRINTLN("Human is absent but aircon is on then send power key");
          lastHumanStatus = curStatus; // false (human undetected)
          airconPowerControl.setPower(false);
        }
      }
    }
  }
}

