/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DDL.h"
#include "WTOsc.h"

//==============================================================================
/**
*/
class ModDelayAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ModDelayAudioProcessor();
    ~ModDelayAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    enum MODTYPE{Flanger,Vibrato,Chorus};
    AudioProcessorValueTreeState parameters;
    void setModType(); 
    float setDelayOffset(float lfoValue);
    
    WTOsc mLFO; //lfo
    Ddl mDLL, mDLR; //left and right delay lines
    float mMinDelay, mMaxDelay;
    
    std::atomic<float>* depthSliderValue, *rateSliderValue, *feedbackSliderValue;
    std::atomic<float>* delaySliderValue, *mixSliderValue, *gainSliderValue, *widthSliderValue;
    std::atomic<float>* modTypeValue, *lfoTypeValue;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModDelayAudioProcessor)
};
