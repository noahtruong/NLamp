#include <Arduino.h>
#include <FastLED.h>
#include <pixeltypes.h>

#define LED_PIN 34
#define NUM_LEDS 120
#define BRIGHTNESS 64
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define OFF 0
#define ON 1
#define RANDOM 2
#define CUSTOM 3

CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

int state = RANDOM;
int r = 0;
int g = 0;
int b = 0;
String sR = "";
String sG = "";
String sB = "";

void setup() {
  Serial.begin(115200);
  Serial.println("serial ready");
  delay(3000);
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}
void FillLEDsFromPaletteColors(uint8_t colorIndex){
  uint8_t brightness = 255;
  for(int i = 0; i<NUM_LEDS; i++){
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
  }
}

void randomPalette(){
  for(int i = 0; i<16;i++){
    currentPalette[i] = CHSV(random8(),255,random8());
  }
  Serial.println("random");
}

void customColor(uint8_t r, uint8_t g, uint8_t b){
  for(int i = 0; i<16; i++){
    currentPalette[i] = CRGB(r,g,b);
  }
  Serial.println("custom color");
  Serial.println(r);
  Serial.print(g);
  Serial.print(b);
}
void loop() {

  if(Serial.available() > 0){
    state = Serial.read();
  }

  if(state == OFF){
    FastLED.setBrightness(0);
  }
  else if(state == ON){
    FastLED.setBrightness(BRIGHTNESS);
  }
  else if(state == RANDOM)
  {
    FastLED.setBrightness(BRIGHTNESS);
    randomPalette();
  }
  /*
  else if(state == CUSTOM)
  {
    FastLED.setBrightness(BRIGHTNESS);
    sR = Serial.readStringUntil('\n');
    sG = Serial.readStringUntil('\n');
    sB = Serial.readStringUntil('\n');
    r = (int)sR.toInt();
    g = (int)sG.toInt();
    b = (int)sB.toInt();
    customColor(r,g,b);
  }
  

  static uint8_t startIndex = 0;
  startIndex = startIndex +1; //motion speed

  FillLEDsFromPaletteColors(startIndex);

  FastLED.show();
  FastLED.delay(1000/UPDATES_PER_SECOND);
}
*/
