/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DistortionPluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
enum RadioButtonIds
    {
        typeButtons = 1001
    };
    DistortionPluginAudioProcessorEditor (DistortionPluginAudioProcessor&);
    ~DistortionPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void updateToggleState (juce::Button* button, int type);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionPluginAudioProcessor& audioProcessor;

    juce::Slider inputGainSlider;
    juce::Label inputGainLabel;
    
    juce::Slider outputLevelSlider;
    juce::Label outputLevelLabel;
    
    juce::Slider toneSlider;
    juce::Label toneLabel;
    
    juce::Label typeLabel         { {}, "Choose one distortion type:"};
    juce::ToggleButton hard1   { "Hard Clipping" },
                       soft1 { "Soft Clipping Quadratic" },
                       soft2 {"Soft Clipping Exp"},
                       rect {"Full wave rectifier"},
                       halfrect{"Half wave rectifier"};
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputLevelSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneSliderAttachment;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionPluginAudioProcessorEditor)
};
