
namespace bav::dsp
{
template < typename SampleType >
void LatencyEngine< SampleType >::prepared (int blocksize, double samplerate)
{
    chunkMidiBuffer.ensureSize (static_cast< size_t > (blocksize));
    inputFIFO.setSize (2, blocksize);
    outputFIFO.setSize (2, blocksize);
    inBuffer.setSize (2, blocksize, true, true, true);
    outBuffer.setSize (2, blocksize, true, true, true);
    onPrepare (blocksize, samplerate);
}

template < typename SampleType >
void LatencyEngine< SampleType >::released()
{
    chunkMidiBuffer.clear();
    inBuffer.setSize (0, 0);
    outBuffer.setSize (0, 0);
    internalBlocksize = 0;
    onRelease();
}

template < typename SampleType >
void LatencyEngine< SampleType >::changeLatency (int newInternalBlocksize)
{
    using Engine = Engine< SampleType >;

    jassert (newInternalBlocksize > 0);

    internalBlocksize = newInternalBlocksize;
    Engine::prepare (Engine::getSamplerate(), internalBlocksize);
}

template < typename SampleType >
void LatencyEngine< SampleType >::renderBlock (const AudioBuffer& input, AudioBuffer& output, MidiBuffer& midiMessages, bool isBypassed)
{
    jassert (internalBlocksize > 0);

    const auto totalNumSamples = input.getNumSamples();

    if (input.getNumChannels() == 0 || output.getNumChannels() == 0)
    {
        renderChunk (input, output, midiMessages, isBypassed);
        return;
    }

    jassert (totalNumSamples == output.getNumSamples());

    if (totalNumSamples == 0)
    {
        renderChunk (inBuffer, outBuffer, midiMessages, isBypassed);
        return;
    }

    inputFIFO.push (input, midiMessages);

    while (inputFIFO.numStoredSamples() >= internalBlocksize)
    {
        AudioBuffer inAlias {inBuffer.getArrayOfWritePointers(),
                             input.getNumChannels(), internalBlocksize};

        AudioBuffer outAlias {outBuffer.getArrayOfWritePointers(),
                              output.getNumChannels(), internalBlocksize};

        inputFIFO.pop (inAlias, chunkMidiBuffer);

        renderChunk (inAlias, outAlias, chunkMidiBuffer, isBypassed);

        outputFIFO.push (outAlias, chunkMidiBuffer);
    }

    outputFIFO.pop (output, midiMessages);
}

template < typename SampleType >
void LatencyEngine< SampleType >::onPrepare (int, double)
{
}

template class LatencyEngine< float >;
template class LatencyEngine< double >;

}  // namespace bav::dsp
