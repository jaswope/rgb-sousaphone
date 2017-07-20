#pragma once
#include <FastLED.h>


namespace Palettes {

	class StripesPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			CRGB black = CRGB::Black;
			CRGB white = CRGB::White;

			currentPalette = CRGBPalette16(
				black, black, white, white,
				black, black, white, white,
				black, black, white, white,
				black, black, white, white);
		}

		static const char* getName() {
			return "STRP";
		}
	};

	class PinkStripesPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			CRGB black = CRGB::Black;
			CRGB pink = CRGB::DeepPink;

			currentPalette = CRGBPalette16(
				black, black, pink, pink,
				black, black, pink, pink,
				black, black, pink, pink,
				black, black, pink, pink);
		}

		static const char* getName() {
			return "PNKS";
		}
	};

	class SeedAndFeedPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			CRGB red = CRGB::Red;
			CRGB gold = CRGB::Gold;

			currentPalette = CRGBPalette16(
				red, red, gold, gold,
				red, red, gold, gold,
				red, red, gold, gold,
				red, red, gold, gold);
		}

		static const char* getName() {
			return "SFMA";
		}
	};

	class ChristmasPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			CRGB red = CRGB::Red;
			CRGB green = CRGB::Green;

			currentPalette = CRGBPalette16(
				red, red, red, red,
				green, green, green, green,
				red, red, red, red,
				green, green, green, green);
		}

		static const char* getName() {
			return "XMAS";
		}
	};

	class CandyCanePalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			CRGB red = CRGB::Red;
			CRGB white = CRGB::LightGrey;

			currentPalette = CRGBPalette16(
				red, red, white, white,
				red, red, white, white,
				red, red, white, white,
				red, red, white, white);
		}

		static const char* getName() {
			return "CANE";
		}
	};

	class MardiGrasPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			CRGB purple = CHSV(HUE_PURPLE, 255, 255);
			CRGB green = CHSV(HUE_GREEN, 255, 255);
			CRGB yellow = CHSV(HUE_YELLOW, 255, 255);;

			currentPalette = CRGBPalette16(
				green, green, purple, purple, yellow, yellow, purple, purple,
				green, green, purple, purple, yellow, yellow, purple, purple);
		}

		static const char* getName() {
			return "GRAS";
		}
	};

	class PatrioticPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			CRGB red = CRGB::Red;
			CRGB blue = CRGB::Blue;
			CRGB white = CRGB::LightGrey;

			currentPalette = CRGBPalette16(
				blue, blue, blue, blue, white, white, red, red,
				white, white, red, red, white, white, red, red);
		}

		static const char* getName() {
			return "USA";
		}
	};

	class MexicanFlagPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			CRGB red = CRGB::Red;
			CRGB green = CRGB::Green;
			CRGB white = CRGB::LightGrey;

			currentPalette = CRGBPalette16(
				CRGB(64, 64, 0), green, green, green,
				green, green, white, white,
				white, white, white, red,
				red, red, red, red);
		}

		static const char* getName() {
			return "MEX";
		}
	};

	class RainbowPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = CRGBPalette16(RainbowColors_p);
		}

		static const char* getName() {
			return "RAIN";
		}
	};

	class OceanPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = CRGBPalette16(OceanColors_p);
		}

		static const char* getName() {
			return "AQUA";
		}
	};

	class HeatPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = CRGBPalette16(LavaColors_p);
		}

		static const char* getName() {
			return "HEAT";
		}
	};

	class SkyPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = CRGBPalette16(CloudColors_p);
		}

		static const char* getName() {
			return "SKY";
		}
	};

	class RedPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = CRGBPalette16(CRGB::Red);
		}

		static const char* getName() {
			return "RED";
		}
	};

	class GreenPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = CRGBPalette16(CRGB::Green);
		}

		static const char* getName() {
			return "GRN";
		}
	};

	CRGBPalette16 pulseColor(CRGB color) {
		return CRGBPalette16(CRGB::Black, CRGB::Black, color, CRGB::Black);
	}

	class GreenPulsePalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = pulseColor(CRGB::Green);
		}

		static const char* getName() {
			return "PLSG";
		}
	};

	class BluePulsePalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = pulseColor(CRGB::Blue);
		}

		static const char* getName() {
			return "PLSB";
		}
	};

	class PinkPulsePalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = pulseColor(CRGB::DeepPink);
		}

		static const char* getName() {
			return "PLSP";
		}
	};

	class BluePalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = CRGBPalette16(CRGB::Blue);
		}

		static const char* getName() {
			return "BLUE";
		}
	};

	class WhitePalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = CRGBPalette16(CRGB::LightGrey);
		}

		static const char* getName() {
			return "WHTE";
		}
	};

	class PinkPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = CRGBPalette16(CRGB::DeepPink);
		}

		static const char* getName() {
			return "PINK";
		}
	};

	class BlackPalette {
	public:
		static void getPalette(CRGBPalette16 &currentPalette) {
			currentPalette = CRGBPalette16(CRGB::Black);
		}

		static const char* getName() {
			return "BLCK";
		}
	};

	#define AS_PALETTE(A) {A::getName, A::getPalette}

	typedef void GetPalette(CRGBPalette16 &currentPalette);
	typedef const char* GetName(void);

	struct Palette {
		GetName *getName;
		GetPalette *getPalette;
	};

	Palette PaletteList[] = {
		AS_PALETTE(RainbowPalette),
		AS_PALETTE(StripesPalette),
		AS_PALETTE(PinkStripesPalette),
		AS_PALETTE(MardiGrasPalette),
		AS_PALETTE(PatrioticPalette),
		AS_PALETTE(MexicanFlagPalette),
		AS_PALETTE(SeedAndFeedPalette),
		AS_PALETTE(ChristmasPalette),
		AS_PALETTE(CandyCanePalette),
		AS_PALETTE(OceanPalette),
		AS_PALETTE(HeatPalette),
		AS_PALETTE(GreenPulsePalette),
		AS_PALETTE(BluePulsePalette),
		AS_PALETTE(PinkPulsePalette),
		AS_PALETTE(RedPalette),
		AS_PALETTE(GreenPalette),
		AS_PALETTE(BluePalette),
		AS_PALETTE(WhitePalette),
		AS_PALETTE(BlackPalette),
		AS_PALETTE(PinkPalette),
	};
}
