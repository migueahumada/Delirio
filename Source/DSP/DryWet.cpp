/*
  ==============================================================================

    DryWet.cpp
    Created: 22 Sep 2023 2:57:04pm
    Author:  Miguel Angel Ahumada Gaona

  ==============================================================================
*/

#include "DryWet.h"

DryWet::DryWet(){}
DryWet::~DryWet(){}


void DryWet::process(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& wetBuffer, float dryWetValue)
{
    dryWetValue = dryWetValue / 100.0f;
    
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            
            drySample = buffer.getSample(channel, i);
            wetSample = wetBuffer.getSample(channel, i);
            
            outSample = drySample * (1.0f - dryWetValue) + (dryWetValue * wetSample);
            
            wetBuffer.setSample(channel, i, outSample);
            
            
        }
    }
    
}

