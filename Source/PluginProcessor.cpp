/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ModDelayAudioProcessor::ModDelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr, "PARAMS",
                                     {
                           std::make_unique<AudioParameterFloat>("depth",
                                                                 "Depth",
                                                                 0.0,
                                                                 100.0,
                                                                 50.0),
                           std::make_unique<AudioParameterFloat>("rate",
                                                                 "Rate",
                                                                 0.02,
                                                                 5,
                                                                 0.18),
                           std::make_unique<AudioParameterFloat>("feedback",
                                                                 "FeedBack",
                                                                 0.0,
                                                                 100.0,
                                                                 0.0),
                           std::make_unique<AudioParameterChoice>("modulation",
                                                                  "ModType",
                                                                  StringArray("Flanger",
                                                                              "Vibrato",
                                                                              "Chorus"),
                                                                  0),
                           std::make_unique<AudioParameterChoice>("lfo",
                                                                  "LFO",
                                                                  StringArray("Triangle",
                                                                              "Sine"),
                                                                  0)
                       })
#endif
{
    depthSliderValue = parameters.getRawParameterValue("depth");
    rateSliderValue = parameters.getRawParameterValue("rate");
    feedbackSliderValue = parameters.getRawParameterValue("feedback");
    modTypeValue = parameters.getRawParameterValue("modulation");
    lfoTypeValue = parameters.getRawParameterValue("lfo");
    
    mMinDelay = 0.0;
    mMaxDelay = 0.0;
    
}

ModDelayAudioProcessor::~ModDelayAudioProcessor()
{
}

//==============================================================================
const String ModDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ModDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ModDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ModDelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ModDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ModDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ModDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ModDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String ModDelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void ModDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}
void ModDelayAudioProcessor::setModType()
{
    switch ((int)*modTypeValue) {
        case Flanger:
            mMinDelay = 0.1;
            mMaxDelay = 7.0;
            mDL.wetRawValue = 90.0;
            mDL.feedbackRawValue = *feedbackSliderValue;
            break;
        case Vibrato:
            mMinDelay = 0.0;
            mMaxDelay = 7.0;
            mDL.wetRawValue = 100.0;
            mDL.feedbackRawValue = 0.0;
            break;
        case Chorus:
            mMinDelay = 5.0;
            mMaxDelay = 30.0;
            mDL.feedbackRawValue = *feedbackSliderValue;
            mDL.wetRawValue = 50.0;
            break;
        default:
            mMinDelay = 0.1;
            mMaxDelay = 7.0;
            mDL.wetRawValue = 50.0;
            mDL.feedbackRawValue = *feedbackSliderValue;
            break;
    }
}
//==============================================================================
void ModDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mLFO.resetIndex();
    
    mDL.setSize(sampleRate);
    mDL.resetDelay();
    
    setModType();
    mLFO.updateInc(*rateSliderValue, (float)sampleRate);
    mLFO.setOscType(OSCTYPE::SINE);
    mDL.calculateVariables();
    
}

void ModDelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ModDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

float ModDelayAudioProcessor::setDelayOffset(float lfoValue)
{
    if((int)*modTypeValue==Flanger || (int)*modTypeValue==Vibrato)
        return (*depthSliderValue/100.0)*(lfoValue*(mMaxDelay-mMinDelay)+mMinDelay);
    else if ((int)*modTypeValue==Chorus)
    {
        float start = mMinDelay + 2.0; //fixed chorus offset
        return (*depthSliderValue/100.0)*(lfoValue*(mMaxDelay-mMinDelay)+start);
    }
    return 0.0;
}

void ModDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    auto* inputBuffer = buffer.getReadPointer(0);
    auto* outBuffer = buffer.getArrayOfWritePointers();
    
    setModType();
    mLFO.updateInc(*rateSliderValue, (float)getSampleRate());
    mLFO.setOscType(OSCTYPE::SINE);
    for (int samp=0; samp<buffer.getNumSamples(); ++samp) {
        float outSamp = inputBuffer[samp];
        
        float lfoValue = 0.0;
        mLFO.oscillate(&lfoValue);
        float delay = setDelayOffset(lfoValue);
        
        mDL.delayMs = delay;
        mDL.calculateVariables();
        mDL.delayLineProcessor(&outSamp);
        
        outBuffer[0][samp] = outSamp;
        outBuffer[1][samp] = outSamp;
        
    }
        

}

//==============================================================================
bool ModDelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ModDelayAudioProcessor::createEditor()
{
    return new ModDelayAudioProcessorEditor (*this,parameters);
}

//==============================================================================
void ModDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ModDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ModDelayAudioProcessor();
}
