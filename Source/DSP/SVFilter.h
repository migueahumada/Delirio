/*
  ==============================================================================

    SVFilter.h
    Created: 27 Sep 2023 9:07:43pm
    Author:  Miguel Angel Ahumada Gaona

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SVFilter
{
public:
    SVFilter();
    ~SVFilter();
        

    
    void prepare(double inSampleRate, int samplePerBlock, int inNumChannel, juce::dsp::StateVariableTPTFilterType filterType);
    void process(juce::AudioBuffer<float>& inBuffer, float cutoffFrequency);
    
    float cutoffFrequency = {0.0f};
    float resonance = {1/std::sqrt(2.0f)};
    float inSample={0.0f};
    float outSample={0.0f};
    
private:
    
    juce::dsp::StateVariableTPTFilter<float> filter;

    

};
