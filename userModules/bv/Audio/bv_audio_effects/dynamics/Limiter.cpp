
namespace bav::dsp::FX
{
template < typename SampleType >
void Limiter< SampleType >::setThreshold (float thresh_dB)
{
    thresholddB = thresh_dB;
    update();
}

template < typename SampleType >
void Limiter< SampleType >::setRelease (float release_ms)
{
    releaseTime = release_ms;
    update();
}

template < typename SampleType >
void Limiter< SampleType >::reset()
{
    firstStageCompressor.reset();
    secondStageCompressor.reset();

    outputVolume.reset (sampleRate, 0.001);
}

template < typename SampleType >
void Limiter< SampleType >::prepare (double samplerate, int blocksize)
{
    jassert (samplerate > 0);

    sampleRate = samplerate;

    firstStageCompressor.prepare (samplerate, blocksize);
    secondStageCompressor.prepare (samplerate, blocksize);

    update();
    reset();
}


template < typename SampleType >
static inline SampleType getMagnitude (const SampleType* signal, int numSamps)
{
    auto r = juce::FloatVectorOperations::findMinAndMax (signal, numSamps);
    return juce::jmax (r.getStart(), -r.getStart(), r.getEnd(), -r.getEnd());
}
template float  getMagnitude (const float*, int);
template double getMagnitude (const double*, int);


template < typename SampleType >
SampleType Limiter< SampleType >::processChannel (int               channel,
                                                  int               numSamples,
                                                  SampleType*       signalToLimit,
                                                  const SampleType* sidechain)
{
    if (numSamples == 0) return (SampleType) 0;

    const auto levelBefore = getMagnitude (signalToLimit, numSamples);

    for (int s = 0; s < numSamples; ++s)
    {
        const auto sc = sidechain[s];

        signalToLimit[s] = firstStageCompressor.processSample (channel, signalToLimit[s], sc, nullptr);

        signalToLimit[s] = secondStageCompressor.processSample (channel, signalToLimit[s], sc, nullptr);

        signalToLimit[s] = signalToLimit[s] * outputVolume.getNextValue();
    }

    juce::FloatVectorOperations::clip (signalToLimit,
                                       signalToLimit,
                                       SampleType (-1.0),
                                       SampleType (1.0),
                                       numSamples);

    return juce::jlimit (SampleType (0.0), SampleType (1.0), getMagnitude (signalToLimit, numSamples) - levelBefore);
}

template < typename SampleType >
void Limiter< SampleType >::update()
{
    firstStageCompressor.setThreshold (-10.0f);
    firstStageCompressor.setRatio (4.0f);
    firstStageCompressor.setAttack (2.0f);
    firstStageCompressor.setRelease (200.0f);

    secondStageCompressor.setThreshold (thresholddB);
    secondStageCompressor.setRatio (1000.0f);
    secondStageCompressor.setAttack (0.001f);
    secondStageCompressor.setRelease (releaseTime);

    auto ratioInverse = static_cast< SampleType > (1.0 / 4.0);

    auto gain = static_cast< SampleType > (std::pow (10.0, 10.0 * (1.0 - ratioInverse) / 40.0));
    gain *= static_cast< SampleType > (juce::Decibels::decibelsToGain (-thresholddB, -100.0f));

    outputVolume.setTargetValue (gain);
}


template class Limiter< float >;
template class Limiter< double >;


}  // namespace bav::dsp::FX
