#include "WIFIManager.h"

WIFIManager::WIFIManager(int type){
    int status = WL_IDLE_STATUS;

    WiFi.mode(WIFI_STA);

    if(type == WIFI_TYPE_HOME){
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    } else if (type == WIFI_TYPE_HOTSPOT){
        WiFi.begin(WIFI_SSID_HOTSPOT, WIFI_PASSWORD_HOTSPOT);
    }
    else if (type == WIFI_TYPE_HOME_B){
        WiFi.begin(WIFI_SSID_HOME_B, WIFI_PASSWORD_B);
    }
    
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        delay(100);
    }
    Serial.println("\nConnected");
}

String WIFIManager::getIPAddress(){
    String theIPAddress = WiFi.localIP().toString();
    return theIPAddress;
}