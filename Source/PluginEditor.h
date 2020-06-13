/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ModDelayAudioProcessorEditor  : public AudioProcessorEditor,
                                      public ComboBox::Listener
{
public:
    ModDelayAudioProcessorEditor (ModDelayAudioProcessor&, AudioProcessorValueTreeState&);
    ~ModDelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
private:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;
    
    Slider depthSlider, rateSlider, feedbackSlider, mixSlider, delaySlider, gainSlider,widthSlider;
    Label depthLabel, rateLabel, feedbackLabel, mixLabel, delayLabel, gainLabel, widthLabel;
    ComboBox effectChoice, lfoChoice;
    
    
    std::unique_ptr<SliderAttachment> depthAttachment;
    std::unique_ptr<SliderAttachment> rateAttachment;
    std::unique_ptr<SliderAttachment> feedbackAttachment;
    std::unique_ptr<SliderAttachment> delayAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;
    std::unique_ptr<SliderAttachment> gainAttachment;
    std::unique_ptr<SliderAttachment> widthAttachment;

    std::unique_ptr<ComboBoxAttachment> effectAttachment;
    std::unique_ptr<ComboBoxAttachment> lfoAttachment;
    
    ModDelayAudioProcessor& processor;
    AudioProcessorValueTreeState& valueTree;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModDelayAudioProcessorEditor)
};
