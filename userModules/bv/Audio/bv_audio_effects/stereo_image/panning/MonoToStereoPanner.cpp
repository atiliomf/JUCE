
namespace bav::dsp::FX
{
template < typename SampleType >
void MonoToStereoPanner< SampleType >::prepare (double sr, int blocksize)
{
    left.prepare (sr, blocksize);
    right.prepare (sr, blocksize);
}

template < typename SampleType >
void MonoToStereoPanner< SampleType >::reset()
{
    left.reset();
    right.reset();
}


template < typename SampleType >
void MonoToStereoPanner< SampleType >::process (const AudioBuffer& monoInput,
                                                AudioBuffer&       stereoOutput)
{
    stereoOutput.clear();
    jassert (stereoOutput.getNumChannels() >= 2);
    jassert (monoInput.getNumSamples() == stereoOutput.getNumSamples());

    left.setGain (PannerBase::getLeftGain());
    right.setGain (PannerBase::getRightGain());

    const auto numSamples = stereoOutput.getNumSamples();

    vecops::copy (monoInput.getReadPointer (0), stereoOutput.getWritePointer (0), numSamples);
    vecops::copy (monoInput.getReadPointer (0), stereoOutput.getWritePointer (1), numSamples);

    AudioBuffer leftAlias {stereoOutput.getArrayOfWritePointers(), 1, numSamples};
    AudioBuffer rightAlias {stereoOutput.getArrayOfWritePointers() + 1, 1, numSamples};

    left.process (leftAlias);
    right.process (rightAlias);
}

template class MonoToStereoPanner< float >;
template class MonoToStereoPanner< double >;

}  // namespace bav::dsp::FX
