
namespace bav::dsp
{
template < typename SampleType >
void CircularBuffer< SampleType >::resize (int blocksize, int blocksizeMultipleToAllocate)
{
    const auto newSize = blocksize * blocksizeMultipleToAllocate;
    buffer.setSize (1, newSize, true, true, true);

    lastFrameStart %= newSize;
    lastFrameEnd %= newSize;
}

template < typename SampleType >
void CircularBuffer< SampleType >::storeSamples (const AudioBuffer& samples, int channel)
{
    storeSamples (samples.getReadPointer (channel), samples.getNumSamples());
}

template < typename SampleType >
void CircularBuffer< SampleType >::storeSamples (const SampleType* samples, int numSamples)
{
    const auto bufferSize = getCapacity();

    jassert (numSamples > 0 && numSamples <= bufferSize);
    lastFrameSize = numSamples;

    auto newStart = lastFrameEnd + 1;
    if (newStart > bufferSize) newStart -= bufferSize;

    lastFrameStart = newStart;

    if (newStart + numSamples > bufferSize)
    {
        const auto chunkAtEndOfBuffer = bufferSize - newStart;

        vecops::copy (samples,
                      buffer.getWritePointer (0, newStart),
                      chunkAtEndOfBuffer);

        const auto left = numSamples - chunkAtEndOfBuffer;

        vecops::copy (samples + chunkAtEndOfBuffer,
                      buffer.getWritePointer (0, 0),
                      left);

        lastFrameEnd = left;
    }
    else
    {
        vecops::copy (samples,
                      buffer.getWritePointer (0, newStart),
                      numSamples);

        lastFrameEnd = newStart + numSamples;
    }
}

template < typename SampleType >
int CircularBuffer< SampleType >::getCapacity() const
{
    return buffer.getNumSamples();
}

template < typename SampleType >
int CircularBuffer< SampleType >::getLastFrameStartIndex() const
{
    return lastFrameStart;
}

template < typename SampleType >
int CircularBuffer< SampleType >::getLastFrameEndIndex() const
{
    return lastFrameEnd;
}

template < typename SampleType >
int CircularBuffer< SampleType >::clipValueToCapacity (int value) const
{
    const auto capacity = getCapacity();

    if (value < capacity)
        return value;

    return value - capacity;
}

template < typename SampleType >
SampleType CircularBuffer< SampleType >::getSample (int index) const
{
    jassert (index >= 0 && index <= getCapacity());
    return buffer.getSample (0, index);
}

template class CircularBuffer< float >;
template class CircularBuffer< double >;

}  // namespace bav::dsp
