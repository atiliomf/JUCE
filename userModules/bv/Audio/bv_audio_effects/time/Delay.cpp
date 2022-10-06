namespace bav::dsp::FX
{
template < typename SampleType >
void Delay< SampleType >::setDelay (int delayInSamples)
{
    delay.setDelay (SampleType (delayInSamples));
}

template < typename SampleType >
void Delay< SampleType >::prepare (double samplerate, int blocksize)
{
    jassert (samplerate > 0);

    spec.sampleRate       = samplerate;
    spec.maximumBlockSize = static_cast< juce::uint32 > (blocksize);
    spec.numChannels      = 2;

    delay.prepare (spec);

    dryGain.reset (blocksize);
    wetGain.reset (blocksize);
}

template < typename SampleType >
void Delay< SampleType >::reset()
{
    delay.reset();
    dryGain.reset (static_cast< int > (spec.maximumBlockSize));
    wetGain.reset (static_cast< int > (spec.maximumBlockSize));
}

template < typename SampleType >
void Delay< SampleType >::setDryWet (int wetMixPercent)
{
    const auto wet = static_cast< float > (wetMixPercent) * 0.01f;
    wetGain.set (wet);
    dryGain.set (1.0f - wet);
}


template < typename SampleType >
void Delay< SampleType >::pushSample (int channel, SampleType sample)
{
    delay.pushSample (channel, sample);
}


template < typename SampleType >
SampleType Delay< SampleType >::popSample (int channel, SampleType* delayLevel)
{
    const auto delaySamps = delay.getDelay();

    const auto drySample = delay.popSample (channel, SampleType (0), false)
                         * dryGain.getNextValue();
    const auto wetSample =
        delay.popSample (channel, delaySamps, true) * wetGain.getNextValue();

    if (delayLevel != nullptr) *delayLevel = std::abs (wetSample);

    return drySample + wetSample;
}

template < typename SampleType >
SampleType Delay< SampleType >::processChannel (int         channel,
                                                int         numSamples,
                                                SampleType* signal,
                                                const SampleType*)
{
    if (numSamples == 0) return (SampleType) 0;

    for (int i = 0; i < numSamples; ++i)
        pushSample (channel, signal[i]);

    auto avgMag = SampleType (0.0);

    for (int i = 0; i < numSamples; ++i)
    {
        SampleType mag;
        signal[i] = popSample (channel, &mag);
        avgMag += mag;
    }

    avgMag /= static_cast< SampleType > (numSamples);
    return avgMag;
}

template class Delay< float >;
template class Delay< double >;


}  // namespace bav::dsp::FX
