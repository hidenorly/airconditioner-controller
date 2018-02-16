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
#include "AirConConfig.h"
#include <FS.h>

void AirConConfig::savePowerControlConfig(String human_timeout, String poweron_period)
{
  if( human_timeout!="" && poweron_period!="" ) {
    if ( SPIFFS.exists(CONFIG_FILE) ) {
      SPIFFS.remove(CONFIG_FILE);
    }

    File f = SPIFFS.open(CONFIG_FILE, "w");
    f.println(human_timeout);
    f.println(poweron_period);
    f.close();
  }
}


void AirConConfig::loadPowerControlConfig(int& humanTimeout, int& powerOnPeriod)
{
  File f = SPIFFS.open(CONFIG_FILE, "r");
  if ( SPIFFS.exists(CONFIG_FILE) ) {
    String humanTimeOutStr = f.readStringUntil('\n');
    humanTimeOutStr.trim();
    humanTimeout = humanTimeOutStr.toInt();
    DEBUG_PRINT("humanTimeout:");
    DEBUG_PRINTLN(String(humanTimeout));

    String powerOnPeriodStr = f.readStringUntil('\n');
    powerOnPeriodStr.trim();
    powerOnPeriod = powerOnPeriodStr.toInt();
    DEBUG_PRINT("powerOnPeriod:");
    DEBUG_PRINTLN(String(powerOnPeriod));
    f.close();
  }
}
