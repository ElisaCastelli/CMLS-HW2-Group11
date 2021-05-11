/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionAudioProcessorEditor::DistortionAudioProcessorEditor (DistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    //GAIN
    gainSlider.setSliderStyle (juce::Slider::Rotary);
    gainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    gainLabel.setText("Gain", juce::dontSendNotification);
    addAndMakeVisible (gainSlider);
    addAndMakeVisible (gainLabel);
    
    //VOLUME
    volumeSlider.setSliderStyle (juce::Slider::Rotary);
    volumeSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    volumeLabel.setText("Volume", juce::dontSendNotification);
    addAndMakeVisible (volumeSlider);
    addAndMakeVisible (volumeLabel);
    
    //TONE
    toneSlider.setSliderStyle (juce::Slider::Rotary);
    toneSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 20);
    toneLabel.setText("Cutoff", juce::dontSendNotification);
    addAndMakeVisible (toneSlider);
    addAndMakeVisible (toneLabel);
    
    //MENU TO CHOOSE DISTORTION TYPE
    
    addAndMakeVisible (distortionTypeMenu);
    distortionTypeMenu.addItem ("Hard clipping",  1);
    distortionTypeMenu.addItem ("Soft clipping Quadratic",   2);
    distortionTypeMenu.addItem ("Soft Clipping Exp", 3);
    distortionTypeMenu.addItem ("Full wave rectifier", 4);
    distortionTypeMenu.addItem ("Half wave rectifier", 5);
 
    
    //attachments to ValueTreeState
    gainSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTree, "gain", gainSlider);
    volumeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTree, "volume", volumeSlider);
    toneSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTree, "tone", toneSlider);
    typeMenuAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.valueTree, "distortionType", distortionTypeMenu);
}

DistortionAudioProcessorEditor::~DistortionAudioProcessorEditor()
{
}

//==============================================================================
void DistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void DistortionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    const int borderX = 20;
    const int borderY = 50;
    
    const int sliderWidth = getWidth()/3 - 2*borderX;
    const int sliderHeight = getHeight() - borderX -100;
    
    gainSlider.setBounds(borderX, borderY, sliderWidth, sliderHeight);
    gainLabel.setBounds(borderX, borderY + 10, 100, 20);
    
    toneSlider.setBounds(getWidth()/3+ borderX, borderY, sliderWidth, sliderHeight);
    toneLabel.setBounds(getWidth()/3+ borderX, borderY + 10, 100, 20);
    
    volumeSlider.setBounds(2*getWidth()/3 + borderX, borderY,  sliderWidth, sliderHeight);
    volumeLabel.setBounds(2*getWidth()/3 + borderX, borderY + 10, 100, 20);
    
    distortionTypeMenu.setBounds(100, 10, 200, 20);
}
