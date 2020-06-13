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
    // set and initiliaze sliders and labels
    
    //depth
    addAndMakeVisible(depthSlider);
    depthSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    depthSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    depthSlider.setTextValueSuffix(" %");
    depthSlider.setNumDecimalPlacesToDisplay(1);
    depthSlider.setPopupDisplayEnabled(true, false, this);
    depthAttachment.reset(new SliderAttachment(valueTree,"depth",depthSlider));
    addAndMakeVisible(depthLabel);
    depthLabel.setText("Depth", dontSendNotification);
    depthLabel.setFont(Font("Papyrus", 25.0, Font::FontStyleFlags::bold));
    depthLabel.attachToComponent(&depthSlider, false);
    depthLabel.setJustificationType(Justification::centred);
    
    //rate
    addAndMakeVisible(rateSlider);
    rateSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    rateSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    rateSlider.setNumDecimalPlacesToDisplay(1);
    rateSlider.setPopupDisplayEnabled(true, false, this);
    rateAttachment.reset(new SliderAttachment(valueTree,"rate",rateSlider));
    addAndMakeVisible(rateLabel);
    rateLabel.setText("Rate", dontSendNotification);
    rateLabel.setFont(Font("Papyrus", 25.0, Font::FontStyleFlags::bold));
    rateLabel.attachToComponent(&rateSlider, false);
    rateLabel.setJustificationType(Justification::centred);

    //feedback
    addAndMakeVisible(feedbackSlider);
    feedbackSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    feedbackSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    feedbackSlider.setTextValueSuffix(" %");
    feedbackSlider.setNumDecimalPlacesToDisplay(1);
    feedbackSlider.setPopupDisplayEnabled(true, false, this);
    feedbackAttachment.reset(new SliderAttachment(valueTree,"feedback",feedbackSlider));
    addAndMakeVisible(feedbackLabel);
    feedbackLabel.setText("Feedback", dontSendNotification);
    feedbackLabel.setFont(Font("Papyrus", 25.0, Font::FontStyleFlags::bold));
    feedbackLabel.attachToComponent(&feedbackSlider, false);
    feedbackLabel.setJustificationType(Justification::centred);
    
    //delay
    addAndMakeVisible(delaySlider);
    delaySlider.setSliderStyle(Slider::RotaryVerticalDrag);
    delaySlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    delaySlider.setTextValueSuffix(" ms");
    delaySlider.setNumDecimalPlacesToDisplay(1);
    delaySlider.setPopupDisplayEnabled(true, false, this);
    delayAttachment.reset(new SliderAttachment(valueTree,"delay",delaySlider));
    addAndMakeVisible(delaySlider);
    delayLabel.setText("Delay", dontSendNotification);
    delayLabel.setFont(Font("Papyrus", 25.0, Font::FontStyleFlags::bold));
    delayLabel.attachToComponent(&delaySlider, false);
    delayLabel.setJustificationType(Justification::centred);
    
    //wet/dry
    addAndMakeVisible(mixSlider);
    mixSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    mixSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    mixSlider.setTextValueSuffix(" %");
    mixSlider.setNumDecimalPlacesToDisplay(1);
    mixSlider.setPopupDisplayEnabled(true, false, this);
    mixAttachment.reset(new SliderAttachment(valueTree,"mix",mixSlider));
    addAndMakeVisible(mixLabel);
    mixLabel.setText("Mix", dontSendNotification);
    mixLabel.setFont(Font("Papyrus", 25.0, Font::FontStyleFlags::bold));
    mixLabel.attachToComponent(&mixSlider, false);
    mixLabel.setJustificationType(Justification::centred);
    
    //gain
    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    gainSlider.setTextValueSuffix(" dB");
    gainSlider.setNumDecimalPlacesToDisplay(1);
    gainSlider.setPopupDisplayEnabled(true, false, this);
    gainAttachment.reset(new SliderAttachment(valueTree,"gain",gainSlider));
    addAndMakeVisible(gainLabel);
    gainLabel.setText("Output", dontSendNotification);
    gainLabel.setFont(Font("Papyrus", 25.0, Font::FontStyleFlags::bold));
    gainLabel.attachToComponent(&gainSlider, false);
    gainLabel.setJustificationType(Justification::centred);
    
    //stereo width
    addAndMakeVisible(widthSlider);
    widthSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    widthSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    widthSlider.setTextValueSuffix(" %");
    widthSlider.setNumDecimalPlacesToDisplay(1);
    widthSlider.setPopupDisplayEnabled(true, false, this);
    widthAttachment.reset(new SliderAttachment(valueTree,"width",widthSlider));
    addAndMakeVisible(widthLabel);
    widthLabel.setText("Width", dontSendNotification);
    widthLabel.setFont(Font("Papyrus", 25.0, Font::FontStyleFlags::bold));
    widthLabel.attachToComponent(&widthSlider, false);
    widthLabel.setJustificationType(Justification::centred);

    //ComboBox for type of effect
    addAndMakeVisible(effectChoice);
    effectChoice.addItemList(StringArray("Flanger",
                                         "Vibrato",
                                         "Chorus"),
                             1);
    effectChoice.addListener(this);
    effectChoice.setJustificationType(Justification::centred);
    effectAttachment.reset(new ComboBoxAttachment(valueTree,"modulation",effectChoice));
    
    //ComboBox for type of LFO. Only Sine and Triangle
    addAndMakeVisible(lfoChoice);
    lfoChoice.addItemList(StringArray("Triangle",
                                         "Sine"),
                             1);
    lfoChoice.addListener(this);
    lfoChoice.setJustificationType(Justification::centred);
    lfoAttachment.reset(new ComboBoxAttachment(valueTree,"lfo",lfoChoice));
    
    //changing colour of sliders
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::cornflowerblue);
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::lightcyan);
    setSize (600, 540);
}

ModDelayAudioProcessorEditor::~ModDelayAudioProcessorEditor()
{
}

//==============================================================================
void ModDelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::slategrey);


}

void ModDelayAudioProcessorEditor::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
    //Only show feedback slider for Flanger. No feedback for Chorus and Vibrato
    if(comboBoxThatHasChanged == &effectChoice && comboBoxThatHasChanged->getItemText(comboBoxThatHasChanged->getSelectedItemIndex())=="Flanger")
    {
        addAndMakeVisible(feedbackSlider);
        addAndMakeVisible(feedbackLabel);
    }
    else if(comboBoxThatHasChanged == &effectChoice)
    {
        removeChildComponent(&feedbackSlider);
        removeChildComponent(&feedbackLabel);
    }
}

void ModDelayAudioProcessorEditor::resized()
{
    //set size and position of components
    depthSlider.setBounds(0, getHeight()/3, getWidth()/4, getWidth()/4);
    rateSlider.setBounds(getWidth()/4, getHeight()/3, getWidth()/4, getWidth()/4);
    delaySlider.setBounds(getWidth()/4+150, getHeight()/3, getWidth()/4, getWidth()/4);
    feedbackSlider.setBounds(getWidth()/4+300, getHeight()/3, getWidth()/4, getWidth()/4);
    mixSlider.setBounds(getWidth()-240, getHeight()-140, 100, 100);
    widthSlider.setBounds(getWidth()-360, getHeight()-140, 100, 100);
    gainSlider.setBounds(getWidth()-480, getHeight()-140, 100, 100);
    effectChoice.setBounds(getWidth()/5, 80, 110, 30);
    lfoChoice.setBounds(getWidth()/5+240, 80, 110, 30);
}
