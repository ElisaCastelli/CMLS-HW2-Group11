/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionAudioProcessor::DistortionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), valueTree(*this, nullptr, "Parameters", createParameters()),
                      antiAliasingFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0f, 0.1)),
                      toneFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0f, 0.1))
#endif
{
    oversampling.reset (new juce::dsp::Oversampling<float> (2, log2(oversamplingFactor), juce::dsp::Oversampling<float>::filterHalfBandFIREquiripple, false));
}

DistortionAudioProcessor::~DistortionAudioProcessor()
{
}

//==============================================================================
const juce::String DistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DistortionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DistortionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DistortionAudioProcessor::getProgramName (int index)
{
    return {};
}

void DistortionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    oversampling->reset();
    oversampling->initProcessing (static_cast<size_t> (samplesPerBlock));
    
    lastSampleRate = sampleRate;
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate*oversamplingFactor;
    spec.numChannels = getTotalNumOutputChannels();

    antiAliasingFilter.prepare(spec);
    antiAliasingFilter.reset();
    
    juce::dsp::ProcessSpec specTone;
    specTone.maximumBlockSize = samplesPerBlock;
    specTone.sampleRate = sampleRate;
    specTone.numChannels = getTotalNumOutputChannels();
    
    toneFilter.prepare(specTone);
    toneFilter.reset();
}

void DistortionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DistortionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void DistortionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    //int numSamples = buffer.getNumSamples();
    
    float toneCutoff = *valueTree.getRawParameterValue("toneCutoff");
    float toneResonance = *valueTree.getRawParameterValue("toneResonance");
    float inputGain = *valueTree.getRawParameterValue("gain");
    float outputLevel = *valueTree.getRawParameterValue("volume");
    int distortionType = *valueTree.getRawParameterValue("distortionType");
    
    float gainDB = pow(10, inputGain / 20);
    float volumeDB = pow(10, outputLevel / 20);
    
    juce::dsp::AudioBlock<float> block(buffer);
    
    juce::dsp::AudioBlock<float> blockOversampled = oversampling->processSamplesUp(block); 
    
    for (int channel = 0; channel < blockOversampled.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
       
        // ..do something to the data...
        
        for (int i = 0; i < blockOversampled.getNumSamples(); ++i) {
            float in = blockOversampled.getSample(channel, i);
            
            float out;
            in *= gainDB;
            if(distortionType == 1){
                float threshold = 1.0f;
                if(in > threshold){
                    out = threshold;
                }
                else if(in < -threshold){
                    out = -threshold;
                }
                else{
                    out = in;
                }
            }
            else if(distortionType == 2){
                float threshold1 = 1.0f/3.0f;
                float threshold2 = 2.0f/3.0f;
                if(in > threshold2){
                    out = 1.0f;
                }
                else if(in > threshold1){
                    out = (3.0f - (2.0f - 3.0f*in) * (2.0f - 3.0f*in))/3.0f;
                }
                else if(in < -threshold2){
                    out = -1.0f;
                }
                else if(in < -threshold1){
                    out = -(3.0f - (2.0f + 3.0f*in) * (2.0f + 3.0f*in))/3.0f;
                }
                else{
                    out = 2.0f* in;
                }
            }else if(distortionType == 3){
                if(in > 0)
                    out = 1.0f - expf(-in);
                else
                    out = -1.0f + expf(in);
            }else if(distortionType == 4){
                    out = fabsf(in);
            }else if(distortionType == 5){
                if(in > 0)
                    out = in;
                else
                    out = 0;
            }
            out *= volumeDB;
            blockOversampled.setSample(channel, i, out);
            
        }
        
    }
     
    *antiAliasingFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate*oversamplingFactor, lastSampleRate/2.0f);
    
    antiAliasingFilter.process(juce::dsp::ProcessContextReplacing<float> (blockOversampled));
    
    oversampling->processSamplesDown(block); 
    
   
    *toneFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, toneCutoff, toneResonance);
    
    toneFilter.process(juce::dsp::ProcessContextReplacing<float> (block));
     
}

//==============================================================================
bool DistortionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DistortionAudioProcessor::createEditor()
{
    return new DistortionAudioProcessorEditor (*this);
}

//==============================================================================
void DistortionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DistortionAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout DistortionAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;
     
    parameters.push_back (std::make_unique<juce::AudioParameterFloat> ("gain", "Input Gain", 0.0f, 40.0f, 10.0f));
    
    parameters.push_back (std::make_unique<juce::AudioParameterFloat> ("volume", "Output level", -20.0f, 20.0f, 0.0f));

    parameters.push_back (std::make_unique<juce::AudioParameterFloat> ("toneCutoff", "Tone Cutoff", 20.0f, 20000.0f, 4000.0f));
    
    parameters.push_back (std::make_unique<juce::AudioParameterFloat> ("toneResonance", "Tone Resonance", 0.1f, 1.0f, 0.5f)); 
    
    parameters.push_back (std::make_unique<juce::AudioParameterInt> ("distortionType", "Distortion Type", 1, 5, 1));

    // we return                         
    return { parameters.begin(), parameters.end() };
}