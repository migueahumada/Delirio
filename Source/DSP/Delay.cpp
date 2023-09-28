/*
  ==============================================================================

    Delay.cpp
    Created: 19 Sep 2023 11:06:48pm
    Author:  Miguel Angel Ahumada Gaona

  ==============================================================================
*/

#include "Delay.h"

Delay::Delay()
{
    
}
Delay::~Delay(){}


void Delay::prepare(double inSamplesRate, int samplesPerBlock, int inNumChannels)
{
    delayObject.setMaximumDelayInSamples((int)inSamplesRate);
    currentSampleRate = inSamplesRate;
    
    spec.sampleRate = inSamplesRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = inNumChannels;
    
    delayObject.reset();
    delayObject.prepare(spec);
    
}

void Delay::process(juce::AudioBuffer<float>& inBuffer, float feedback, float gain, float delayTime)
{
    float inSample = {0.0f};
    float outSample = {0.0f};
    float delayedSample = {0.0f};
    float inDelay = {0.0f};
    

    for (int channel = 0; channel < inBuffer.getNumChannels(); channel++) {
        for (int i = 0; i < inBuffer.getNumSamples(); i++) {
            
            inSample = inBuffer.getSample(channel, i);
            
            delayedSample = delayObject.popSample(channel, ((currentSampleRate * delayTime)/1000));
            
            inDelay = inSample + (feedback * delayedSample);
            
            delayObject.pushSample(channel, inDelay);
            
            outSample = (inSample + delayedSample) * gain;
            
            inBuffer.setSample(channel, i, outSample);
            
        }
    }
    
    
    
}
