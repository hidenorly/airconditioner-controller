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

#include "UPnPDeviceWiFiSwitchAirCon.h"

UPnPDeviceWiFiSwitchAirCon::UPnPDeviceWiFiSwitchAirCon(AirConPowerControl* pPowerControl, String deviceName, int port, String urn, String uuid, String serviceUrl):UPnPDeviceWiFiSwitch(deviceName, port, urn, uuid,serviceUrl),mpPowerController(pPowerControl)
{

}

UPnPDeviceWiFiSwitchAirCon::~UPnPDeviceWiFiSwitchAirCon()
{
	mpPowerController = NULL;
}

void UPnPDeviceWiFiSwitchAirCon::onSetSwitchState(bool bState)
{
	DEBUG_PRINT("Request state is ");
	DEBUG_PRINT(String(bState));
	DEBUG_PRINTLN("");
	mSwitchStatus = bState ? true : false;
	if(mpPowerController){
		mpPowerController->setPower(mSwitchStatus);
	}
}

bool UPnPDeviceWiFiSwitchAirCon::onGetState(void)
{
	if(mpPowerController){
		mSwitchStatus = mpPowerController->getPowerStatus();
	}
	return mSwitchStatus;
};

