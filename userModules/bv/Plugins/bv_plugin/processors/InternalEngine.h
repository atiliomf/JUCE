#pragma once

namespace bav::plugin
{
template < typename SampleType >
class ProcessorInternalEngine : public ParameterProcessorBase< SampleType >
{
public:
    ProcessorInternalEngine (juce::AudioProcessor&      processorToUse,
                             State&                     stateToUse,
                             dsp::Engine< SampleType >& engineToUse);

    dsp::Engine< SampleType >* operator->() { return &engine; }

private:
    void renderChunk (juce::AudioBuffer< SampleType >& audio, MidiBuffer& midi) final;

    juce::AudioProcessor&      processor;
    State&                     state;
    dsp::Engine< SampleType >& engine;
};

}  // namespace bav::plugin
