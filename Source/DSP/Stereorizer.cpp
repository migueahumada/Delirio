/*
  ==============================================================================

    Stereorizer.cpp
    Created: 28 Sep 2023 12:25:49pm
    Author:  Miguel Angel Ahumada Gaona

  ==============================================================================
*/

#include "Stereorizer.h"

Stereorizer::Stereorizer(){}
Stereorizer::~Stereorizer(){}

void Stereorizer::process(juce::AudioBuffer<float>& monoBuffer)
{
    juce::AudioBuffer<float> stereoBuffer(2,monoBuffer.getNumSamples());
    float pan = {0.0f};
    
    for (int channel = 0; channel < 2; channel++) {
        for (int i = 0; i < monoBuffer.getNumSamples(); i++) {
            
            if (channel == 0) {
                pan = -1.0f;
            }else
            {
                pan = 1.0f;
            }
            
            stereoBuffer.setSample(channel, i, monoBuffer.getSample(0, i));
            
        }
    }
    
    monoBuffer.makeCopyOf(stereoBuffer);
}
