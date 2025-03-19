#include <Arduino.h>
#include <FastLED.h>
#include "LEDManager.h"

LEDManager::LEDManager(){
    FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
    FastLED.setBrightness(25);
    FastLED.show();
}


void LEDManager::setLEDColor(int R, int G, int B){
    leds[0] = CRGB(R, G, B);
    
    FastLED.show();
}
