
#include <FastLED.h>
//#include <stdint.h>
//#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Encoder.h>

#include "tuba_bell.h"
#include "led_config.h"
#include "patterns.h"
#include "palettes.h"


#define FPS 60
#define FRAME_DELAY (1000 / FPS)

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#define PIN_LEDS 6
#define PIN_ENCODER_1_A 9
#define PIN_ENCODER_1_B 10
#define PIN_ENCODER_1_SWITCH 11
#define PIN_ENCODER_2_A 3
#define PIN_ENCODER_2_B 4
#define PIN_ENCODER_2_SWITCH 5
#define PIN_TOGGLE 12
#define PIN_MIC   A3

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
Encoder knobPattern = Encoder(PIN_ENCODER_2_A, PIN_ENCODER_2_B);
Encoder knobPalette = Encoder(PIN_ENCODER_1_A, PIN_ENCODER_1_B);

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;

uint8_t gCurrentPatternNumber = 0;
uint8_t gCurrentPaletteNumber = 0;

uint8_t gStagedPatternNumber = 0;
uint8_t gStagedPaletteNumber = 0;

bool useDefaultPalette = true;
bool glitter = false;

void setup()
{
	pinMode(PIN_ENCODER_1_SWITCH, INPUT_PULLUP);
	pinMode(PIN_ENCODER_2_SWITCH, INPUT_PULLUP);
	pinMode(PIN_TOGGLE, INPUT_PULLUP);

	FastLED.addLeds<NEOPIXEL, PIN_LEDS>(leds, NUM_LEDS);
	FastLED.setCorrection(TypicalLEDStrip);
	FastLED.setBrightness(255);

	Patterns::Setup(leds, ledInfo, NUM_LEDS, PIN_MIC);

	alpha4.begin(0x70);
	delay(100);
	writeDisplay();
	updatePattern();
}

void loop()
{
	auto now = millis();
	readPattern();
	readPalette();
	readGlitter();
	readUseDefaultPalette();
	if (readSwitchToStagedPattern())
	{
		updatePattern();
	}

	renderDisplay(now);

	Patterns::PatternList[gCurrentPatternNumber].render(now, currentPalette);
	addGlitter(now);
	FastLED.show();
}

void updatePattern()
{
	gCurrentPatternNumber = gStagedPatternNumber;
	if (useDefaultPalette)
	{
		Patterns::PatternList[gCurrentPatternNumber].getPalette(currentPalette);
	}
	else {
		Palettes::PaletteList[gStagedPaletteNumber].getPalette(currentPalette);
	}
}

#pragma region Display

bool displayOn = false;
unsigned long displayOffTime = 0;
char displayBuffer[4] = { ' ',' ',' ',' ' };


void writeDisplay()
{
	alpha4.writeDigitAscii(0, displayBuffer[0]);
	alpha4.writeDigitAscii(1, displayBuffer[1], glitter);
	alpha4.writeDigitAscii(2, displayBuffer[2], !useDefaultPalette);
	alpha4.writeDigitAscii(3, displayBuffer[3]);

	alpha4.writeDisplay();

	displayOn = true;
	displayOffTime = millis() + 5000;
}

void clearDisplay()
{
	alpha4.writeDigitAscii(0, ' ');
	alpha4.writeDigitAscii(1, ' ', glitter);
	alpha4.writeDigitAscii(2, ' ', !useDefaultPalette);
	alpha4.writeDigitAscii(3, ' ');

	alpha4.writeDisplay();
	displayOn = false;
}

void renderDisplay(unsigned long now) {
	if (displayOn && now > displayOffTime)
		clearDisplay();
}

inline void copyStringToDisplayBuffer(const char *str) {
	bool endOfStringFound = false;
	for (int i = 0; i < 4; i++) {
		auto thisChar = endOfStringFound ? 0 : str[i];
		if (!endOfStringFound && thisChar == '\0')
			endOfStringFound = true;
		if (endOfStringFound)
			displayBuffer[i] = ' ';
		else
			displayBuffer[i] = thisChar;
	}
}

