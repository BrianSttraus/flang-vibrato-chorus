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
                           //add valuetree parameters to attach to sliders
                           std::make_unique<AudioParameterFloat>("depth",
                                                                 "Depth",
                                                                 0.0,
                                                                 100.0,
                                                                 50.0),
                           std::make_unique<AudioParameterFloat>("rate",
                                                                 "Rate",
                                                                 0,
                                                                 5,
                                                                 0.6),
                           std::make_unique<AudioParameterFloat>("feedback",
                                                                 "FeedBack",
                                                                 0.0,
                                                                 100.0,
                                                                 0.0),
                           std::make_unique<AudioParameterFloat>("mix",
                                                                 "Mix",
                                                                 0.0,
                                                                 100.0,
                                                                 50.0),
                           std::make_unique<AudioParameterFloat>("delay",
                                                                 "Delay",
                                                                 0.0,
                                                                 7.0,
                                                                 0.8),
                           std::make_unique<AudioParameterFloat>("gain",
                                                                 "Gain",
                                                                 -10.0,
                                                                 6.0,
                                                                 0.0),
                           std::make_unique<AudioParameterFloat>("width",
                                                                 "Width",
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
    //get raw parameters. Dereference later to use values
    depthSliderValue = parameters.getRawParameterValue("depth");
    rateSliderValue = parameters.getRawParameterValue("rate");
    feedbackSliderValue = parameters.getRawParameterValue("feedback");
    mixSliderValue = parameters.getRawParameterValue("mix");
    gainSliderValue = parameters.getRawParameterValue("gain");
    widthSliderValue = parameters.getRawParameterValue("width");
    delaySliderValue = parameters.getRawParameterValue("delay");
    modTypeValue = parameters.getRawParameterValue("modulation");
    lfoTypeValue = parameters.getRawParameterValue("lfo");
    //min and max values of modulation range
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
    //set variables to use for each type of effect
    //delay slider sets start of modulation up to a fixed max
    switch ((int)*modTypeValue) {
        case Flanger:
            mMinDelay = *delaySliderValue;
            mMaxDelay = 10.0;
            mDLL.feedbackRawValue = *feedbackSliderValue; //slider controls raw percentage value inside delay line
            mDLR.feedbackRawValue = *feedbackSliderValue;
            break;
        case Vibrato:
            mMinDelay = *delaySliderValue;
            mMaxDelay = 10.0;
            mDLL.feedbackRawValue = 0.0; //no feedback for vibrato!
            mDLR.feedbackRawValue = 0.0;
            break;
        case Chorus:
            mMinDelay = *delaySliderValue;
            mMaxDelay = 30.0;
            mDLL.feedbackRawValue = 0.0; //no feedback for chorus!
            mDLR.feedbackRawValue = 0.0;
            break;
        default:
            //default Flanger
            mMinDelay = 0.1;
            mMaxDelay = 7.0;
            mDLL.feedbackRawValue = *feedbackSliderValue;
            mDLR.feedbackRawValue = *feedbackSliderValue;
            break;
    }
}
//==============================================================================
void ModDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    //reset read indexes of lfo
    mLFO.resetIndex();
    
    //set size of left and right delay lines. Clear buffers
    mDLL.setSize(sampleRate);
    mDLL.resetDelay();
    mDLR.setSize(sampleRate);
    mDLR.resetDelay();
    
    //set all variables depending on effect
    setModType();
    //set frequency of lfo
    mLFO.updateInc(*rateSliderValue, (float)sampleRate);
    //select Sine or Triangle with ComboBox
    mLFO.setOscType((int)*lfoTypeValue);
    //set raw wet/dry value inside delay lines
    mDLL.wetRawValue = *mixSliderValue;
    mDLR.wetRawValue = *mixSliderValue;
    //calculate with raw values
    mDLL.calculateVariables();
    mDLR.calculateVariables();
    
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
    //get delay time in msec by mapping lfovalue to modulation range. Scale by depth
    if((int)*modTypeValue==Flanger || (int)*modTypeValue==Vibrato)
        return (*depthSliderValue/100.0)*(lfoValue*(mMaxDelay-mMinDelay)+mMinDelay);
    else if ((int)*modTypeValue==Chorus)
    {
        //for chorus offset initial delay time
        float start = mMinDelay + 20.0; //fixed chorus offset
        return (*depthSliderValue/100.0)*(lfoValue*(mMaxDelay-mMinDelay)+start);
    }
    return 0;
}

void ModDelayAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //get input and output buffers
    auto* inputBuffer = buffer.getArrayOfReadPointers();
    auto* outBuffer = buffer.getArrayOfWritePointers();
    
    //update variables in real time
    setModType();
    mDLL.wetRawValue = *mixSliderValue;
    mDLR.wetRawValue = *mixSliderValue;
    mLFO.updateInc(*rateSliderValue, (float)getSampleRate());
    mLFO.setOscType((int)*lfoTypeValue);
    
    
    for (int samp=0; samp<buffer.getNumSamples(); ++samp) {
        //get left and right input samples
        float outSampL = inputBuffer[0][samp];
        float outSampR = outSampL;
        
        if(totalNumInputChannels==2)
            outSampR = inputBuffer[1][samp];
        
        float lfoValue = 0.0;
        float lfoPhaseValue = 0.0;
        //calculate lfo position of read pointers. Offset read pointer used for rigth channel
        mLFO.oscillate(&lfoValue,&lfoPhaseValue);
        //get delay in msec
        float delay = setDelayOffset(lfoValue);
        float delayR = setDelayOffset(lfoPhaseValue);
        
        //width slider increments delay offset from 32 samples up to 256 samples
        delayR += (((*widthSliderValue/100.0)*224.0)*1000.0)/getSampleRate();
        //set delay amount for left and right channels
        mDLL.delayMs = delay;
        mDLR.delayMs = delayR;
        //recalculate variables and process the input samples in delay line
        mDLL.calculateVariables();
        mDLR.calculateVariables();
        mDLL.delayLineProcessor(&outSampL);
        mDLR.delayLineProcessor(&outSampR);
        
        float gain = pow(10.0, *gainSliderValue/20.0);
        //output samples and scale by gain
        outBuffer[0][samp] = outSampL*gain;
        
        if(totalNumOutputChannels==2)
            outBuffer[1][samp] = outSampR*gain;
        
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
