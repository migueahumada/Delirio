/*
  ==============================================================================

    Delay.h
    Created: 19 Sep 2023 11:06:48pm
    Author:  Miguel Angel Ahumada Gaona

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Delay
{
public:
    
    //Constructores
    Delay();
    ~Delay();
    
    void prepare(double inSamplesRate, int samplesPerBlock, int inNumChannels);
    void process(juce::AudioBuffer<float>& inBuffer, float feedback, float gain, float delayTime);
    
    float currentSampleRate{0.0f};
    float feedback{0.3f};
    float delay{250.0f};
    float gain{0.7f};
    
    juce::dsp::ProcessSpec spec;
    
    
private:
    //Variables
    
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayObject;
};

