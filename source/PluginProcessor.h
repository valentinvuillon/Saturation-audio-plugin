/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define INPUT_GAIN_ID "input gain"
#define INPUT_GAIN_NAME "Input gain"
#define DRY_WET_ID "dry wet"
#define DRY_WET_NAME "Dry wet"

using namespace juce;


//==============================================================================
/**
*/
class Waveshaper_v4AudioProcessor  : public juce::AudioProcessor, public AudioProcessorValueTreeState::Listener
                            
{
public:
    //==============================================================================
    Waveshaper_v4AudioProcessor();
    ~Waveshaper_v4AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void parameterChanged  (const String &parameterID, float newValue) override;
    
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    AudioProcessorValueTreeState treeState;
    
private:
    float input_gain;
    float dry_wet;
    float dry_signal;
    float wet_signal;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Waveshaper_v4AudioProcessor)
};
