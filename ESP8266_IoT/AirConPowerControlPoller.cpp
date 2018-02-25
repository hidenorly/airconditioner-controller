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
#include "AirConPowerControlPoller.h"

AirConPowerControlPoller::AirConPowerControlPoller(AirConPowerControl* pPowerControl, GpioDetector* pPowerStatus, GpioDetector* pHumanDetector, int humanTimeout, int dutyMSec):LooperThreadTicker(NULL, NULL, dutyMSec),mpPowerControl(pPowerControl),mpPowerStatus(pPowerStatus),mpHumanDetector(pHumanDetector),mHumanTimeout(humanTimeout)
{

}

AirConPowerControlPoller::~AirConPowerControlPoller()
{
  mpPowerControl = NULL;
  mpPowerStatus = NULL;
  mpHumanDetector = NULL;
}

void AirConPowerControlPoller::doCallback(void)
{
  if(mpPowerStatus && mpHumanDetector && mpPowerControl){
    mpPowerStatus->update();
    mpHumanDetector->update();

    static bool lastPowerStatus = mpPowerStatus->getStatus();
    bool curPowerStatus = mpPowerStatus->getStatus();

    if( curPowerStatus ){
      // power is On!
      static bool lastHumanStatus = true;
      static unsigned long lastHumanDetected = 0;
      unsigned long n = millis();
      bool curStatus = mpHumanDetector->getStatus();

      if(curStatus || (lastPowerStatus!=curPowerStatus)){
        lastHumanDetected = n;
        lastHumanStatus = curStatus; // true (human detected)
      }

      if( lastHumanStatus!=curStatus ){
        if( !curStatus ){
          if( (n - lastHumanDetected) > mHumanTimeout ){
            DEBUG_PRINTLN("Human is absent but aircon is on then send power key");
            lastHumanStatus = curStatus; // false (human undetected)
            mpPowerControl->setPower(false);
          }
        }
      }

      lastPowerStatus = curPowerStatus;
    }
  }
}

