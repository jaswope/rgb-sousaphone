#pragma once
#include "PROGMEM_readAnything.h"
#include <FastLED.h>
#include "palettes.h"
#include "tuba_bell.h"

namespace Patterns {
	#define AS_PATTERN(A) {A::getName, A::getPalette, A::render}
 
	typedef void Render(long now, CRGBPalette16 &currentPalette);
	typedef void GetPalette(CRGBPalette16 &currentPalette);
	typedef const char* GetName(void);
	typedef void PerPixel(long now, CRGBPalette16 &currentPalette, CRGB &led, PositionInfo currentPositionInfo);

	class PatternClass
	{
	public:
		static void setup(CRGB *leds, const PositionInfo *ledInfo, int numLeds);

	protected:
		static CRGB *m_leds;
		static int m_nLeds;
		static const PositionInfo *m_ledInfo;
		static PositionInfo getPositionInfo(uint8_t pos);
		static void eachPixel(PerPixel perPixel, long now, CRGBPalette16 currentPalette);
	};

	CRGB *PatternClass::m_leds;
	int PatternClass::m_nLeds;
	const PositionInfo *PatternClass::m_ledInfo;

	void PatternClass::setup(CRGB *leds, const PositionInfo *ledInfo, int numLeds)
	{
		PatternClass::m_leds = leds;
		PatternClass::m_ledInfo = ledInfo;
		PatternClass::m_nLeds = numLeds;
	}

	void PatternClass::eachPixel(PerPixel perPixel, long now, CRGBPalette16 currentPalette)
	{
		for (int i = 0; i < PatternClass::m_nLeds; i++)
		{
			perPixel(now, currentPalette, PatternClass::m_leds[i], getPositionInfo(i));
		}
	}

	PositionInfo PatternClass::getPositionInfo(uint8_t pos)
	{
		return PROGMEM_getAnything(&PatternClass::m_ledInfo[pos]);
	}

	class CycledPattern : public PatternClass
	{
	protected:
		static const long cycleTime = 2500;
	};

	class SpinPattern : public CycledPattern
	{
	public:
		static const char* getName()
		{
			return "SPIN";
		}

		static void getPalette(CRGBPalette16 &currentPalette)
		{
			Palettes::RainbowPalette::getPalette(currentPalette);
		}

		static void render(long now, CRGBPalette16 &currentPalette)
		{
			eachPixel(perPixel, now, currentPalette);
		}
	protected:
		static void perPixel(long now, CRGBPalette16 &currentPalette, CRGB &led, PositionInfo currentPositionInfo)
		{
			led = ColorFromPalette(currentPalette, currentPositionInfo.theta + map(now % cycleTime, 0, cycleTime, 0, 255), 255, LINEARBLEND);
		}
	};

	class SpiralPattern : public CycledPattern
	{
	public:
		static const char* getName()
		{
			return "SPRL";
		}

		static void getPalette(CRGBPalette16 &currentPalette)
		{
			Palettes::StripesPalette::getPalette(currentPalette);
		}

		static void render(long now, CRGBPalette16 &currentPalette)
		{
			eachPixel(perPixel, now, currentPalette);
		}
	protected:
		static void perPixel(long now, CRGBPalette16 &currentPalette, CRGB &led, PositionInfo currentPositionInfo)
		{
			led = ColorFromPalette(currentPalette, currentPositionInfo.theta + map(now % cycleTime, 0, cycleTime, 0, 255) - (currentPositionInfo.r8 / 2), 255, LINEARBLEND);
		}
	};


	class FadePattern : public CycledPattern
	{
	public:
		static const char* getName()
		{
			return "FADE";
		}

		static void getPalette(CRGBPalette16 &currentPalette)
		{
			Palettes::RainbowPalette::getPalette(currentPalette);
		}

		static void render(long now, CRGBPalette16 &currentPalette)
		{
			fill_solid(PatternClass::m_leds, PatternClass::m_nLeds, ColorFromPalette(currentPalette, map(now % cycleTime, 0, cycleTime, 0, 255), 255, LINEARBLEND));
		}
	};

	class ScanPattern : public CycledPattern
	{
	public:
		static const char* getName()
		{
			return "SCAN";
		}

		static void getPalette(CRGBPalette16 &currentPalette)
		{
			Palettes::RedPalette::getPalette(currentPalette);
		}

		static void render(long now, CRGBPalette16 &currentPalette)
		{
			beat = quadwave8((uint8_t)map(now % beatTime, 0, beatTime, 0, 255));
			eachPixel(perPixel, now, currentPalette);
		}
	protected:
		static const long beatTime = 1500;
		static uint8_t beat;

