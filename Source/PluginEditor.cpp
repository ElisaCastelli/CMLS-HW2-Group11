/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionPluginAudioProcessorEditor::DistortionPluginAudioProcessorEditor (DistortionPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    
    inputGainSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    inputGainSlider.setRange(0.0, 40.0, 1.0);
    inputGainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    //inputGainSlider.addListener(this);
    inputGainLabel.setText("Gain", juce::dontSendNotification);
    addAndMakeVisible (inputGainSlider);
    addAndMakeVisible (inputGainLabel);
    
    outputLevelSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    outputLevelSlider.setRange(0.0, 100.0, 1.0);
    outputLevelSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    //outputLevelSlider.addListener(this);
    outputLevelLabel.setText("Volume", juce::dontSendNotification);
    addAndMakeVisible (outputLevelSlider);
    addAndMakeVisible (outputLevelLabel);
    
    toneSlider.setSliderStyle (juce::Slider::Rotary);
    toneSlider.setRange(220.0, 880.0, 1.0);
    toneSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    //toneSlider.addListener(this);
    toneLabel.setText("Tone", juce::dontSendNotification);
    addAndMakeVisible (toneSlider);
    addAndMakeVisible (toneLabel);
    
    //ToggleButtons
    addAndMakeVisible(typeLabel);
    addAndMakeVisible (hard1);
    addAndMakeVisible (soft1);
    addAndMakeVisible (soft2);
    addAndMakeVisible (rect);
    addAndMakeVisible (halfrect);
    
    hard1.onClick = [this] { updateToggleState (&hard1,   1);   };
    soft1.onClick = [this] { updateToggleState (&soft1, 2); };
    soft2.onClick = [this] { updateToggleState (&soft2,   3);   };
    rect.onClick = [this] { updateToggleState (&rect, 4); };
    halfrect.onClick = [this] { updateToggleState (&halfrect, 5); };

    hard1.setRadioGroupId (typeButtons);
    soft1.setRadioGroupId (typeButtons);
    soft2.setRadioGroupId (typeButtons);
    rect.setRadioGroupId (typeButtons);
    halfrect.setRadioGroupId (typeButtons);
    
    
    inputGainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", inputGainSlider);
    outputLevelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOLUME", outputLevelSlider);
    toneSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TONE", toneSlider);
    
    
    setSize (500, 400);
}

DistortionPluginAudioProcessorEditor::~DistortionPluginAudioProcessorEditor()
{
}

//==============================================================================
void DistortionPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Distortion", getLocalBounds(), juce::Justification::centred, 1);
}

void DistortionPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    inputGainSlider.setBounds(30, 90, 30, 260); // first two coordinates, last two dimensions
    inputGainLabel.setBounds(30, 360, getWidth()-110, 20);
    
    outputLevelSlider.setBounds (440, 90, 30, 260);
    outputLevelLabel.setBounds (430, 360, getWidth() - 110, 20);
    
    toneSlider.setBounds(180, 120, 150, 150);
    toneLabel.setBounds(200, 290, getWidth()-110, 20);
    
    hard1.setBounds (10, 40, getWidth() - 20, 20);
    soft1  .setBounds (140, 40, getWidth() - 20, 20);
    soft2.setBounds (330, 40, getWidth() - 20, 20);

    rect.setBounds (100, 70, getWidth() - 20, 20);
    halfrect .setBounds (240, 70, getWidth() - 30, 20);
    
    typeLabel.setBounds(160, 8, getWidth()-110, 20);
    
}

void DistortionPluginAudioProcessorEditor::updateToggleState (juce::Button* button, int type)
{
        auto state = button->getToggleState();
        juce::String stateString = state ? "ON" : "OFF";

        //juce::Logger::outputDebugString (name + " Button changed to " + stateString);
        audioProcessor.setTypeSelected(type);
}
