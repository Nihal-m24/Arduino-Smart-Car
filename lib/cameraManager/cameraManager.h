#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include <WiFi.h>
#include "esp_camera.h"
#include <WiFiServer.h>

class CameraManager {
public:
    CameraManager();
    void initCamera();
    void startCameraServer();   // Starts the WiFiServer on port 80
    void handleClient();        // Call this in loop() 
    bool connectionStatus();    // Returns whether a client is connected

private:
    void sendFrame(WiFiClient &client);
    void cleanupClient();

    WiFiServer server;          // Server listening on port 80
    WiFiClient client;          // Currently connected client
    bool clientConnected;       // Whether a client is actively streaming
    unsigned long lastStreamTime; // For frame rate control
    const unsigned long frameInterval = 380; // Time between frames. 100 should be around 10fps
};

#endif
