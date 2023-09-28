/*
  ==============================================================================

    Stereorizer.h
    Created: 28 Sep 2023 12:25:49pm
    Author:  Miguel Angel Ahumada Gaona

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Stereorizer
{
public:
    Stereorizer();
    ~Stereorizer();
    void process(juce::AudioBuffer<float>& monoBuffer);
private:
    
    
};
