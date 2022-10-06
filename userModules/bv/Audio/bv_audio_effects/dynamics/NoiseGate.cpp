
namespace bav::dsp::FX
{
template < typename SampleType >
NoiseGate< SampleType >::NoiseGate()
{
    spec.numChannels = 2;

    update();

    RMSFilter.setLevelCalculationType (
        juce::dsp::BallisticsFilterLevelCalculationType::RMS);
    RMSFilter.setAttackTime (static_cast< SampleType > (0.0));
    RMSFilter.setReleaseTime (static_cast< SampleType > (50.0));
}

template < typename SampleType >
void NoiseGate< SampleType >::setInverted (bool gateBehaviorShouldBeInverted)
{
    inverted = gateBehaviorShouldBeInverted;
}


template < typename SampleType >
void NoiseGate< SampleType >::setThreshold (float newThreshold_dB)
{
    thresholddB = static_cast< SampleType > (newThreshold_dB);
    update();
}

template < typename SampleType >
void NoiseGate< SampleType >::setRatio (SampleType newRatio)
{
    jassert (newRatio >= static_cast< SampleType > (1.0));

    ratio = newRatio;
    update();
}


template < typename SampleType >
void NoiseGate< SampleType >::setAttack (SampleType newAttack_ms)
{
    attackTime = newAttack_ms;
    update();
}


template < typename SampleType >
void NoiseGate< SampleType >::setRelease (SampleType newRelease_ms)
{
    releaseTime = newRelease_ms;
    update();
}


template < typename SampleType >
void NoiseGate< SampleType >::prepare (double samplerate, int blocksize)
{
    jassert (samplerate > 0);

    spec.sampleRate       = samplerate;
    spec.maximumBlockSize = static_cast< juce::uint32 > (blocksize);

    RMSFilter.prepare (spec);
    envelopeFilter.prepare (spec);

    update();
    reset();
}


template < typename SampleType >
void NoiseGate< SampleType >::reset()
{
    RMSFilter.reset();
    envelopeFilter.reset();
}

template < typename SampleType >
SampleType NoiseGate< SampleType >::processChannel (int               channel,
                                                    int               numSamples,
                                                    SampleType*       signalToGate,
                                                    const SampleType* sidechain)
{
    if (numSamples == 0) return (SampleType) 0;

    SampleType avgGainReduction = 0;
    SampleType gainRedux        = 0;

    for (int s = 0; s < numSamples; ++s)
    {
        signalToGate[s] = processSample (channel, signalToGate[s], sidechain[s], &gainRedux);
        avgGainReduction += gainRedux;
    }

    return avgGainReduction * ((SampleType) 1 / (SampleType) numSamples);
}


template < typename SampleType >
SampleType NoiseGate< SampleType >::processSample (const int        channel,
                                                   const SampleType sampleToGate,
                                                   const SampleType sidechainValue,
                                                   SampleType*      gainReduction)
{
    auto env = RMSFilter.processSample (channel,
                                        sidechainValue);  // RMS ballistics filter

    env = envelopeFilter.processSample (channel, env);  // Ballistics filter

    // VCA
    SampleType gain;

    if (inverted)
    {
        gain = (env < threshold)
                 ? static_cast< SampleType > (1.0)
                 : std::pow (env * thresholdInverse,
                             currentRatio - static_cast< SampleType > (1.0));
    }
    else
    {
        gain = (env > threshold)
                 ? static_cast< SampleType > (1.0)
                 : std::pow (env * thresholdInverse,
                             currentRatio - static_cast< SampleType > (1.0));
    }

    if (gainReduction != nullptr)  // report gain reduction, if requested
        *gainReduction = gain;

    return gain * sampleToGate;
}

template < typename SampleType >
void NoiseGate< SampleType >::update()
{
    threshold = juce::Decibels::decibelsToGain (
        thresholddB, static_cast< SampleType > (-200.0));
    thresholdInverse = static_cast< SampleType > (1.0) / threshold;
    currentRatio     = ratio;

    envelopeFilter.setAttackTime (attackTime);
    envelopeFilter.setReleaseTime (releaseTime);
}


template class NoiseGate< float >;
template class NoiseGate< double >;

}  // namespace bav::dsp::FX
