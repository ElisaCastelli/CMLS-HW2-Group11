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
    inputGainSlider.setRange(0.0, 127.0, 1.0);
    inputGainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    //inputGainSlider.addListener(this);
    inputGainLabel.setText("Gain", juce::dontSendNotification);
    addAndMakeVisible (inputGainSlider);
    addAndMakeVisible (inputGainLabel);
    
    outputLevelSlider.setSliderStyle (juce::Slider::LinearBarVertical);
    outputLevelSlider.setRange(0.0, 127.0, 1.0);
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
    
    inputGainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", inputGainSlider);
    outputLevelSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VOLUME", outputLevelSlider);
    toneSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "TONE", toneSlider);
    
    
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
    
    inputGainSlider.setBounds(10, 10, 30, 260); // first two coordinates, last two dimensions
    inputGainLabel.setBounds(10, 280, getWidth()-110, 20);
    
    outputLevelSlider.setBounds (360, 10, 30, 260);
    outputLevelLabel.setBounds (330, 280, getWidth() - 110, 20);
    
    toneSlider.setBounds(120, 80, 150, 150);
    toneLabel.setBounds(180, 250, getWidth()-110, 20);
    
}

/*void DistortionPluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &inputGain)
    {
        audioProcessor.setInputGain(inputGain.getValue());
    }
    else if (slider == &outputLevel)
    {
         audioProcessor.setOutputLevel(outputLevel.getValue());
    }
    else if(slider == &toneSlider)
    {
        audioProcessor.setTone(toneSlider.getValue());
    }
}*/