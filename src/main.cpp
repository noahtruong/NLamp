#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 5
#define NUM_LEDS 14
#define BRIGHTNESS 64
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {
  delay(3000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

void loop() {
  static uint8_t startIndex = 0;
  startIndex = startIndex +1; //motion speed

  FillLEDsFromPaletteColors(startIndex);

  FastLED.show();
  FastLED.delay(1000/UPDATES_PER_SECOND);
}
void FillLEDsFromPaletteColors(uint8_t colorIndex){
  uint8_t brightness = 255;
  for(int i = 0; i<NUM_LEDS; i++){
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
  }
}