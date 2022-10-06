#pragma once

#include <bv_dsp/bv_dsp.h>

namespace bav::dsp::psola
{
template < typename SampleType >
class PeakFinder
{
public:
    using IArray = juce::Array< int >;
    using FArray = juce::Array< float >;

    void findPeaks (IArray&           targetArray,
                    const SampleType* reading,
                    int               totalNumSamples,
                    int               period);

    void releaseResources();

    void prepare (int blocksize);

private:
    int findNextPeak (int               frameStart,
                      int               frameEnd,
                      int               predictedPeak,
                      const SampleType* reading,
                      const IArray&     targetArray,
                      int               period,
                      int               grainSize);

    void sortSampleIndicesForPeakSearching (IArray& output,
                                            int     startSample,
                                            int     endSample,
                                            int     predictedPeak);

    void getPeakCandidateInRange (IArray&           candidates,
                                  const SampleType* input,
                                  int               startSample,
                                  int               endSample,
                                  int               predictedPeak,
                                  const IArray&     searchingOrder);

    int chooseIdealPeakCandidate (const IArray&     candidates,
                                  const SampleType* reading,
                                  int               deltaTarget1,
                                  int               deltaTarget2);

    int choosePeakWithGreatestPower (const IArray&     candidates,
                                     const SampleType* reading);

    IArray peakIndices;
    IArray peakCandidates;
    IArray peakSearchingOrder;

    FArray candidateDeltas;
    IArray finalHandful;
    FArray finalHandfulDeltas;

    static constexpr auto numPeaksToTest          = 10;
    static constexpr auto defaultFinalHandfulSize = 5;
};

}  // namespace bav::dsp::psola
