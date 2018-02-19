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

AirConPowerControl::AirConPowerControl(IRemoteController* pController, GpioDetector* pPowerStatus, int checkStatusPeriod, int checkStatusPollingPeriod):mpController(pController),mpPowerStatus(pPowerStatus),mCheckStatusPeriod(checkStatusPeriod),mCheckStatusPollingPeriod(checkStatusPollingPeriod)
{

}

AirConPowerControl::~AirConPowerControl()
{

}

void AirConPowerControl::setPower(bool bPower)
{
	if( mpPowerStatus && mpController ){
		while( bPower != getPowerStatus() ){
			mpController->sendKey(IRemoteController::KEY_POWER);
			for(int i=0; i<mCheckStatusPeriod; i=i+mCheckStatusPollingPeriod){
				delay(mCheckStatusPollingPeriod);
				if( bPower == getPowerStatus() ) return;
				yield();
			}
		}
	}
}

bool AirConPowerControl::getPowerStatus(void)
{
	bool result = false;

	if( mpPowerStatus ){
		mpPowerStatus->update();
		result = mpPowerStatus->getStatus();
	}

	return result;
}
