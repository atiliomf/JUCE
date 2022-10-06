
namespace bav::dsp::FX
{
template < typename SampleType >
Compressor< SampleType >::Compressor()
{
    spec.numChannels = 2;
    update();
}

template < typename SampleType >
void Compressor< SampleType >::prepare (double samplerate, int blocksize)
{
    jassert (samplerate > 0);

    spec.sampleRate       = samplerate;
    spec.maximumBlockSize = static_cast< juce::uint32 > (blocksize);

    envelopeFilter.prepare (spec);

    update();
    reset();
}

template < typename SampleType >
SampleType Compressor< SampleType >::processChannel (int               channel,
                                                     int               numSamples,
                                                     SampleType*       signalToCompress,
                                                     const SampleType* sidechain)
{
    if (numSamples == 0) return static_cast< SampleType > (0);

    SampleType avgGainReduction = 0;
    SampleType gainRedux        = 0;

    for (int s = 0; s < numSamples; ++s)
    {
        signalToCompress[s] = processSample (channel, signalToCompress[s], sidechain[s], &gainRedux);
        avgGainReduction += gainRedux;
    }

    avgGainReduction *= (1 / numSamples);
    return avgGainReduction;
}

template < typename SampleType >
SampleType Compressor< SampleType >::processSample (int         channel,
                                                    SampleType  inputSample,
                                                    SampleType  sidechainSample,
                                                    SampleType* gainReduction)
{
    auto env = envelopeFilter.processSample (
        channel, sidechainSample);  // Ballistics filter with peak rectifier

    // VCA
    auto gain = (env < threshold) ? SampleType (1.0)
                                  : std::pow (env * thresholdInverse,
                                              ratioInverse - SampleType (1.0));

    if (gainReduction != nullptr)  // report gain reduction, if requested
        *gainReduction = gain;

    return gain * inputSample;
}


template < typename SampleType >
void Compressor< SampleType >::setThreshold (float newThresh_dB)
{
    thresholddB = static_cast< SampleType > (newThresh_dB);
    update();
}

template < typename SampleType >
void Compressor< SampleType >::setRatio (float newRatio)
{
    jassert (newRatio >= 1.0f);

    ratio = static_cast< SampleType > (newRatio);
    update();
}

template < typename SampleType >
void Compressor< SampleType >::setAttack (float attackMs)
{
    attackTime = static_cast< SampleType > (attackMs);
    update();
}

template < typename SampleType >
void Compressor< SampleType >::setRelease (float releaseMs)
{
    releaseTime = static_cast< SampleType > (releaseMs);
    update();
}

template < typename SampleType >
void Compressor< SampleType >::reset()
{
    envelopeFilter.reset();
}

template < typename SampleType >
void Compressor< SampleType >::update()
{
    threshold = juce::Decibels::decibelsToGain (
        thresholddB, static_cast< SampleType > (-200.0));
    thresholdInverse = static_cast< SampleType > (1.0) / threshold;
    ratioInverse     = static_cast< SampleType > (1.0) / ratio;

    envelopeFilter.setAttackTime (attackTime);
    envelopeFilter.setReleaseTime (releaseTime);
}


template class Compressor< float >;
template class Compressor< double >;


}  // namespace bav::dsp::FX
