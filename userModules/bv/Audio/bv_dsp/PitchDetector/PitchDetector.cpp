
#if BV_USE_VDSP
#    include <Accelerate/Accelerate.h>
#endif

namespace bav::dsp
{
template < typename SampleType >
PitchDetector< SampleType >::PitchDetector()
    : minHz (0), maxHz (0), lastEstimatedPeriod (0), lastFrameWasPitched (false), samplerate (0.0), confidenceThresh (static_cast< SampleType > (0.15)), asdfBuffer (0, 0)
{
}


template < typename SampleType >
void PitchDetector< SampleType >::initialize()
{
    periodCandidates.ensureStorageAllocated (numPeriodCandidatesToTest);
    candidateDeltas.ensureStorageAllocated (numPeriodCandidatesToTest);
    weightedCandidateConfidence.ensureStorageAllocated (numPeriodCandidatesToTest);

    asdfBuffer.setSize (1, 512);

    lastFrameWasPitched = false;

    hiCut.prepare();
    loCut.prepare();
}

template < typename SampleType >
void PitchDetector< SampleType >::releaseResources()
{
    periodCandidates.clear();
    candidateDeltas.clear();
    weightedCandidateConfidence.clear();

    asdfBuffer.setSize (0, 0, false, false, false);

    hiCut.reset();
    loCut.reset();
}


template < typename SampleType >
static inline int samplesToFirstZeroCrossing (const SampleType* inputAudio, int numSamples)
{
#if BV_USE_VDSP
    unsigned long index =
        0;  // in Apple's vDSP, the type vDSP_Length is an alias for unsigned long
    unsigned long totalcrossings = 0;

    if constexpr (std::is_same_v< SampleType, float >)
        vDSP_nzcros (inputAudio,
                     vDSP_Stride (1),
                     vDSP_Length (1),
                     &index,
                     &totalcrossings,
                     vDSP_Length (numSamples));
    else
        vDSP_nzcrosD (inputAudio,
                      vDSP_Stride (1),
                      vDSP_Length (1),
                      &index,
                      &totalcrossings,
                      vDSP_Length (numSamples));
    return static_cast< int > (index);
#else
    const bool startedPositive = inputAudio[0] > SampleType (0);

    for (int s = 1; s < numSamples; ++s)
        if (startedPositive != (inputAudio[s] > SampleType (0))) return s;

    return 0;
#endif
}

template int samplesToFirstZeroCrossing (const float*, int);
template int samplesToFirstZeroCrossing (const double*, int);


template < typename SampleType >
float PitchDetector< SampleType >::detectPitch (const AudioBuffer& inputAudio)
{
    return detectPitch (inputAudio.getReadPointer (0), inputAudio.getNumSamples());
}

template < typename SampleType >
float PitchDetector< SampleType >::detectPitch (const SampleType* inputAudio, int numSamples)
{
    // this function returns the pitch in Hz, or 0.0f if the frame of audio is determined to be unpitched

    jassert (samplerate > 0);

    const auto halfNumSamples = juce::roundToInt (floor (numSamples * 0.5f));

    jassert (numSamples >= 2 * maxPeriod);

    // the minPeriod & maxPeriod members define the overall global period range; here, the minLag & maxLag local variables are used to define the period range for this specific frame of audio, if it can be constrained more than the global range based on instantaneous conditions:

    auto minLag = samplesToFirstZeroCrossing (
        inputAudio,
        numSamples);  // period cannot be smaller than the # of samples to the first zero crossing
    auto maxLag = halfNumSamples;

    if (lastFrameWasPitched)  // pitch shouldn't halve or double between consecutive voiced frames
    {
        minLag = std::max (
            minLag, juce::roundToInt (lastEstimatedPeriod * SampleType (0.5)));
        maxLag = std::min (maxLag,
                           juce::roundToInt (lastEstimatedPeriod * SampleType (2)));
    }

    minLag = std::max (minLag, minPeriod);
    maxLag = std::min (maxLag, maxPeriod);

    if (! (maxLag > minLag))  // truncation of edge cases
        minLag = std::min (maxLag - 1, minPeriod);

    jassert (maxLag > minLag);

    auto* reading = filteringBuffer.getWritePointer (0);

    // copy to filtering buffer
    vecops::copy (inputAudio, reading, numSamples);

    // filter to our min and max possible frequencies
    loCut.coefs.makeHighPass (samplerate,
                              static_cast< SampleType > (math::freqFromPeriod (samplerate, maxLag)));

    hiCut.coefs.makeLowPass (samplerate,
                             static_cast< SampleType > (math::freqFromPeriod (samplerate, minLag)));

    loCut.process (reading, numSamples);
    hiCut.process (reading, numSamples);

    auto* asdfData = asdfBuffer.getWritePointer (0);

    vecops::fill (asdfData, SampleType (0), asdfBuffer.getNumSamples());

    // COMPUTE ASDF

    jassert (asdfBuffer.getNumSamples() >= maxLag - minLag + 1);

    const auto asdfDataSize = maxLag - minLag + 1;

    for (int k = minLag; k <= maxLag; ++k)  // k = lag = period
    {
        const auto index =
            k
            - minLag;  // the actual asdfBuffer index for this k value's data. offset = minPeriod

        jassert (index <= asdfDataSize);

        for (int s1 = 0, s2 = halfNumSamples; s1 < halfNumSamples && s2 < numSamples;
             ++s1, ++s2)
        {
            const auto difference =
                ((reading[s1] - reading[s1 + k]) + (reading[s2 - k] - reading[s2]));

            asdfData[index] += (difference * difference);
        }
    }

    vecops::normalize (asdfData, asdfDataSize);

    int        minIndex           = 0;
    SampleType greatestConfidence = 0;

    vecops::findMinAndMinIndex (
        asdfData, asdfDataSize, greatestConfidence, minIndex);

    jassert (minIndex >= 0 && minIndex <= asdfDataSize);

    if (greatestConfidence
        > confidenceThresh)  // determine if frame is unpitched - not enough periodicity
    {
        lastFrameWasPitched = false;
        return 0.0f;
    }

    if (lastFrameWasPitched)
        minIndex = chooseIdealPeriodCandidate (asdfData, asdfDataSize, minIndex);

    const auto realPeriod =
        minIndex
        + minLag;  // account for offset in asdf data (index 0 stored data for lag of minPeriod)

    jassert (realPeriod <= maxPeriod && realPeriod >= minPeriod);

    lastEstimatedPeriod = realPeriod;
    lastFrameWasPitched = true;

    // return pitch in hz as a float
    return static_cast< float > (static_cast< float > (samplerate)
                                 / static_cast< float > (realPeriod));
}


template < typename SampleType >
void PitchDetector< SampleType >::getNextBestPeriodCandidate (
    juce::Array< int >& candidates, const SampleType* asdfData, int dataSize)
{
    int index = -1;

    for (int i = 0; i < dataSize; ++i)
    {
        if (! candidates.contains (i))
        {
            index = i;
            break;
        }
    }

    if (index == -1) return;

    auto min = asdfData[index];

    for (int i = 0; i < dataSize; ++i)
    {
        if (candidates.contains (i)) continue;

        const auto current = asdfData[i];

        if (current < min)
        {
            min   = current;
            index = i;
        }
    }

    candidates.add (index);
}


template < typename SampleType >
int PitchDetector< SampleType >::chooseIdealPeriodCandidate (
    const SampleType* asdfData, int asdfDataSize, int minIndex)
{
    periodCandidates.clearQuick();
    candidateDeltas.clearQuick();
    weightedCandidateConfidence.clearQuick();

    periodCandidates.add (minIndex);

    for (int c = 1; c <= numPeriodCandidatesToTest; ++c)
        getNextBestPeriodCandidate (periodCandidates, asdfData, asdfDataSize);

    jassert (! periodCandidates.isEmpty());

    if (periodCandidates.size() <= 2) return minIndex;

    const auto adding = minPeriod - lastEstimatedPeriod;

    // candidate deltas: how far away each period candidate is from the last estimated period
    for (int candidate : periodCandidates)
        candidateDeltas.add (abs (candidate + adding));

    // find the difference between the max & min delta values of the candidates
    const auto deltaRange = vecops::findRangeOfExtrema (
        candidateDeltas.getRawDataPointer(), candidateDeltas.size());

    if (deltaRange < 2)  // prevent dividing by zero in the next step...
        return minIndex;

    // weight the asdf data based on each candidate's delta value
    // because higher asdf values represent a lower confidence in that period candidate, we want to artificially increase the asdf data a bit for candidates with higher deltas
    for (int c = 0; c < periodCandidates.size(); ++c)
        weightedCandidateConfidence.add (
            asdfData[periodCandidates.getUnchecked (c)]
            * (candidateDeltas.getUnchecked (c) / deltaRange));

    // choose the estimated period based son the lowest weighted asdf data value:
    return periodCandidates.getUnchecked (vecops::findIndexOfMinElement (
        weightedCandidateConfidence.getRawDataPointer(),
        weightedCandidateConfidence.size()));
}


template < typename SampleType >
void PitchDetector< SampleType >::setHzRange (int newMinHz, int newMaxHz)
{
    jassert (newMaxHz > newMinHz);
    jassert (newMinHz > 0 && newMaxHz > 0);

    minHz = newMinHz;
    maxHz = newMaxHz;

    if (samplerate == 0) return;

    maxPeriod = juce::roundToInt (samplerate / static_cast< double > (minHz));
    minPeriod = juce::roundToInt (samplerate / static_cast< double > (maxHz));

    if (minPeriod < 1) minPeriod = 1;

    if (! (maxPeriod > minPeriod)) maxPeriod = minPeriod + 1;

    const auto numOfLagValues = maxPeriod - minPeriod + 1;

    asdfBuffer.setSize (1, numOfLagValues, true, true, true);
    filteringBuffer.setSize (1, numOfLagValues, true, true, true);
}


template < typename SampleType >
void PitchDetector< SampleType >::setSamplerate (double newSamplerate)
{
    jassert (newSamplerate > 0);

    if (lastFrameWasPitched)
        lastEstimatedPeriod =
            juce::roundToInt (newSamplerate / (samplerate / lastEstimatedPeriod));

    samplerate = newSamplerate;

    if (minHz > 0 && maxHz > 0) setHzRange (minHz, maxHz);

    hiCut.prepare();
    loCut.prepare();
}

template < typename SampleType >
juce::Range< int > PitchDetector< SampleType >::getCurrentLegalPeriodRange() const
{
    return {minPeriod, maxPeriod};
}

template < typename SampleType >
void PitchDetector< SampleType >::setConfidenceThresh (SampleType newThresh)
{
    confidenceThresh = newThresh;
}

template < typename SampleType >
int PitchDetector< SampleType >::getLatencySamples() const noexcept
{
    return 2 * maxPeriod;
}

template class PitchDetector< float >;
template class PitchDetector< double >;

}  // namespace bav::dsp
