
namespace bav::dsp::FX
{
template < typename SampleType >
void Saturator< SampleType >::setHardness (float newHardness)
{
    hardness = static_cast< SampleType > (newHardness);
}

template < typename SampleType >
void Saturator< SampleType >::process (juce::AudioBuffer< SampleType >& audio)
{
    for (int chan = 0; chan < audio.getNumChannels(); ++chan)
    {
        auto* audio = audio.getWritePointer (chan);

        for (int s = 0; s < audio.getNumSamples(); ++s)
            audio[s] = processSample (audio[s]);
    }
}

template < typename SampleType >
SampleType Saturator< SampleType >::processSample (SampleType inputSample)
{
    const SampleType amplitude = 1.;

    const auto tf     = transferFunc (amplitude);
    const auto sample = inputSample * amplitude;

    const auto p1 = phaseOne (sample, tf);

    return p1 + phaseTwo (sample, tf, p1);
}

template < typename SampleType >
SampleType Saturator< SampleType >::transferFunc (SampleType amplitude)
{
    return std::pow (hardness, amplitude - SampleType (1.));
}

template < typename SampleType >
SampleType Saturator< SampleType >::phaseOne (SampleType inputSample, SampleType transferFuncSample)
{
    return std::max (-transferFuncSample,
                     std::min (inputSample, transferFuncSample));
}

template < typename SampleType >
SampleType Saturator< SampleType >::phaseTwo (SampleType inputSample, SampleType transferFuncSample, SampleType phaseOneSample)
{
    return std::tanh ((inputSample - phaseOneSample) / (SampleType (1.) - transferFuncSample))
         * (SampleType (1.) - transferFuncSample);
}

template < typename SampleType >
void Saturator< SampleType >::prepare (double samplerate, int blocksize)
{
}

template < typename SampleType >
void Saturator< SampleType >::bypassedBlock (int numSamples)
{
}

template class Saturator< float >;
template class Saturator< double >;

}  // namespace bav::dsp::FX
