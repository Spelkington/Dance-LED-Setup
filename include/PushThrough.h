#ifndef PUSHTHROUGH_H
#define PUSHTHROUGH_H

#include "Changer.h"

class PushThrough: public Changer
{
    public:

        PushThrough(CRGBSet _leds) : Changer(_leds) {}
        PushThrough(CRGBSet _leds, CRGBPalette16 _palette) : Changer(_leds) {}

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