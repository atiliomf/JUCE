
namespace bav::dsp::osc
{
template < typename SampleType >
SuperSaw< SampleType >::SuperSaw()
{
    while (saws.size() < 7)
        saws.add (new Saw< SampleType >());
}

template < typename SampleType >
void SuperSaw< SampleType >::resetPhase()
{
    for (auto* saw : saws)
        saw->resetPhase();
}

template < typename SampleType >
void SuperSaw< SampleType >::setFrequency (SampleType frequency, SampleType sampleRate)
{
    lastFrequency = frequency;
    samplerate    = sampleRate;

    const auto spreadSemitones = (float) totalSpreadCents * 0.01f;
    const auto increment       = spreadSemitones / (float) saws.size();
    const auto centerPitch     = math::freqToMidi (frequency);

    auto lowBound = centerPitch - (spreadSemitones * 0.5f);

    for (auto* saw : saws)
    {
        saw->setFrequency ((SampleType) math::midiToFreq (lowBound), sampleRate);
        lowBound += increment;
    }
}

template < typename SampleType >
void SuperSaw< SampleType >::setDetuneAmount (int totalPitchSpreadInCents)
{
    totalSpreadCents = totalPitchSpreadInCents;
    setFrequency (lastFrequency, samplerate);
}

template < typename SampleType >
int SuperSaw< SampleType >::getPitchSpreadCents() const
{
    return totalSpreadCents;
}

template < typename SampleType >
SampleType SuperSaw< SampleType >::getSample()
{
    auto sample = (SampleType) 0.;

    for (auto* saw : saws)
        sample += saw->getSample();

    return sample;
}

template class SuperSaw< float >;
template class SuperSaw< double >;

}  // namespace bav::dsp::osc
