#include <FastLED.h>

#include "MusicData.h"
#include "PushThrough.h"
#include "Changer.h"
#include "Rainbow.h"
#include "SimplePulse.h"
#include "MixBar.h"
#include "Quadrant.h"
#include "SplitSpiral.h"
#include "BassPush.h"

// Arduino Music Visualizer 0.3

// This music visualizer works off of analog input from a 3.5mm headphone jack
// Just touch jumper wire from A0 to tip of 3.5mm headphone jack

// The code is dynamic and can handle variable amounts of LEDs
// as long as you adjust NUM_LEDS according to the amount of LEDs you are using

// This code uses the Sparkfun Spectrum Shield

// LED LIGHTING SETUP
#define LED_PIN     7
#define NUM_LEDS    1097
#define BRIGHTNESS  64
#define LED_TYPE    NEOPIXEL
#define COLOR_ORDER GRB
CRGB realleds[NUM_LEDS];
CRGBSet leds(realleds, NUM_LEDS);


DEFINE_GRADIENT_PALETTE( PAL_HEATMAP_TEST ) {
  0,     0,  0,  0,   //black
128,   255,  0,  0,   //red
224,   255,255,  0,   //bright yellow
255,   255,255,255 }; //full white

DEFINE_GRADIENT_PALETTE( PAL_QUAD_TEST ) {
  0,   255,  0,  0,   // red
 62,   255,  0,  0,   // red
125,     0,255,  0,   // green
188,     0,  0,255,   // blue
251,   255,255,  0,   // yellow
255,   255,255,  0, };// yellow

DEFINE_GRADIENT_PALETTE( PAL_HALLOWEEN_GEN ) {
  0,    0,    0,    0,
  32,    0,    21,   140, // midnight blue
  128,   0,    196,  84,  // emerald green
  170,  232,  104,  0,   // pumpkin orange
  230,  232,  104,  0,   // pumpkin orange
  255,  255,  255,  255, // full white
};

DEFINE_GRADIENT_PALETTE( PAL_HALLOWEEN_PUMPKIN ) {
  0,    0,    0,    0,  // black
  16,   100,  25,   25, // dark red
  96,   230,  10,   10, // blood red
  120,  255,  150,  40, // pumpkin orange
  245,  255,  255,  40, // candle yellow
  255,  255,  255,  255, // pure white
};

DEFINE_GRADIENT_PALETTE( PAL_HALLOWEEN_GHOUL ) {
  0,    0,    0,    0,    // black
  16,   90,   0,    120,  // dark purple
  64,   160,  30,   160,  // light purple
  200,  0,    150,  40,   // green
  255,  255,  255,  255   // pure white
};

DEFINE_GRADIENT_PALETTE( PAL_POINTY_PARTY ) {
  0,    0,    0,    0, // black
  16,   150,  44,   44, // Dark Rose
  180,  208,  100,  100, // Old Rose
  210,  220,  220,  220, // Grey
  255,  255,  255,  255, // full while
};

DEFINE_GRADIENT_PALETTE( PAL_RAINBOW ) {
  10,   209,  0,    0,    // r
  50,   255,  102,  32,   // o
  100,   255,  213,  33,   // y
  150,   51,   221,  0,    // g
  200,   17,   51,   204,  // b
  225,   34,   0,    102,  // i
  255,   51,   0,    68,   // v
};

DEFINE_GRADIENT_PALETTE( PAL_MAGMA ) {
	0,		48,		11,		102,		// Deep Violet
	50,		132,	32,		130,		// Maximum Purple
	125,	199,	57,		116,		// Fuchsia Rose
	175,	254,	138,	92,			// Coral
	255,	252,	252,	186,		// Very Pale Yellow
};

DEFINE_GRADIENT_PALETTE( PAL_FOREST ) {
	0,		11,		102,	15,		// Deep Green
	50,		9,		155,	106,	// Green (NCS)
	125,	33,		198,	190,	// Maxmimum Blue-Green
	175,	59,		134,	191,	// Cyan-Blue Azure
	255,	164,	165,	191,	// Blue Bell
};

DEFINE_GRADIENT_PALETTE( PAL_MONOCHROME )
{
  0,    1,    1,    1,    // Black
  230,  127,  127,  127,  // Grey
  255,  255,  255,  255,    // White
};

#define NUM_PALETTES 8
CRGBPalette16 palettes[NUM_PALETTES];

// AUDIO INPUT SETUP
MusicData* data = MusicData::instance();
MusicFrame testFrame = MusicFrame(0);

// Current tick
long tick = 0;

// Time passed tracking
#define TICKS_PER_EPOCH 500

// Track epochs and preallocate random selectors.
int currentChanger;
int currentPalette;

// Preinit changer array
#define NUM_CHANGERS 10
Changer* changers[NUM_CHANGERS];

void setup()
{
  
  // LED LIGHTING SETUP
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  // CLEAR LEDS
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);
  FastLED.show();


  // CREATE CHANGER COLLECTION
//  changers[0] = new PushThrough(data.currentFrame().channels, leds, 0, NUM_LEDS, PAL_RAINBOW);

  // CREATE PALETTE COLLECTION
//  palettes[0] = PAL_MONOCHROME;
//  palettes[1] = PAL_HALLOWEEN_GEN;
//  palettes[2] = PAL_HALLOWEEN_GHOUL;
//  palettes[3] = PAL_HALLOWEEN_PUMPKIN;
//  palettes[4] = PAL_POINTY_PARTY;
//	palettes[5] = PAL_MAGMA;
//  palettes[6] = PAL_FOREST;
//  palettes[7] = PAL_RAINBOW;

  // SERIAL AND INPUT SETUP
  Serial.begin(9600);

  // RANDOM SEED
  randomSeed(analogRead(LED_PIN));

  // BEGIN LOG
  Serial.println("\nListening...");
}



// Gets the current epoch, according to the number of ticks
int getCurrentEpoch()
{
  tick++;
  return tick / TICKS_PER_EPOCH;
}

void printLED(CRGBSet set, int index)
{
  Serial.print(set[index].r);
  Serial.print(",\t");
  Serial.print(set[index].g);
  Serial.print(",\t");
  Serial.print(set[index].b);
  Serial.println();
}

void printCLED(int index)
{
  Serial.print(realleds[index].r);
  Serial.print(",\t");
  Serial.print(realleds[index].g);
  Serial.print(",\t");
  Serial.print(realleds[index].b);
  Serial.println();
}

void startNewEpoch()
{
  currentChanger = random(NUM_CHANGERS);
  currentPalette = random(NUM_PALETTES);

//  Serial.println("\nEPOCH:");
//
//  Serial.print("Changer:\t");
//  Serial.println(currentChanger);
//
//  Serial.print("Palette:\t");
//  Serial.println(currentPalette);

    Serial.print(millis());
    Serial.print(',');
    Serial.print(TICKS_PER_EPOCH);
    Serial.print(',');
    Serial.println(currentChanger);

  changers[currentChanger]->setPalette(palettes[currentPalette]);
}

void loop()
{  

  // Update channel info
  data->update();

  data->at(0).log();

  FastLED.show();

  tick++;

}
