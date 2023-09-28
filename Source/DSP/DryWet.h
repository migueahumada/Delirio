/*
  ==============================================================================

    DryWet.h
    Created: 22 Sep 2023 2:57:04pm
    Author:  Miguel Angel Ahumada Gaona

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DryWet
{
public:
    DryWet();
    ~DryWet();
    
    void process(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& wetBuffer, float dryWetValue);
    
private:
    float wetSample{0.0f};
    float drySample{0.0f};
    float outSample{0.0f};
    
    
};
