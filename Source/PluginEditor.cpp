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
    
    
    inputGain.setSliderStyle (juce::Slider::LinearBarVertical);
    inputGain.setRange(0.0, 127.0, 1.0);
    inputGain.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    inputGain.addListener(this);
    inputGainLabel.setText("Gain", juce::dontSendNotification);
    addAndMakeVisible (inputGain);
    addAndMakeVisible (inputGainLabel);
    
    outputLevel.setSliderStyle (juce::Slider::LinearBarVertical);
    outputLevel.setRange(0.0, 127.0, 1.0);
    outputLevel.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    outputLevel.addListener(this);
    outputLevelLabel.setText("Volume", juce::dontSendNotification);
    addAndMakeVisible (outputLevel);
    addAndMakeVisible (outputLevelLabel);
    
    toneSlider.setSliderStyle (juce::Slider::Rotary);
    toneSlider.setRange(220.0, 880.0, 1.0);
    toneSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    toneSlider.addListener(this);
    toneLabel.setText("Tone", juce::dontSendNotification);
    addAndMakeVisible (toneSlider);
    addAndMakeVisible (toneLabel);
    
    setSize (400, 300);
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
    
    inputGain.setBounds(10, 10, 30, 260); // first two coordinates, last two dimensions
    inputGainLabel.setBounds(10,280, getWidth()-110, 20);
    
    outputLevel.setBounds (360, 10, 30, 260);
    outputLevelLabel.setBounds (330, 280, getWidth() - 110, 20);
    
    toneSlider.setBounds(120,80,150,150);
    toneLabel.setBounds(180,250, getWidth()-110, 20);
    
}

void DistortionPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &inputGain)
    {
        audioProcessor.setInputGain(inputGain.getValue());
    }
    else if (slider == &outputLevel)
    {
         audioProcessor.setOutputLevel(outputLevel.getValue());
    }
}