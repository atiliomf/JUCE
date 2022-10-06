
namespace bav::plugin
{
ProcessorBase::ProcessorBase (State&                                stateToUse,
                              dsp::Engine< float >&                 floatEngineToUse,
                              dsp::Engine< double >&                doubleEngineToUse,
                              juce::AudioProcessor::BusesProperties busesLayout)
    : BasicProcessorBase (busesLayout),
      state (stateToUse),
      floatEngine (*this, state, floatEngineToUse),
      doubleEngine (*this, state, doubleEngineToUse)
{
}

void ProcessorBase::saveEditorSize (int width, int height)
{
    state.setDimensions (width, height);
    repaintEditor();
}

juce::Point< int > ProcessorBase::getSavedEditorSize() const
{
    return {state.dimensions.width, state.dimensions.height};
}

juce::AudioProcessorParameter* ProcessorBase::getBypassParameter() const
{
    return state.mainBypass.get();
}

void ProcessorBase::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    if (isUsingDoublePrecision())
        prepareToPlayInternal (sampleRate, samplesPerBlock, doubleEngine, floatEngine);
    else
        prepareToPlayInternal (sampleRate, samplesPerBlock, floatEngine, doubleEngine);
}

template < typename SampleType1, typename SampleType2 >
void ProcessorBase::prepareToPlayInternal (const double                            sampleRate,
                                           int                                     samplesPerBlock,
                                           ProcessorInternalEngine< SampleType1 >& activeEngine,
                                           ProcessorInternalEngine< SampleType2 >& idleEngine)
{
    if (idleEngine->isInitialized())
        idleEngine->releaseResources();

    jassert (sampleRate > 0 && samplesPerBlock > 0);

    activeEngine->prepare (sampleRate, samplesPerBlock);

    setLatencySamples (activeEngine->reportLatency());
}

void ProcessorBase::releaseResources()
{
    doubleEngine->releaseResources();
    floatEngine->releaseResources();
}

void ProcessorBase::getStateInformation (juce::MemoryBlock& block)
{
    serializing::toBinary (state, block);
}

void ProcessorBase::setStateInformation (const void* data, int size)
{
    serializing::fromBinary (data, size, state);
}

void ProcessorBase::processBlock (juce::AudioBuffer< float >& audio, MidiBuffer& midi)
{
    juce::ScopedNoDenormals nodenorms;

    floatEngine.process (audio, midi);
}

void ProcessorBase::processBlock (juce::AudioBuffer< double >& audio, MidiBuffer& midi)
{
    juce::ScopedNoDenormals nodenorms;

    doubleEngine.process (audio, midi);
}

void ProcessorBase::processBlockBypassed (juce::AudioBuffer< float >& audio, MidiBuffer& midi)
{
    juce::ScopedNoDenormals nodenorms;

    state.mainBypass->set (true);
    floatEngine.process (audio, midi);
}

void ProcessorBase::processBlockBypassed (juce::AudioBuffer< double >& audio, MidiBuffer& midi)
{
    juce::ScopedNoDenormals nodenorms;

    state.mainBypass->set (true);
    doubleEngine.process (audio, midi);
}

}  // namespace bav::plugin
