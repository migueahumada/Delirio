/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelirioAudioProcessor::DelirioAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this,nullptr,"Parameters",createParameters())
#endif
{
}

DelirioAudioProcessor::~DelirioAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout DelirioAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout parameters;
    
//-------Parameters of the plugin-----------------
    
    //Feedback
    parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"Feedback",1}, "Feedback", 0.0f, 0.9f, 0.2f));
    //Delay
    parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"DelayTime",1}, "DelayTime", 0.0f, 4000.0f,250.0f));
    //LPF
    parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"LPFCutoffFreq", 1}, "LPFCutoffFreq", 2000.0f, 20000.0f,20000.0f));
    //HPF
    parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"HPFCutoffFreq", 1}, "HPFCutoffFreq", 0.0f, 2000.0f,0.0f));
    //Gain Out
    parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"Gain",1}, "Gain", 0.0f, 2.0f,1.0f));
    //Dry Wet
    parameters.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"DryWet",1}, "DryWet", 0.0f, 100.0f,100.0f));
    
    
    return parameters;
}

//==============================================================================
const juce::String DelirioAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelirioAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelirioAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelirioAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelirioAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelirioAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelirioAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelirioAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelirioAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelirioAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelirioAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lpf.prepare(sampleRate, samplesPerBlock, getTotalNumInputChannels(), juce::dsp::StateVariableTPTFilterType::lowpass);
    hpf.prepare(sampleRate, samplesPerBlock, getTotalNumInputChannels(), juce::dsp::StateVariableTPTFilterType::highpass);
    myDelay.prepare(sampleRate, samplesPerBlock, getTotalNumInputChannels());
}

void DelirioAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelirioAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelirioAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChanels = getTotalNumOutputChannels();
    
    for (auto i = totalNumOutputChanels; i < totalNumOutputChanels; ++i) buffer.clear(i,0,buffer.getNumSamples());
    
    auto feedbackParameter = apvts.getRawParameterValue("Feedback")->load();
    auto delayTimeParameter = apvts.getRawParameterValue("DelayTime")->load();
    auto lpfCutoffParameter = apvts.getRawParameterValue("LPFCutoffFreq")->load();
    auto hpfCutoffParameter = apvts.getRawParameterValue("HPFCutoffFreq")->load();
    auto gainParameter = apvts.getRawParameterValue("Gain")->load();
    auto dryWetParameter = apvts.getRawParameterValue("DryWet")->load();
    
    dryBuffer.makeCopyOf(buffer);

    
    myDelay.process(buffer, feedbackParameter, gainParameter, delayTimeParameter);
    lpf.process(buffer, lpfCutoffParameter);
    hpf.process(buffer, hpfCutoffParameter);
    
    //stereorizer.process(buffer);
    
    myDryWet.process(dryBuffer, buffer, dryWetParameter);
    
}

//==============================================================================
bool DelirioAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelirioAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void DelirioAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelirioAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelirioAudioProcessor();
}
