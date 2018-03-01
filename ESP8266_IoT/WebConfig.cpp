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

#include "base.h"
#include "config.h"
#include "WiFiUtil.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include "AirConConfig.h"

#include "GpioDetector.h"
extern GpioDetector g_powerStatus;
extern GpioDetector g_humanDetector;
#include <Time.h>
#include "TimeUtil.h"

#include "AirConPowerControl.h"
extern AirConPowerControl* g_pAirPowerControl;


// --- HTTP server related
static ESP8266WebServer* g_pHttpd = NULL; // http server for WiFi AP Mode

char* HTML_TAIL = "</body></html>";

void httpd_handleRootGet(void);
void httpd_handleRootPost(void);
void httpd_handleRootStatusGet(void);
void httpd_handleRootStatusPost(void);

void setup_httpd() {
  if( g_pHttpd == NULL ){
    g_pHttpd = new ESP8266WebServer(HTTP_SERVER_PORT);
  }
  g_pHttpd->on("/", HTTP_GET, httpd_handleRootGet);
  g_pHttpd->on("/", HTTP_POST, httpd_handleRootPost);
  g_pHttpd->on("/status", HTTP_GET, httpd_handleRootStatusGet);
  g_pHttpd->on("/status", HTTP_POST, httpd_handleRootStatusPost);
  g_pHttpd->begin();
  DEBUG_PRINTLN("HTTP server started.");
}

void httpd_handleRootGet() {
  DEBUG_PRINTLN("Receive: GET /");
  if (g_pHttpd != NULL) {
    String html = HTML_HEAD;
    html += "<h1>WiFi/Power Control Settings</h1>";
    html += "<form method='post'>";
    html += "  <input type='text' name='ssid' placeholder='ssid'><br>";
    html += "  <input type='text' name='pass' placeholder='pass'><br>";
    html += "  <input type='text' name='human_timeout' placeholder='human_timeout'><br>";
    html += "  <input type='text' name='poweron_period' placeholder='poweron_period'><br>";
    html += "  <input type='submit'><br>";
    html += "</form>";
    html += HTML_TAIL;
    g_pHttpd->send(200, "text/html", html);
  }
}

void httpd_handleRootPost() {
  DEBUG_PRINTLN("Receive: POST /");
  if( g_pHttpd != NULL ){
    String ssid = g_pHttpd->arg("ssid");
    String pass = g_pHttpd->arg("pass");
    String human_timeout = g_pHttpd->arg("human_timeout");
    String poweron_period = g_pHttpd->arg("poweron_period");
  
    // --- SSID & Password are specified.
    if( (ssid=="format") && (pass=="format") ){
      SPIFFS.format();
      String html = HTML_HEAD;
      html += "Format successful on SPIFFS!";
      html += HTML_TAIL;
      g_pHttpd->send(200, "text/html", html);
    } else {
      saveWiFiConfig(ssid, pass);
      AirConConfig::savePowerControlConfig(human_timeout,poweron_period);
    }
    
    String html = HTML_HEAD;
    html += "<h1>WiFi Settings</h1>";
    html += ((ssid!="") ? ssid : "ssid isn't changed") + "<br>";
    html += ((pass!="") ? pass : "password isn't changed") + "<br>";
    html += ((human_timeout!="") ? human_timeout : "human_timeout isn't changed") + "<br>";
    html += ((poweron_period!="") ? poweron_period : "poweron_period isn't changed") + "<br>";
    html += HTML_TAIL;

    g_pHttpd->send(200, "text/html", html);

    delay(1000*3);

    ESP.restart();
  }
}

void httpd_handleRootStatusGet() {
  DEBUG_PRINTLN("Receive: GET /status");
  if (g_pHttpd != NULL) {
    g_powerStatus.update();
    g_humanDetector.update();
    String html = HTML_HEAD;
    html += "<h1>AirCon Power status</h1>";
    html += "Now : " + TimeUtil::getDateTimeFromTime(now()) + "<br>";
    html += "AirCon power : " + String(g_powerStatus.getStatus() ? "On" : "Off") +"<br>";
    html += "Human detection status : " + String(g_humanDetector.getStatus() ? "On" : "Off") +"<br>";
    html += "<form method='post'>";
    html += "  <input type='text' name='power' placeholder='power' value='"+String(String(g_powerStatus.getStatus() ? "Off" : "On"))+"'><br>";
    html += "  <input type='submit'><br>";
    html += "</form>";
    html += HTML_TAIL;
    g_pHttpd->send(200, "text/html", html);
  }
}

void httpd_handleRootStatusPost() {
  DEBUG_PRINTLN("Receive: POST /status");
  if( g_pHttpd != NULL && g_pAirPowerControl){
    String power = g_pHttpd->arg("power");
    g_pAirPowerControl->setPower( power == "On" );
    httpd_handleRootStatusGet();
  }
}

void handleWebServer(){
  if( g_pHttpd ){
    g_pHttpd->handleClient();
  }
}
