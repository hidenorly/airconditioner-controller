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

#ifndef __AIRCON_POWER_CONTROL_H__
#define __AIRCON_POWER_CONTROL_H__

#include "base.h"

#include "RemoteController.h"
#include "GpioDetector.h"

#define CHECK_STATUS_PERIOD_MSEC 1500
#define CHECK_STATUS_POLLING_PERIOD_MSEC	100

class AirConPowerControl
{
public:
	AirConPowerControl(IRemoteController* pController, GpioDetector* pPowerStatus, int checkStatusPeriod=CHECK_STATUS_PERIOD_MSEC, int checkStatusPollingPeriod=CHECK_STATUS_POLLING_PERIOD_MSEC);
	virtual ~AirConPowerControl();
	void setPower(bool bPower);
	bool getPowerStatus(void);

protected:
	IRemoteController* mpController;
	GpioDetector* mpPowerStatus;
	int mCheckStatusPeriod;
	int mCheckStatusPollingPeriod;
};

#endif /* __AIRCON_POWER_CONTROL_H__ */
