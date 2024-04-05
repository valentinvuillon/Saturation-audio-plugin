/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "DSP_functions.h"

//==============================================================================
Waveshaper_v4AudioProcessor::Waveshaper_v4AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#else
        : AudioProcessor (BusesProperties()
                    #if ! JucePlugin_IsMidiEffect
                        #if ! JucePlugin_IsSynth
                      .withInput  ("Input",  AudioChannelSet::stereo(), true)
                        #endif
                      .withOutput ("Output", AudioChannelSet::stereo(), true)
                    #endif
                      ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    treeState.addParameterListener("input gain", this);
    treeState.addParameterListener("dry wet", this);
    input_gain = 1.0f;
    dry_wet = 0.0f;
 
}

Waveshaper_v4AudioProcessor::~Waveshaper_v4AudioProcessor()
{
}

//==============================================================================
const juce::String Waveshaper_v4AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

AudioProcessorValueTreeState::ParameterLayout Waveshaper_v4AudioProcessor::createParameterLayout(){
AudioProcessorValueTreeState::ParameterLayout layout;
layout.add(std::make_unique<AudioParameterFloat>(INPUT_GAIN_ID, INPUT_GAIN_NAME, 1.0f, 20.0f, 1.0f));
layout.add(std::make_unique<AudioParameterFloat>(DRY_WET_ID, DRY_WET_NAME, 0.0f, 1.0f, 0.0f));
    

return layout;
}

void Waveshaper_v4AudioProcessor::parameterChanged  (const String &parameterID, float newValue){
    if (parameterID.compare("input gain")==0) input_gain = newValue;
    if (parameterID.compare("dry wet")==0) dry_wet = newValue;
}


bool Waveshaper_v4AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Waveshaper_v4AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Waveshaper_v4AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Waveshaper_v4AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Waveshaper_v4AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Waveshaper_v4AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Waveshaper_v4AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Waveshaper_v4AudioProcessor::getProgramName (int index)
{
    return {};
}

void Waveshaper_v4AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Waveshaper_v4AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void Waveshaper_v4AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Waveshaper_v4AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Waveshaper_v4AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
         for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
               {
                   dry_signal = buffer.getSample(channel, sample);
                   
                   wet_signal=waveshaping_function(input_gain*dry_signal);
                   
                   channelData[sample]= dry_wet*wet_signal+(1-dry_wet)*dry_signal;
           
                       
               }
    }
}

//==============================================================================
bool Waveshaper_v4AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Waveshaper_v4AudioProcessor::createEditor()
{
    return new Waveshaper_v4AudioProcessorEditor (*this);
}

//==============================================================================
void Waveshaper_v4AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Waveshaper_v4AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Waveshaper_v4AudioProcessor();
}
