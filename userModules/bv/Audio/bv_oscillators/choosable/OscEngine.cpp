
namespace bav::dsp::osc
{
template < typename SampleType, template < typename T > class OscType >
OscEngine< SampleType, OscType >::OscEngine()
{
}

template < typename SampleType, template < typename T > class OscType >
OscType< SampleType >* OscEngine< SampleType, OscType >::operator->()
{
    return &osc;
}

template < typename SampleType, template < typename T > class OscType >
void OscEngine< SampleType, OscType >::setFrequency (float freqHz)
{
    frequency = freqHz;
    osc.setFrequency (frequency, (SampleType) this->getSamplerate());
}

template < typename SampleType, template < typename T > class OscType >
void OscEngine< SampleType, OscType >::renderBlock (const AudioBuffer&,
                                                    AudioBuffer& output,
                                                    MidiBuffer&, bool)
{
    osc.getSamples (output.getWritePointer (0), output.getNumSamples());
}

template < typename SampleType, template < typename T > class OscType >
void OscEngine< SampleType, OscType >::prepared (int, double samplerate)
{
    osc.resetPhase();
    osc.setFrequency (frequency, (SampleType) samplerate);
}


#define BV_DECLARE_OSC_ENGINE(Class)          \
    template class OscEngine< float, Class >; \
    template class OscEngine< double, Class >;

BV_DECLARE_OSC_ENGINE (Sine)
BV_DECLARE_OSC_ENGINE (Saw)
BV_DECLARE_OSC_ENGINE (Square)
BV_DECLARE_OSC_ENGINE (Triangle)
BV_DECLARE_OSC_ENGINE (SuperSaw)

#undef BV_DECLARE_OSC_ENGINE

}  // namespace bav::dsp::osc
