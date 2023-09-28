/*
  ==============================================================================

    SVFilter.cpp
    Created: 27 Sep 2023 9:07:43pm
    Author:  Miguel Angel Ahumada Gaona

  ==============================================================================
*/

#include "SVFilter.h"

SVFilter::SVFilter(){}
SVFilter::~SVFilter(){}

void SVFilter::prepare(double inSampleRate, int samplesPerBlock,int inNumChannels, juce::dsp::StateVariableTPTFilterType filterType)
{
    filter.reset();
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = inSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = inNumChannels;
    
    filter.setType(filterType);
    filter.setResonance(resonance);
    filter.setCutoffFrequency(0.0f);
    
}
void SVFilter::process(juce::AudioBuffer<float>& inBuffer, float cutoffFrequency)
{
    
    for (int channel = 0; channel < inBuffer.getNumChannels(); channel++) {
        for (int i = 0; i < inBuffer.getNumSamples(); i++) {
            
            inSample = inBuffer.getSample(channel, i);
            filter.setCutoffFrequency(cutoffFrequency);
            outSample = filter.processSample(channel, inSample);
            
            inBuffer.setSample(channel, i, outSample);
        }
    }
    

}




