#include "cameraManager.h"
#include "camera_index.h"   // Camera pin definitions from EleGOo
#define CAMERA_MODEL_ESP32S3_EYE
#include "camera_pins.h"    // Camera pin definitions from EleGOo
#define quality 12          // Adjust quality as needed

// initialize the server on port 80 and state variables.
CameraManager::CameraManager() 
  : server(80), clientConnected(false), lastStreamTime(0) {
}

// Initializes the camera 
void CameraManager::initCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_UXGA;
    config.pixel_format = PIXFORMAT_JPEG; // for streaming
   // config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 10;
    config.fb_count = 2;
    //init with high specs to pre-allocate larger buffers
    if(config.pixel_format == PIXFORMAT_JPEG){
      if(psramFound()){
        config.jpeg_quality = 63;
        config.fb_count = 2;
        config.grab_mode = CAMERA_GRAB_LATEST;
      } else {
        // Limit the frame size when PSRAM is not available
        config.frame_size = FRAMESIZE_SVGA;
        config.fb_location = CAMERA_FB_IN_DRAM;
      }
    } else {
      // Best option for face detection/recognition
      config.frame_size = FRAMESIZE_240X240;
  #if CONFIG_IDF_TARGET_ESP32S3
      config.fb_count = 2;
  #endif
    }

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera initialization issue: %d\n", err);
        return;
    } 

   



    sensor_t * s = esp_camera_sensor_get();
    s->set_framesize(s, config.frame_size);
    s->set_vflip(s, 1); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation

    if(config.pixel_format == PIXFORMAT_JPEG){
         s->set_framesize(s, FRAMESIZE_240X240);
       }
     


}

// Starts the WiFiServer.
void CameraManager::startCameraServer() {
    server.begin();

}


bool CameraManager::connectionStatus() {
    return clientConnected;
}


void CameraManager::handleClient() {
    // Check for a new client if there isn't an active one
    if (!client || !client.connected()) {
        if (client) {
            client.stop();
        }
        client = server.available();
        if (client) {
            Serial.println("New client connected");
            // Read available header data using a fixed size buffer
            while (client.available()) {
                char buffer[64];
                int len = client.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
                buffer[len] = '\0';
                // If the header line is blank, we reach the end
                if (len == 0 || strcmp(buffer, "\r") == 0) {
                    break;
                }
            }

            client.print("HTTP/1.1 200 OK\r\nContent-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n");
            clientConnected = true;
        }
    }

    // If a client is connected, stream frames.
    if (client && client.connected() && clientConnected) {
        if (millis() - lastStreamTime > frameInterval) {
            lastStreamTime = millis();
            sendFrame(client);
        }
    } else {
        cleanupClient();
    }
}

// Captures a frame from the camera and sends it to the client.
void CameraManager::sendFrame(WiFiClient &client) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Failed to capture frame");
        return;
    }
    // Write header
    client.printf("--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n", fb->len);
    // Write the image data to frame buffer fb
    client.write(fb->buf, fb->len);
    client.print("\r\n");  // End of frame

    esp_camera_fb_return(fb);
}

// if cilient stop
void CameraManager::cleanupClient() {
    if (client) {
        client.stop();
    }
    clientConnected = false;
}
