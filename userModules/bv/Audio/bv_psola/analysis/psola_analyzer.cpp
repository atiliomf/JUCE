
namespace bav::dsp::psola
{
template < typename SampleType >
void Analyzer< SampleType >::prepare (double sampleRate, int blocksize)
{
    samplerate = sampleRate;
    pitchDetector.setSamplerate (sampleRate);

    grainExtractor.prepare (blocksize);
    grainStorage.prepare (blocksize);
}

template < typename SampleType >
void Analyzer< SampleType >::analyzeInput (const AudioBuffer& input, int channel)
{
    analyzeInput (input.getReadPointer (channel), input.getNumSamples());
}

template < typename SampleType >
void Analyzer< SampleType >::analyzeInput (const SampleType* samples, int numSamples)
{
    jassert (samplerate > 0);

    const auto pitchInHz = pitchDetector.detectPitch (samples, numSamples);

    const auto period = pitchInHz > 0.f ? math::periodInSamples (samplerate, pitchInHz)
                                        : getNextUnpitchedPeriod();

    grainExtractor.analyzeInput (samples, numSamples, period);
    grainStorage.storeNewFrame (samples, numSamples, grainExtractor.getIndices());

    currentPeriod = period;

    broadcaster.trigger();
}

template < typename SampleType >
int Analyzer< SampleType >::getGrainLength() const
{
    return currentPeriod * 2;
}

template < typename SampleType >
int Analyzer< SampleType >::getLatencySamples() const
{
    return pitchDetector.getLatencySamples();
}

template < typename SampleType >
int Analyzer< SampleType >::getNextUnpitchedPeriod()
{
    return rand.nextInt (pitchDetector.getCurrentLegalPeriodRange());
}

template < typename SampleType >
const AnalysisGrainStorage< SampleType >& Analyzer< SampleType >::getStorage() const
{
    return grainStorage;
}

template < typename SampleType >
events::Broadcaster& Analyzer< SampleType >::getBroadcaster()
{
    return broadcaster;
}

template < typename SampleType >
int Analyzer< SampleType >::getPeriod() const
{
    return currentPeriod;
}

template < typename SampleType >
float Analyzer< SampleType >::getFrequency() const
{
    return (float) math::freqFromPeriod (samplerate, currentPeriod);
}


template class Analyzer< float >;
template class Analyzer< double >;

}  // namespace bav::dsp::psola
