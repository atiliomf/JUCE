
namespace bav::dsp::psola
{
template < typename SampleType >
void AnalysisGrainExtractor< SampleType >::releaseResources()
{
    peakFinder.releaseResources();
}


template < typename SampleType >
void AnalysisGrainExtractor< SampleType >::prepare (int blocksize)
{
    grainStartIndices.ensureStorageAllocated (blocksize);
    peakFinder.prepare (blocksize);
}


template < typename SampleType >
void AnalysisGrainExtractor< SampleType >::analyzeInput (const SampleType* inputSamples,
                                                         int               numSamples,
                                                         int               period)
{
    // identify  peak indices for each pitch period & places them in the peakIndices array

    grainStartIndices.clearQuick();
    peakFinder.findPeaks (grainStartIndices, inputSamples, numSamples, period);

    jassert (! grainStartIndices.isEmpty());

    const auto grainLength = period * 2;
    const auto halfPeriod  = juce::roundToInt (period * 0.5f);

    // create array of grain start indices, such that grains are 2 pitch periods long, CENTERED on points of synchronicity previously identified

    for (int i = 0; i < grainStartIndices.size(); ++i)
    {
        const auto peakIndex = grainStartIndices.getUnchecked (i);

        // offset the peak index by the period so that the peak index will be in the center of the grain (if grain is 2 periods long)
        auto grainStart = peakIndex - period;

        if (grainStart < 0)
        {
            if (i < grainStartIndices.size() - 2 || grainStartIndices.size() > 1) continue;

            while (grainStart < 0)
                grainStart += halfPeriod;
        }

        if (grainStart + grainLength > numSamples)
        {
            if (i < grainStartIndices.size() - 2 || grainStartIndices.size() > 1) continue;

            while (grainStart + grainLength > numSamples)
                grainStart -= halfPeriod;

            if (grainStart < 0)
            {
                if (! grainStartIndices.isEmpty()) continue;
                grainStart = 0;
            }
        }

        if (grainStart < 0 || grainStart + grainLength > numSamples)
            continue;

        grainStartIndices.add (grainStart);
    }

    jassert (! grainStartIndices.isEmpty());
}

template < typename SampleType >
const juce::Array< int >& AnalysisGrainExtractor< SampleType >::getIndices() const
{
    return grainStartIndices;
}


template class AnalysisGrainExtractor< float >;
template class AnalysisGrainExtractor< double >;


}  // namespace bav::dsp::psola
