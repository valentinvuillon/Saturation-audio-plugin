/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Waveshaper_v4AudioProcessorEditor::Waveshaper_v4AudioProcessorEditor (Waveshaper_v4AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    input_gain_knob.setSliderStyle(Slider::Rotary);
    input_gain_knob.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 13);
    input_gain_KnobValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, INPUT_GAIN_ID, input_gain_knob);
    addAndMakeVisible(&input_gain_knob);
    audioProcessor.treeState.addParameterListener(INPUT_GAIN_ID, &audioProcessor);
    input_gain_Label.setText("Input Gain", dontSendNotification);
    addAndMakeVisible(input_gain_Label);
    
    dry_wet_knob.setSliderStyle(Slider::Rotary);
    dry_wet_knob.setTextBoxStyle(Slider::TextBoxBelow, true, 40, 13);
    dry_wet_KnobValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, DRY_WET_ID, dry_wet_knob);
    addAndMakeVisible(&dry_wet_knob);
    audioProcessor.treeState.addParameterListener(DRY_WET_ID, &audioProcessor);
    dry_wet_Label.setText("Dry/Wet", dontSendNotification);
    addAndMakeVisible(dry_wet_Label);
  
    setSize (300,150);
}

Waveshaper_v4AudioProcessorEditor::~Waveshaper_v4AudioProcessorEditor()
{
}

//==============================================================================
void Waveshaper_v4AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
}

void Waveshaper_v4AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    input_gain_knob.setBounds(25, 25, 100, 100);
    input_gain_Label.setBounds(40, 130, 100, 12);
    dry_wet_knob.setBounds(175,25, 100, 100);
    dry_wet_Label.setBounds(197, 130, 100, 12);
    
}
