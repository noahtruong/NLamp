#include <Arduino.h>
#include <FastLED.h>
#include <pixeltypes.h>
#include <BluetoothSerial.h>

#define SERVICE_UUID "70e6c00e-d075-4f09-ae77-c952359aaff3"
#define CHARACTERISTIC_UUID "92841757-ad57-438d-8d05-dbbf3072a6ca"

#define LED_PIN 14
#define NUM_LEDS 66
#define BRIGHTNESS 255
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

#define OFF "0"
#define ON "1"
#define RANDOM "2"
#define CUSTOM "3"

CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType currentBlending;
BluetoothSerial SerialBT;
uint8_t brightness = 64;
String state = ON;
int r = 0;
int g = 0;
int b = 0;
String sR = "";
String sG = "";
String sB = "";

int buttIndex = 2;

int patternIndex = 0;

bool button = true;

void FillLEDsFromPaletteColors(uint8_t colorIndex){
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

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}
extern const TProgmemRGBPalette16 orange FL_PROGMEM =
{
    CRGB::Orange,
    CRGB::Orange,
    CRGB::Orange,
    CRGB::Orange,

    CRGB::Orange,
    CRGB::Orange,
    CRGB::Orange,
    CRGB::Orange,

    CRGB::Orange,
    CRGB::Orange,
    CRGB::Orange,
    CRGB::Orange,

    CRGB::Orange,
    CRGB::Orange,
    CRGB::Orange,
    CRGB::Orange,
};
// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.
/*
void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
    }
}
*/
void setup() {

    Serial.begin(115200);
    while(!Serial);
    Serial.println("serial ready");
    SerialBT.begin("ESP32");

    pinMode(23,INPUT_PULLUP);
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}


void loop()
{
  /*
    if(SerialBT.readString() == OFF)
    {
      brightness = 0;
      Serial.println("off");
    }
    if(SerialBT.readString() == ON)
    {
      brightness = 255;
      Serial.println("on");
    }
    if(SerialBT.readString() == RANDOM)
    {
      SetupTotallyRandomPalette();
      Serial.println("random");
    }
    if(SerialBT.readString() == CUSTOM)
    {
      sR = SerialBT.readStringUntil('\n');
      sG = SerialBT.readStringUntil('\n');
      sB = SerialBT.readStringUntil('\n');
      r = (int) sR.toInt();
      g = (int) sG.toInt();
      b = (int) sB.toInt();
      Serial.println(r);
      Serial.println(g);
      Serial.println(b);
      for( int i = 0; i < 16; i++) 
      {
        currentPalette[i] = CRGB(r,g,b);
      }
    }
*/
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    //ChangePalettePeriodically();

    FillLEDsFromPaletteColors(startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);

    if(digitalRead(23) == LOW)
    { 
      buttIndex += 1;
      delay(500);
      if(buttIndex > 4)
      {
        buttIndex = 0;
      }
    }

    if(buttIndex == 0){brightness = 0;}
    if(buttIndex == 1){brightness = 255;}
    if(buttIndex == 2){currentPalette = RainbowColors_p;}
    if(buttIndex == 3){currentPalette = orange;}
    if(buttIndex == 4){currentPalette = OceanColors_p;}

    
}

// Additional notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes.
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.
