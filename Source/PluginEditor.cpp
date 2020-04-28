/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ModDelayAudioProcessorEditor::ModDelayAudioProcessorEditor (ModDelayAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), valueTree(vts)
{
    addAndMakeVisible(depthSlider);
    depthSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    depthSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    depthSlider.setNumDecimalPlacesToDisplay(1);
    depthSlider.setPopupDisplayEnabled(true, false, this);
    depthAttachment.reset(new SliderAttachment(valueTree,"depth",depthSlider));
    addAndMakeVisible(depthLabel);
    depthLabel.setText("Depth", dontSendNotification);
    depthLabel.attachToComponent(&depthSlider, false);
    depthLabel.setJustificationType(Justification::centred);
    
    addAndMakeVisible(rateSlider);
    rateSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    rateSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    rateSlider.setNumDecimalPlacesToDisplay(1);
    rateSlider.setPopupDisplayEnabled(true, false, this);
    rateAttachment.reset(new SliderAttachment(valueTree,"rate",rateSlider));
    addAndMakeVisible(rateLabel);
    rateLabel.setText("Rate", dontSendNotification);
    rateLabel.attachToComponent(&rateSlider, false);
    rateLabel.setJustificationType(Justification::centred);

    addAndMakeVisible(feedbackSlider);
    feedbackSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    feedbackSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    feedbackSlider.setNumDecimalPlacesToDisplay(1);
    feedbackSlider.setPopupDisplayEnabled(true, false, this);
    feedbackAttachment.reset(new SliderAttachment(valueTree,"feedback",feedbackSlider));
    addAndMakeVisible(feedbackLabel);
    feedbackLabel.setText("Feedback", dontSendNotification);
    feedbackLabel.attachToComponent(&feedbackSlider, false);
    feedbackLabel.setJustificationType(Justification::centred);

    addAndMakeVisible(effectChoice);
    effectChoice.addItemList(StringArray("Flanger",
                                         "Vibrato",
                                         "Chorus"),
                             1);
    effectChoice.setJustificationType(Justification::centred);
    effectAttachment.reset(new ComboBoxAttachment(valueTree,"modulation",effectChoice));

    addAndMakeVisible(lfoChoice);
    lfoChoice.addItemList(StringArray("Triangle",
                                         "Sine"),
                             1);
    lfoChoice.setJustificationType(Justification::centred);
    lfoAttachment.reset(new ComboBoxAttachment(valueTree,"lfo",lfoChoice));
    
    setSize (600, 500);
}

ModDelayAudioProcessorEditor::~ModDelayAudioProcessorEditor()
{
}

//==============================================================================
void ModDelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::rebeccapurple);


}

void ModDelayAudioProcessorEditor::resized()
{
    depthSlider.setBounds(40, 200, 120, 120);
    rateSlider.setBounds(170, 200, 120, 120);
    feedbackSlider.setBounds(300, 200, 120, 120);
    effectChoice.setBounds(130, 80, 110, 30);
    lfoChoice.setBounds(360, 80, 110, 30);
}
