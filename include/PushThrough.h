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

        int squeezeChannelsToInt(int bass, int mid, int treb);

        long ticks = 0;

};

#endif