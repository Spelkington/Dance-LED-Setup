#ifndef PUSHTHROUGH_H
#define PUSHTHROUGH_H

#include "Changer.h"

class PushThrough: public Changer
{
    public:

        PushThrough(int* _channels, CRGBSet _leds, int _minLED, int _maxLED) : Changer(_channels, _leds, _minLED, _maxLED) {}
        PushThrough(int* _channels, CRGBSet _leds, int _minLED, int _maxLED, CRGBPalette16 _palette) : Changer(_channels, _leds, _minLED, _maxLED, _palette) {}

        virtual void step();

    private:

        void shiftPixDown(int speed);

        long ticks = 0;

        int getMaxChannel();
        CRGB getColorFromPaletteWeight();

        void setPulseColor(int maxChannel);
        CRGB pulseColor = CRGB::Black;
        CRGB scalePulseBrightness(float scale);

        int threshold = 0;
        int brightnessScalar = 0;

};

#endif