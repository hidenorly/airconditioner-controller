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

class AirConPowerControlPoller:public LooperThreadTicker
{
  public:
    AirConPowerControlPoller(int dutyMSec=0);
    virtual ~AirConPowerControlPoller();

    virtual void doCallback(void);
};

#endif /* __AIRCON_POWER_CONTROL_POLLER_H__ */
