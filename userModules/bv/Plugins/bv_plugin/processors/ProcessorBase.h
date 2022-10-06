
#pragma once

#include "InternalEngine.h"

namespace bav::plugin
{
class ProcessorBase : public dsp::BasicProcessorBase, private SystemInitializer
{
public:
    ProcessorBase (State&                                stateToUse,
                   dsp::Engine< float >&                 floatEngineToUse,
                   dsp::Engine< double >&                doubleEngineToUse,
                   juce::AudioProcessor::BusesProperties busesLayout);

    void               saveEditorSize (int width, int height);
    juce::Point< int > getSavedEditorSize() const;

private:
    juce::AudioProcessorParameter* getBypassParameter() const final;

    void getStateInformation (juce::MemoryBlock& block) final;
    void setStateInformation (const void* data, int size) final;

    void prepareToPlay (double sampleRate, int samplesPerBlock) final;

    template < typename SampleType1, typename SampleType2 >
    void prepareToPlayInternal (const double sampleRate, int samplesPerBlock,
                                ProcessorInternalEngine< SampleType1 >& activeEngine,
                                ProcessorInternalEngine< SampleType2 >& idleEngine);

    void releaseResources() final;

    bool supportsDoublePrecisionProcessing() const final { return true; }

    void processBlock (juce::AudioBuffer< float >& audio, MidiBuffer& midi) final;
    void processBlock (juce::AudioBuffer< double >& audio, MidiBuffer& midi) final;
    void processBlockBypassed (juce::AudioBuffer< float >& audio, MidiBuffer& midi) final;
    void processBlockBypassed (juce::AudioBuffer< double >& audio, MidiBuffer& midi) final;

    State& state;

    ProcessorInternalEngine< float >  floatEngine;
    ProcessorInternalEngine< double > doubleEngine;
};

}  // namespace bav::plugin
