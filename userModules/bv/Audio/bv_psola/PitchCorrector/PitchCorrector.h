#pragma once

/*
 This class attempts to guess the correct pitch for an input signal, and performs PSOLA pitch shifting to provide pitch corrected output.
 Intended for monophonic audio only.
 */

namespace bav::dsp::psola
{
template < typename SampleType >
class PitchCorrectorBase
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;
    using Analyzer    = dsp::psola::Analyzer< SampleType >;

    PitchCorrectorBase (Analyzer& analyzerToUse, const midi::PitchPipeline* pitchConverterToUse = nullptr);

    void processNextFrame (AudioBuffer& output);

    void prepare (double samplerate);

    int   getOutputMidiPitch() const;
    float getOutputFreq() const;

    int getCentsSharp() const;

private:
    float updateAndReturnFreq();
    float getCurrentInputMidipitch() const;
    int   getMidipitchToCorrectTo (float inputMidipitch) const;

    Analyzer& analyzer;

    const midi::PitchPipeline* pitch;

    dsp::psola::Shifter< SampleType > shifter {analyzer};

    double sampleRate {0.};

    int correctedNote {-1};  // the midi pitch that the input will be corrected to
    int centsSharp {-1};
};


template < typename SampleType >
class PitchCorrector : public PitchCorrectorBase< SampleType >
{
public:
    PitchCorrector (const midi::PitchPipeline* pitchConverterToUse = nullptr);

private:
    dsp::psola::Analyzer< SampleType > analyzer;
};

}  // namespace bav::dsp::psola
