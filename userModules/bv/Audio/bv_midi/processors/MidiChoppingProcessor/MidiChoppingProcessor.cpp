
namespace bav::dsp
{
template < typename SampleType >
MidiChoppingProcessor< SampleType >::MidiChoppingProcessor()
{
    midiStorage.ensureSize (size_t (512));
}

template < typename SampleType >
void MidiChoppingProcessor< SampleType >::process (juce::AudioBuffer< SampleType >& audio, MidiBuffer& midi)
{
    auto samplesLeft = audio.getNumSamples();

    if (samplesLeft == 0 || audio.getNumChannels() == 0)
    {
        processInternal (audio, midi, 0, 0);
        return;
    }

    auto midiIterator = midi.findNextSamplePosition (0);
    int  startSample  = 0;

    for (; samplesLeft > 0; ++midiIterator)
    {
        if (midiIterator == midi.cend())
        {
            processInternal (audio, midi, startSample, samplesLeft);
            return;
        }

        const auto metadata                 = *midiIterator;
        const auto samplesToNextMidiMessage = metadata.samplePosition - startSample;
        const auto nextMidiMessage          = metadata.getMessage();

        if (samplesToNextMidiMessage >= samplesLeft)
        {
            processInternal (audio, midi, startSample, samplesLeft);
            handleMidiMessage (nextMidiMessage);
            break;
        }

        if (samplesToNextMidiMessage == 0)
        {
            handleMidiMessage (nextMidiMessage);
            continue;
        }

        handleMidiMessage (nextMidiMessage);
        processInternal (audio, midi, startSample, samplesToNextMidiMessage);

        startSample += samplesToNextMidiMessage;
        samplesLeft -= samplesToNextMidiMessage;
    }

    std::for_each (
        midiIterator, midi.cend(), [&] (const juce::MidiMessageMetadata& meta)
        { handleMidiMessage (meta.getMessage()); });
}

template < typename SampleType >
void MidiChoppingProcessor< SampleType >::processInternal (juce::AudioBuffer< SampleType >& audio, MidiBuffer& midi,
                                                           int startSample, int numSamples)
{
    juce::AudioBuffer< SampleType > alias {audio.getArrayOfWritePointers(),
                                           audio.getNumChannels(),
                                           startSample,
                                           numSamples};

    midi::copyRangeOfMidiBuffer (midi, midiStorage, startSample, 0, numSamples);

    renderChunk (alias, midiStorage);

    midi::copyRangeOfMidiBuffer (midiStorage, midi, 0, startSample, numSamples);
}


template class MidiChoppingProcessor< float >;
template class MidiChoppingProcessor< double >;

}  // namespace bav::dsp
