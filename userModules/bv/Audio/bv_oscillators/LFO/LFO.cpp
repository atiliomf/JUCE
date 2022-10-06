
namespace bav::dsp
{
template < typename SampleType >
void LFO< SampleType >::prepared (int blocksize)
{
    storage.setSize (1, blocksize, true, true, true);
}

template < typename SampleType >
void LFO< SampleType >::process (int numSamples)
{
    AudioBuffer alias {storage.getArrayOfWritePointers(), 1, numSamples};
    osc::ChoosableOscillator< SampleType >::process (alias);
}

template class LFO< float >;
template class LFO< double >;

}  // namespace bav::dsp
