namespace bav::dsp::FX
{
template < typename SampleType >
void Manager< SampleType >::add (AudioEffect< SampleType >& effect)
{
    effects.add (&effect);
}

template < typename SampleType >
void Manager< SampleType >::prepare (double samplerate, int blocksize)
{
    for (auto* effect : effects)
        effect->prepare (samplerate, blocksize);

    storage.setSize (2, blocksize, true, true, true);
}

template < typename SampleType >
void Manager< SampleType >::process (const AudioBuffer& input)
{
    buffers::copy (input, storage);

    lastBlocksize = input.getNumSamples();

    storageAlias.setDataToReferTo (storage.getArrayOfWritePointers(), storage.getNumChannels(), 0, lastBlocksize);

    for (auto* effect : effects)
        effect->process (storageAlias);
}

template < typename SampleType >
const juce::AudioBuffer< SampleType >& Manager< SampleType >::getProcessedSignal()
{
    storageAlias.setDataToReferTo (storage.getArrayOfWritePointers(), storage.getNumChannels(), 0, lastBlocksize);
    return storageAlias;
}

template class Manager< float >;
template class Manager< double >;

}  // namespace bav::dsp::FX