		static void perPixel(long now, CRGBPalette16 &currentPalette, CRGB &led, PositionInfo currentPositionInfo)
		{
			uint8_t normpos = currentPositionInfo.y8 + 127;
			uint8_t distance = (normpos > beat ? (normpos - beat) : (beat - normpos));
			uint16_t dsquared = (uint16_t)distance * 3;
			uint8_t brightness = dsquared >= 255 ? 0 : 255 - dsquared;
			led = ColorFromPalette(currentPalette, map(now % cycleTime, 0, cycleTime, 0, 255), brightness, LINEARBLEND);
		}
	};
	uint8_t ScanPattern::beat;

	class LavaPattern : public PatternClass
	{
	public:
		static const char* getName()
		{
			return "LAVA";
		}

		static void getPalette(CRGBPalette16 &currentPalette)
		{
			Palettes::HeatPalette::getPalette(currentPalette);
		}

		static void render(long now, CRGBPalette16 &currentPalette)
		{
			lava_z = (uint16_t)(now / 2);
			eachPixel(perPixel, now, currentPalette);
		}
	protected:
		static const int scale = 5;
		static uint16_t lava_z;
		static void perPixel(long now, CRGBPalette16 &currentPalette, CRGB &led, PositionInfo currentPositionInfo)
		{
			uint8_t data = inoise8((uint16_t)currentPositionInfo.x8 * scale + (lava_z / 8), (uint16_t)currentPositionInfo.y8 * scale + (lava_z / 16), lava_z);
			data = qsub8(data, 16);
			data = qadd8(data, scale8(data, 39));
			led = ColorFromPalette(currentPalette, data, 255, LINEARBLEND);
		}
	};
	uint16_t LavaPattern::lava_z;

	class VerticalPattern : public CycledPattern
	{
	public:
		static const char* getName()
		{
			return "VERT";
		}

		static void getPalette(CRGBPalette16 &currentPalette)
		{
			Palettes::RainbowPalette::getPalette(currentPalette);
		}

		static void render(long now, CRGBPalette16 &currentPalette)
		{
			eachPixel(perPixel, now, currentPalette);
		}
	protected:
		static void perPixel(long now, CRGBPalette16 &currentPalette, CRGB &led, PositionInfo currentPositionInfo)
		{
			led = ColorFromPalette(currentPalette, (255 - currentPositionInfo.y8) - map(now % cycleTime, 0, cycleTime, 0, 255), 255, LINEARBLEND);
		}
	};

	class WavePattern : public CycledPattern
	{
	public:
		static const char* getName()
		{
			return "WAVE";
		}

		static void getPalette(CRGBPalette16 &currentPalette)
		{
			Palettes::RainbowPalette::getPalette(currentPalette);
		}

		static void render(long now, CRGBPalette16 &currentPalette)
		{
			pos = map(now % cycleTime * 2, 0, cycleTime * 2, 0, 255);
			eachPixel(perPixel, now, currentPalette);
		}
	protected:
		static uint8_t pos;
		static void perPixel(long now, CRGBPalette16 &currentPalette, CRGB &led, PositionInfo currentPositionInfo)
		{
			auto w = quadwave8(pos + (currentPositionInfo.y8));
			led = ColorFromPalette(currentPalette, currentPositionInfo.x8 + w, 255, LINEARBLEND);
		}
	};

	uint8_t WavePattern::pos = 0;

	class RadialInPattern : public CycledPattern
	{
	public:
		static const char* getName()
		{
			return "RIN";
		}

		static void getPalette(CRGBPalette16 &currentPalette)
		{
			Palettes::RainbowPalette::getPalette(currentPalette);
		}

		static void render(long now, CRGBPalette16 &currentPalette)
		{
			eachPixel(perPixel, now, currentPalette);
		}
	protected:
		static void perPixel(long now, CRGBPalette16 &currentPalette, CRGB &led, PositionInfo currentPositionInfo)
		{
			led = ColorFromPalette(currentPalette, (255 - currentPositionInfo.r8) - map(now % cycleTime, 0, cycleTime, 0, 255), 255, LINEARBLEND);
		}
	};

	class RadialOutPattern : public CycledPattern
	{
	public:
		static const char* getName()
		{
			return "ROUT";
		}

		static void getPalette(CRGBPalette16 &currentPalette)
		{
			Palettes::RainbowPalette::getPalette(currentPalette);
		}

		static void render(long now, CRGBPalette16 &currentPalette)
		{
			eachPixel(perPixel, now, currentPalette);
		}
	protected:
		static void perPixel(long now, CRGBPalette16 &currentPalette, CRGB &led, PositionInfo currentPositionInfo)
		{
			led = ColorFromPalette(currentPalette, currentPositionInfo.r8 - map(now % cycleTime, 0, cycleTime, 0, 255), 255, LINEARBLEND);
		}
	};

	class CloudsPattern : public PatternClass
	{
	public:
		static const char* getName()
		{
			return "CLDS";
		}

