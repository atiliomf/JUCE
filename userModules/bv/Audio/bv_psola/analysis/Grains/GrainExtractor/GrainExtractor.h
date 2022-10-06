
#pragma once

#include "PeakFinder.h"

namespace bav::dsp::psola
{
template < typename SampleType >
class AnalysisGrainExtractor
{
public:
    AnalysisGrainExtractor()  = default;
    ~AnalysisGrainExtractor() = default;

    void prepare (int blocksize);

    void releaseResources();

    void analyzeInput (const SampleType* inputSamples,
                       int               numSamples,
                       int               period);

    const juce::Array< int >& getIndices() const;

private:
    PeakFinder< SampleType > peakFinder;

    juce::Array< int > grainStartIndices;
};


}  // namespace bav::dsp::psola
