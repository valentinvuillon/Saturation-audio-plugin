/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using namespace juce;

//==============================================================================
/**
*/
class Waveshaper_v4AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Waveshaper_v4AudioProcessorEditor (Waveshaper_v4AudioProcessor&);
    ~Waveshaper_v4AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    Slider input_gain_knob;
    Label input_gain_Label;
    
    Slider dry_wet_knob;
    Label dry_wet_Label;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Waveshaper_v4AudioProcessor& audioProcessor;
    
public:
    
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> input_gain_KnobValue;
    std::unique_ptr <AudioProcessorValueTreeState::SliderAttachment> dry_wet_KnobValue;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Waveshaper_v4AudioProcessorEditor)
};
