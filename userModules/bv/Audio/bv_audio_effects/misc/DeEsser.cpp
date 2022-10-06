
namespace bav::dsp::FX
{
template < typename SampleType >
DeEsser< SampleType >::DeEsser()
{
    gate.setAttack (attackMs);
    gate.setRelease (releaseMs);
    gate.setInverted (true);
}

template < typename SampleType >
void DeEsser< SampleType >::prepare (double samplerate, int blocksize)
{
    filter.coefs.makeHighPass (samplerate, static_cast< SampleType > (hiPassFreq));
    filter.prepare();

    gate.prepare (samplerate, blocksize);
}

template < typename SampleType >
void DeEsser< SampleType >::reset()
{
    filter.reset();
    gate.reset();
}


template < typename SampleType >
void DeEsser< SampleType >::setThresh (float newThresh_dB)
{
    gate.setThreshold (newThresh_dB);
}


template < typename SampleType >
void DeEsser< SampleType >::setDeEssAmount (int newAmount)
{
    jassert (newAmount >= 0 && newAmount <= 100);

    const auto a = static_cast< float > (newAmount) * 0.01f;
    gate.setRatio (static_cast< SampleType > (juce::jmap (a, 0.0f, 1.0f, 1.0f, 10.0f)));
}

template < typename SampleType >
SampleType DeEsser< SampleType >::processChannel (int               channel,
                                                  int               numSamples,
                                                  SampleType*       signalToDeEss,
                                                  const SampleType* sidechain)
{
    filter.processChannel (channel, signalToDeEss, numSamples);

    return gate.processChannel (channel, numSamples, signalToDeEss, sidechain);
}

template class DeEsser< float >;
template class DeEsser< double >;

}  // namespace bav::dsp::FX
