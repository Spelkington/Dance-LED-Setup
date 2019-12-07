#include <FastLED.h>
#include "Changer.h"
#include "BassPush.h"

#define MIN_THRESHOLD 300
#define LIGHT_DIM_INTERVAL 75
#define THRESH_DIM_INTERVAL 45

#define NUM_CHANNELS 1
#define PUSH_SPEED 10
#define CORE_LENGTH 250

void BassPush::step() {

    int maxChannel = getMaxChannel();

    setPulseColor(maxChannel);

    CRGB scaled = scalePulseBrightness(brightnessScalar / 1024.0);

//    Serial.print(scaled.r);
//    Serial.print('\t');
//    Serial.print(scaled.g);
//    Serial.print('\t');
//    Serial.print(scaled.b);
//    Serial.println();

//    leds(minLED, maxLED) = scaled;

    this->shiftPixDown(PUSH_SPEED);

    leds(maxLED - CORE_LENGTH, maxLED) = scaled;

    return;

}

void BassPush::shiftPixDown(int speed)
{

    int next;
    CRGB nextColor;
    int offset = this->ticks % speed;
    this->ticks++;

    for (int i = this->minLED + offset; i < this->maxLED; i += speed)
    {

        next = i + speed;
        if (next >= this->maxLED)
        {
            next = maxLED - 1;
        }
        nextColor = leds[next + 1];

        this->leds(i, next).fill_solid(nextColor);

    }

}

int BassPush::getMaxChannel()
{
    int max = 0;
    int maxChannel = 0;

    for (int i = 0; i < NUM_CHANNELS; i++)
    {
        if (channels[i] > max)
        {
            max = channels[i];
            maxChannel = i;
        }
    }

    return maxChannel;

}

CRGB BassPush::getColorFromPaletteWeight()
{
    int weightedSum = 0;

    for (int i = 0; i < 7; i++)
    {
        weightedSum += channels[i] * (i + 1);
    }

    float weightedAvgChannel = weightedSum / (NUM_CHANNELS + 1);

    int paletteIndex = int(weightedAvgChannel * 255);

    return getColorFromPalette(paletteIndex);

}

void BassPush::setPulseColor(int maxChannel)
{
    int maxAmplitude = channels[maxChannel];

    if (maxAmplitude > threshold)
    {
        threshold = maxAmplitude;
        brightnessScalar = maxAmplitude;
        pulseColor = getColorFromPaletteWeight();
    }
    else if (maxAmplitude > MIN_THRESHOLD)
    {
        threshold -= THRESH_DIM_INTERVAL;
        brightnessScalar -= LIGHT_DIM_INTERVAL;

        if (threshold < 0)
        {
            threshold = 0;
        }

        if (brightnessScalar < 0)
        {
            brightnessScalar = 0;
        }

    }
    else
    {
        pulseColor = CRGB::Black;
    }
}

CRGB BassPush::scalePulseBrightness(float scale)
{
    CRGB scaled = CRGB(
        uint16_t(pulseColor.r * scale),
        uint16_t(pulseColor.g * scale),
        uint16_t(pulseColor.b * scale)
    );

    return scaled;
}