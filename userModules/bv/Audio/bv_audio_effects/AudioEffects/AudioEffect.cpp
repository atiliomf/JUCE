namespace bav::dsp::FX
{
template < typename SampleType >
void AudioEffect< SampleType >::prepare (double, int)
{
}

template < typename SampleType >
void AudioEffect< SampleType >::bypassedBlock (int)
{
}

template struct AudioEffect< float >;
template struct AudioEffect< double >;


template < typename SampleType >
void SidechainableAudioEffect< SampleType >::process (AudioBuffer& audio)
{
    process (audio, audio);
}

template struct SidechainableAudioEffect< float >;
template struct SidechainableAudioEffect< double >;


template < typename SampleType >
void LevelReportingAudioEffect< SampleType >::process (AudioBuffer& inOut, const AudioBuffer& sidechain)
{
    const auto numChannels = inOut.getNumChannels();

    if (numChannels == 0) return;

    const auto numSamples = inOut.getNumSamples();

    jassert (sidechain.getNumChannels() == numChannels);
    jassert (sidechain.getNumSamples() == numSamples);

    for (int chan = 0; chan < numChannels; ++chan)
    {
        gainReductions.set (chan,
                            processChannel (chan,
                                            numSamples,
                                            inOut.getWritePointer (chan),
                                            sidechain.getReadPointer (chan)));
    }
}

template < typename SampleType >
void LevelReportingAudioEffect< SampleType >::process (AudioBuffer& audio)
{
    process (audio, audio);
}

template < typename SampleType >
SampleType LevelReportingAudioEffect< SampleType >::getGainReduction (int channel) const
{
    if (channel < gainReductions.size())
        return gainReductions.getUnchecked (channel);

    return (SampleType) 0;
}

template < typename SampleType >
SampleType LevelReportingAudioEffect< SampleType >::getAverageGainReduction() const
{
    auto avg = (SampleType) 0;

    const auto numChannels = gainReductions.size();

    if (numChannels == 0) return avg;

    for (int i = 0; i < numChannels; ++i)
        avg += gainReductions.getUnchecked (i);

    avg /= static_cast< SampleType > (numChannels);

    return avg;
}

template < typename SampleType >
SampleType LevelReportingAudioEffect< SampleType >::getLevel (int channel) const
{
    return getGainReduction (channel);
}

template < typename SampleType >
SampleType LevelReportingAudioEffect< SampleType >::getAverageLevel() const
{
    return getAverageGainReduction();
}


template struct LevelReportingAudioEffect< float >;
template struct LevelReportingAudioEffect< double >;

}  // namespace bav::dsp::FX
