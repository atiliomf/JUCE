
namespace bav::dsp::psola
{
template < typename SampleType >
PitchCorrectorBase< SampleType >::PitchCorrectorBase (Analyzer& analyzerToUse, const midi::PitchPipeline* pitchConverterToUse)
    : analyzer (analyzerToUse), pitch (pitchConverterToUse)
{
}

template < typename SampleType >
void PitchCorrectorBase< SampleType >::processNextFrame (AudioBuffer& output)
{
    shifter.setPitch (updateAndReturnFreq(), sampleRate);
    shifter.getSamples (output);
}

template < typename SampleType >
void PitchCorrectorBase< SampleType >::prepare (double samplerate)
{
    sampleRate = samplerate;
}

template < typename SampleType >
float PitchCorrectorBase< SampleType >::updateAndReturnFreq()
{
    const auto currentInput = getCurrentInputMidipitch();

    correctedNote = getMidipitchToCorrectTo (currentInput);
    centsSharp    = juce::roundToInt ((currentInput - correctedNote) * 100.f);

    return getOutputFreq();
}

template < typename SampleType >
int PitchCorrectorBase< SampleType >::getMidipitchToCorrectTo (float inputMidipitch) const
{
    const auto closestNote = juce::roundToInt (inputMidipitch);

    if (pitch != nullptr)
    {
        if (pitch->tuning.shouldFilterNote (closestNote))
        {
        }
    }

    return closestNote;
}

template < typename SampleType >
float PitchCorrectorBase< SampleType >::getCurrentInputMidipitch() const
{
    if (pitch != nullptr)
        return pitch->getMidiForFrequency (analyzer.getFrequency());

    return math::freqToMidi (analyzer.getFrequency());
}

template < typename SampleType >
int PitchCorrectorBase< SampleType >::getOutputMidiPitch() const
{
    return correctedNote;
}

template < typename SampleType >
int PitchCorrectorBase< SampleType >::getCentsSharp() const
{
    return centsSharp;
}

template < typename SampleType >
float PitchCorrectorBase< SampleType >::getOutputFreq() const
{
    if (pitch != nullptr)
        return pitch->getFrequencyForMidi (correctedNote);

    return (float) math::midiToFreq (correctedNote);
}

template class PitchCorrectorBase< float >;
template class PitchCorrectorBase< double >;


template < typename SampleType >
PitchCorrector< SampleType >::PitchCorrector (const midi::PitchPipeline* pitchConverterToUse)
    : PitchCorrectorBase< SampleType > (analyzer, pitchConverterToUse)
{
}

template class PitchCorrector< float >;
template class PitchCorrector< double >;

}  // namespace bav::dsp::psola
