
#pragma once

namespace bav::dsp::psola
{
template < typename SampleType >
class AnalysisGrainStorage
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    AnalysisGrainStorage()  = default;
    ~AnalysisGrainStorage() = default;

    void prepare (int blocksize);

    void storeNewFrame (const SampleType*         inputSamples,
                        int                       numSamples,
                        const juce::Array< int >& grainOnsets);

    int getStartOfClosestGrain (int sampleIndex) const;

    SampleType getSample (int grainStartIndexInCircularBuffer, int grainTick) const;

private:
    int blockIndexToBufferIndex (int blockIndex) const;

    CircularBuffer< SampleType > buffer;

    juce::Array< int >         grainIndices;  // sample indices in the circular buffer of grain onsets
    mutable juce::Array< int > distances;     // array for sorting distances of grain onsets
};

}  // namespace bav::dsp::psola
