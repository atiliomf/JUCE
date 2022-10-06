
/*
    ASDF-based pitch detector
*/

#pragma once

namespace bav::dsp
{
template < typename SampleType >
class PitchDetector
{
    using AudioBuffer = juce::AudioBuffer< SampleType >;

public:
    PitchDetector();
    ~PitchDetector() = default;

    void initialize();

    void releaseResources();

    float detectPitch (const AudioBuffer& inputAudio);
    float detectPitch (const SampleType* inputAudio, int numSamples);

    void setHzRange (int newMinHz, int newMaxHz);
    void setConfidenceThresh (SampleType newThresh);
    void setSamplerate (double newSamplerate);

    int getLatencySamples() const noexcept;

    juce::Range< int > getCurrentLegalPeriodRange() const;

private:
    int chooseIdealPeriodCandidate (const SampleType* asdfData,
                                    int               asdfDataSize,
                                    int               minIndex);

    void getNextBestPeriodCandidate (juce::Array< int >& candidates,
                                     const SampleType*   asdfData,
                                     int                 dataSize);


    int minHz, maxHz;
    int minPeriod, maxPeriod;

    int  lastEstimatedPeriod;
    bool lastFrameWasPitched;

    double samplerate;

    SampleType
        confidenceThresh;  // if the lowest asdf data value is above this thresh, the frame of audio is determined to be unpitched

    AudioBuffer asdfBuffer;  // calculated ASDF values will be placed in this buffer

    juce::Array< int >        periodCandidates;
    juce::Array< int >        candidateDeltas;
    juce::Array< SampleType > weightedCandidateConfidence;

    AudioBuffer                   filteringBuffer;
    filters::Filter< SampleType > loCut, hiCut;

    static constexpr int numPeriodCandidatesToTest = 10;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchDetector)
};

}  // namespace bav::dsp
