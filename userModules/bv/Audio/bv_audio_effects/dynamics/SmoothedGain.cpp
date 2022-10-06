
namespace bav::dsp::FX
{
template < typename SampleType, size_t channels >
SmoothedGain< SampleType, channels >::SmoothedGain()
{
    for (size_t i = 0; i < channels; ++i)
        smoothers.add (new ValueSmoother< SampleType >());
}

template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::setGain (float gain)
{
    for (auto* smoother : smoothers)
        smoother->set (gain);
}

template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::prepare (double, int blocksize)
{
    lastBlocksize = blocksize;
    reset();
}

template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::process (AudioBuffer& audio)
{
    const auto numSamples = audio.getNumSamples();

    for (int chan = 0;
         chan < std::min (static_cast< int > (channels), audio.getNumChannels());
         ++chan)
    {
        smoothers[chan]->applyGain (audio.getWritePointer (chan), numSamples);
    }
}

template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::reset()
{
    for (auto* smoother : smoothers)
        smoother->reset (lastBlocksize);
}

template < typename SampleType, size_t channels >
void SmoothedGain< SampleType, channels >::skipSamples (int numSamples)
{
    for (auto* smoother : smoothers)
        for (int s = 0; s < numSamples; ++s)
            smoother->getNextValue();
}

template class SmoothedGain< float, 1 >;
template class SmoothedGain< double, 1 >;
template class SmoothedGain< float, 2 >;
template class SmoothedGain< double, 2 >;

}  // namespace bav::dsp::FX
