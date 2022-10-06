
namespace bav::dsp::psola
{
template < typename SampleType >
void AnalysisGrainStorage< SampleType >::prepare (int blocksize)
{
    buffer.resize (blocksize, 4);
    grainIndices.ensureStorageAllocated (blocksize);
    distances.ensureStorageAllocated (blocksize);
}

template < typename SampleType >
void AnalysisGrainStorage< SampleType >::storeNewFrame (const SampleType*         inputSamples,
                                                        int                       numSamples,
                                                        const juce::Array< int >& grainOnsets)
{
    jassert (! grainOnsets.isEmpty());

    buffer.storeSamples (inputSamples, numSamples);

    grainIndices.clearQuick();

    for (auto index : grainOnsets)
        grainIndices.add (blockIndexToBufferIndex (index));
}

template < typename SampleType >
SampleType AnalysisGrainStorage< SampleType >::getSample (int grainStartIndexInCircularBuffer, int grainTick) const
{
    return buffer.getSample (buffer.clipValueToCapacity (grainStartIndexInCircularBuffer + grainTick));
}

template < typename SampleType >
int AnalysisGrainStorage< SampleType >::getStartOfClosestGrain (int sampleIndex) const
{
    jassert (! grainIndices.isEmpty());

    const auto index = blockIndexToBufferIndex (sampleIndex);

    distances.clearQuick();

    for (auto grain : grainIndices)
        distances.add (std::abs (grain - index));

    return grainIndices.getUnchecked (vecops::findIndexOfMinElement (distances.getRawDataPointer(), distances.size()));
}

template < typename SampleType >
int AnalysisGrainStorage< SampleType >::blockIndexToBufferIndex (int blockIndex) const
{
    return buffer.clipValueToCapacity (blockIndex + buffer.getLastFrameStartIndex());
}

template class AnalysisGrainStorage< float >;
template class AnalysisGrainStorage< double >;

}  // namespace bav::dsp::psola
