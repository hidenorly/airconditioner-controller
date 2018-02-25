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

#ifndef __AIRCON_POWER_CONTROL_POLLER_H__
#define __AIRCON_POWER_CONTROL_POLLER_H__

#include "base.h"
#include "LooperThreadTicker.h"
#include "AirConPowerControl.h"
#include "GpioDetector.h"
#include "config.h"

class AirConPowerControlPoller:public LooperThreadTicker
{
  public:
    AirConPowerControlPoller(AirConPowerControl* pPowerControl=NULL, GpioDetector* pPowerStatus=NULL, GpioDetector* pHumanDetector=NULL, int humanTimeout=HUMAN_UNDETECT_TIMEOUT, int dutyMSec=0);
    virtual ~AirConPowerControlPoller();

    virtual void doCallback(void);

protected:
	AirConPowerControl* mpPowerControl;
	GpioDetector* mpPowerStatus;
	GpioDetector* mpHumanDetector;
	int mHumanTimeout;
};

#endif /* __AIRCON_POWER_CONTROL_POLLER_H__ */
