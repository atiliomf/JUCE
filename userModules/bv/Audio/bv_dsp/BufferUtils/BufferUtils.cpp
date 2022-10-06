
namespace bav::dsp::buffers
{
template < typename SampleType >
void copy (const juce::AudioBuffer< SampleType >& source, juce::AudioBuffer< SampleType >& dest)
{
    dest.clear();

    const auto numSamples = source.getNumSamples();
    jassert (dest.getNumSamples() >= numSamples);

    const auto numChannels = std::min (source.getNumChannels(), dest.getNumChannels());

    for (int chan = 0; chan < numChannels; ++chan)
        vecops::copy (source.getReadPointer (chan), dest.getWritePointer (chan), numSamples);
}

template void copy (const juce::AudioBuffer< float >&, juce::AudioBuffer< float >&);
template void copy (const juce::AudioBuffer< double >&, juce::AudioBuffer< double >&);

template < typename Type1, typename Type2 >
void convert (const juce::AudioBuffer< Type1 >& source, juce::AudioBuffer< Type2 >& dest)
{
    dest.clear();

    const auto numSamples = source.getNumSamples();
    jassert (dest.getNumSamples() >= numSamples);

    const auto numChannels = std::min (source.getNumChannels(), dest.getNumChannels());

    for (int chan = 0; chan < numChannels; ++chan)
        vecops::convert (dest.getWritePointer (chan), source.getReadPointer (chan), numSamples);
}
template void convert (const juce::AudioBuffer< float >&, juce::AudioBuffer< double >&);
template void convert (const juce::AudioBuffer< double >&, juce::AudioBuffer< float >&);

}  // namespace bav::dsp::buffers