		static void getPalette(CRGBPalette16 &currentPalette)
		{
			Palettes::SkyPalette::getPalette(currentPalette);
		}

		static void render(long now, CRGBPalette16 &currentPalette)
		{
			eachPixel(perPixel, now, currentPalette);
		}
	protected:
		static const int scale = 5;
		static void perPixel(long now, CRGBPalette16 &currentPalette, CRGB &led, PositionInfo currentPositionInfo)
		{
			uint8_t data = inoise8((uint16_t)currentPositionInfo.x8 * scale, (uint16_t)currentPositionInfo.y8 + (now / 12));
			data = qsub8(data, 16);
			data = qadd8(data, scale8(data, 39));
			data = scale8(data, 25) + scale8(data, 256 - 25);
			nblend(led, ColorFromPalette(currentPalette, data, 255, LINEARBLEND), 192);
		}
	};

	class FirePattern : public PatternClass
	{
	public:
		static const char* getName()
		{
			return "FIRE";
		}

		static void getPalette(CRGBPalette16 &currentPalette)
		{
			currentPalette = HeatColors_p;
		}

		static void render(long now, CRGBPalette16 &currentPalette)
		{
			eachPixel(perPixel, now, currentPalette);
		}
	protected:
		static const int scale = 5;
		static void perPixel(long now, CRGBPalette16 &currentPalette, CRGB &led, PositionInfo currentPositionInfo)
		{
			uint8_t data = inoise8((uint16_t)currentPositionInfo.theta * scale, ((uint16_t)currentPositionInfo.r8 + (now/3)) * scale);
			data = qsub8(data, 16);
			data = qadd8(data, scale8(data, 39));
			data = scale8(data, 25) + scale8(data, 255 - 25);
			data = qsub8(data, 254 - currentPositionInfo.r8 * 2);
			if (data > 253)
				data = 253; // >= 254 shows up as the wrong color...bug in ColorFromPalette?
			led = ColorFromPalette(currentPalette, data, 255, LINEARBLEND);
		}
	};

	class AudioBurstPattern : public PatternClass
	{
	public:
		static void setup(uint8_t micPin);

		static const char* getName()
		{
			return "AUDB";
		}

		static void getPalette(CRGBPalette16 &currentPalette)
		{
			Palettes::RainbowPalette::getPalette(currentPalette);
		}

		static void render(long now, CRGBPalette16 &currentPalette)
		{
			int n = analogRead(micPin);
			n = abs(n - 512);
			n = (n <= noise) ? 0 : (n - noise);
			lvl = ((lvl * 7) + n) >> 3;
			height = top * lvl / 150;

			if (height <= 1 && fixColor)
				fixColor = false;

			if (height > 1 && !fixColor)
			{
				fixColor = true;
				burstColor += 64 + random8(64);
			}

			eachPixel(perPixel, now, currentPalette);
		}
	protected:
		static uint8_t micPin;
		static const int noise = 50;
		static const int top = 9; // TODO: use radius instead of ring count
		static int lvl;
		static int height;
		static uint8_t burstColor;
		static bool fixColor;

		static void perPixel(long now, CRGBPalette16 &currentPalette, CRGB &led, PositionInfo currentPositionInfo)
		{
			led = currentPositionInfo.ring < height ? ColorFromPalette(currentPalette, burstColor) : CRGB::Black;
		}
	};

	int AudioBurstPattern::lvl = 10;
	int AudioBurstPattern::height = 0;
	uint8_t AudioBurstPattern::burstColor = 0;
	uint8_t AudioBurstPattern::micPin;
	bool AudioBurstPattern::fixColor = false;
	void AudioBurstPattern::setup(uint8_t micPin) {
		AudioBurstPattern::micPin = micPin;
	}


	void Setup(CRGB* leds, const PositionInfo* ledInfo, int numLeds) {
		PatternClass::setup(leds, ledInfo, numLeds);
	}

	void Setup(CRGB* leds, const PositionInfo* ledInfo, int numLeds, uint8_t micPin) {
		PatternClass::setup(leds, ledInfo, numLeds);
		AudioBurstPattern::setup(micPin);
	}

	struct Pattern {
		GetName *getName;
		GetPalette *getPalette;
		Render *render;
	};

	static const Pattern PatternList[]{
		AS_PATTERN(SpinPattern),
		AS_PATTERN(VerticalPattern),
		AS_PATTERN(SpiralPattern),
		AS_PATTERN(RadialOutPattern),
		AS_PATTERN(RadialInPattern),
		AS_PATTERN(WavePattern),
		AS_PATTERN(LavaPattern),
		AS_PATTERN(FirePattern),
		AS_PATTERN(AudioBurstPattern),
		AS_PATTERN(ScanPattern),
		AS_PATTERN(FadePattern),
		AS_PATTERN(CloudsPattern),
	};
}