inline void getPatternName(uint8_t pos)
{
	copyStringToDisplayBuffer(Patterns::PatternList[pos].getName());
}

inline void getPaletteName(uint8_t pos)
{
	copyStringToDisplayBuffer(Palettes::PaletteList[pos].getName());
}

#pragma endregion

#pragma region PaletteKnob

bool pressed = false;

inline void readUseDefaultPalette()
{
	if (!pressed && digitalRead(PIN_ENCODER_1_SWITCH) == LOW)
	{
		pressed = true;
		useDefaultPalette = !useDefaultPalette;
		writeDisplay();
	}
	else if (pressed && digitalRead(PIN_ENCODER_1_SWITCH) == HIGH)
	{
		pressed = false;
	}
}

int old_palette_pos;

void readPalette()
{
	int movement = knobPalette.read();

	if (old_palette_pos != movement)
	{
		if (movement % 4 == 0)
		{
			knobPalette.write(0);
			int steps = movement / 4;
			if (steps > 0)
				gStagedPaletteNumber = (gStagedPaletteNumber + steps) % ARRAY_SIZE(Palettes::PaletteList);
			if (steps < 0)
			{
				gStagedPaletteNumber = (ARRAY_SIZE(Palettes::PaletteList) - 1) - (((ARRAY_SIZE(Palettes::PaletteList) - 1) - gStagedPaletteNumber - steps) % ARRAY_SIZE(Palettes::PaletteList));
			}

			getPaletteName(gStagedPaletteNumber);
			writeDisplay();
		}
		old_palette_pos = movement;
	}
}

#pragma endregion

#pragma region PatternKnob
bool readSwitchToStagedPattern() {
	return digitalRead(PIN_ENCODER_2_SWITCH) == LOW;
}

int old_pattern_pos;

void readPattern()
{
	auto movement = knobPattern.read();

	if (old_pattern_pos != movement)
	{
		if (movement % 4 == 0)
		{
			knobPattern.write(0);
			int steps = movement / 4;
			if (steps > 0)
				gStagedPatternNumber = (gStagedPatternNumber + steps) % ARRAY_SIZE(Patterns::PatternList);
			if (steps < 0)
			{
				gStagedPatternNumber = (ARRAY_SIZE(Patterns::PatternList) - 1) - (((ARRAY_SIZE(Patterns::PatternList) - 1) - gStagedPatternNumber - steps) % ARRAY_SIZE(Patterns::PatternList));
			}

			getPatternName(gStagedPatternNumber);
			writeDisplay();
		}
		old_pattern_pos = movement;
	}
}
#pragma endregion

#pragma region Glitter

void readGlitter()
{
	bool newGlitter = digitalRead(PIN_TOGGLE) == HIGH;

	if (glitter != newGlitter)
	{
		glitter = newGlitter;
		writeDisplay();
	}
}

#define GLITTER_THROWAWAYS 12
#define GLITTER_PIXELS (NUM_LEDS / 20)

uint8_t throwaways = 0;

unsigned long nextFrame = 0;

void addGlitter(unsigned long now)
{
	if (!glitter)
		return;
	int offset = now % FRAME_DELAY;
  auto seed = now - offset;

  if (seed >= nextFrame) {
    nextFrame = seed + FRAME_DELAY;
    throwaways = random8(GLITTER_THROWAWAYS);
  }

	uint16_t oldSeed = random16_get_seed();
	random16_set_seed(seed);
	for (uint8_t i = 0; i < throwaways + GLITTER_PIXELS; i++)
	{
		if (i >= throwaways) {
      auto idx = random8(NUM_LEDS);
			leds[idx] = CRGB::White;
		}
		else
			random8();
	}
	random16_set_seed(oldSeed);
}

#pragma endregion
