
namespace bav::plugin
{
template < typename SampleType >
ProcessorInternalEngine< SampleType >::ProcessorInternalEngine (juce::AudioProcessor&      processorToUse,
                                                                State&                     stateToUse,
                                                                dsp::Engine< SampleType >& engineToUse)
    : ParameterProcessorBase< SampleType > (stateToUse.getParameters()),
      processor (processorToUse), state (stateToUse), engine (engineToUse)
{
}

inline int getIndexOfFirstValidChannelSet (const juce::AudioProcessor::BusesLayout& busLayout,
                                           bool                                     isInput)
{
    const auto numBuses = isInput ? busLayout.inputBuses.size() : busLayout.outputBuses.size();

    for (int i = 0; i < numBuses; ++i)
        if (! busLayout.getChannelSet (isInput, i).isDisabled())
            return i;

    return 0;
}

template < typename SampleType >
inline juce::AudioBuffer< SampleType > findSubBuffer (juce::AudioProcessor&                    processor,
                                                      const juce::AudioProcessor::BusesLayout& busLayout,
                                                      juce::AudioBuffer< SampleType >&         origBuffer,
                                                      bool                                     isInput)
{
    return processor.getBusBuffer (origBuffer, isInput,
                                   getIndexOfFirstValidChannelSet (busLayout, isInput));
}

template < typename SampleType >
void ProcessorInternalEngine< SampleType >::renderChunk (juce::AudioBuffer< SampleType >& audio, MidiBuffer& midi)
{
    const auto busLayout = processor.getBusesLayout();
    const auto inBus     = findSubBuffer (processor, busLayout, audio, true);
    auto       outBus    = findSubBuffer (processor, busLayout, audio, false);

    engine.process (inBus, outBus, midi, state.mainBypass->get());
}

template class ProcessorInternalEngine< float >;
template class ProcessorInternalEngine< double >;

}  // namespace bav::plugin
