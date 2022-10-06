
namespace bav::dsp::FX
{
template < typename SampleType >
void StereoPanner< SampleType >::prepare (double samplerate, int blocksize)
{
    left.prepare (samplerate, blocksize);
    right.prepare (samplerate, blocksize);
}

template < typename SampleType >
void StereoPanner< SampleType >::reset()
{
    left.reset();
    right.reset();
}


template < typename SampleType >
void StereoPanner< SampleType >::process (const AudioBuffer& stereoInput,
                                          AudioBuffer&       stereoOutput)
{
    stereoOutput.clear();
    jassert (stereoInput.getNumChannels() >= 2
             && stereoOutput.getNumChannels() >= 2);
    jassert (stereoInput.getNumSamples() == stereoOutput.getNumSamples());

    left.setGain (PannerBase::getLeftGain());
    right.setGain (PannerBase::getRightGain());

    const auto numSamples = stereoInput.getNumSamples();

    vecops::copy (stereoInput.getReadPointer (0), stereoOutput.getWritePointer (0), numSamples);
    vecops::copy (stereoInput.getReadPointer (1), stereoOutput.getWritePointer (1), numSamples);

    AudioBuffer leftAlias {stereoOutput.getArrayOfWritePointers(), 1, numSamples};
    AudioBuffer rightAlias {stereoOutput.getArrayOfWritePointers() + 1, 1, numSamples};

    left.process (leftAlias);
    right.process (rightAlias);
}

template < typename SampleType >
void StereoPanner< SampleType >::process (AudioBuffer& audio)
{
    process (audio, audio);
}

template class StereoPanner< float >;
template class StereoPanner< double >;


}  // namespace bav::dsp::FX
