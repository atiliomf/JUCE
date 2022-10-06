
namespace bav::dsp
{
template < typename SampleType >
AudioAndMidiFIFO< SampleType >::AudioAndMidiFIFO (int channels, int samples)
{
    setSize (channels, samples);
}

template < typename SampleType >
void AudioAndMidiFIFO< SampleType >::setSize (int numChannels, int numSamples)
{
    audio.setNumChannels (numChannels);
    audio.setMaximumSize (numSamples);
    midi.setSize (numSamples);
}

template < typename SampleType >
void AudioAndMidiFIFO< SampleType >::push (const AudioBuffer& audioIn, const MidiBuffer& midiIn)
{
    audio.pushSamples (audioIn);
    midi.pushEvents (midiIn, audioIn.getNumSamples());
}

template < typename SampleType >
void AudioAndMidiFIFO< SampleType >::pop (AudioBuffer& audioOut, MidiBuffer& midiOut)
{
    audio.popSamples (audioOut);
    midi.popEvents (midiOut, audioOut.getNumSamples());
}

template < typename SampleType >
int AudioAndMidiFIFO< SampleType >::numStoredSamples() const
{
    return audio.numStoredSamples();
}

template class AudioAndMidiFIFO< float >;
template class AudioAndMidiFIFO< double >;

}  // namespace bav::dsp
