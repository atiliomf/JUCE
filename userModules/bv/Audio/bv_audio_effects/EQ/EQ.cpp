
namespace bav::dsp::FX
{
template < typename SampleType >
void EQ< SampleType >::process (juce::AudioBuffer< SampleType >& audio)
{
    jassert (getNumBands() > 0);

    for (auto* filter : filters)
        filter->process (audio);
}

template < typename SampleType >
void EQ< SampleType >::prepare (double samplerate, int blocksize)
{
    for (auto* filter : filters)
        filter->prepare (samplerate, blocksize);

    lastSamplerate = samplerate;
    lastBlocksize  = blocksize;
}

template < typename SampleType >
void EQ< SampleType >::addBand (Band* newFilter)
{
    filters.add (newFilter);
    newFilter->prepare (lastSamplerate, lastBlocksize);
}

template < typename SampleType >
int EQ< SampleType >::getNumBands() const
{
    return filters.size();
}

template < typename SampleType >
Filter< SampleType >* EQ< SampleType >::getBand (int index)
{
    if (index > filters.size()) return nullptr;
    return filters[index];
}

template < typename SampleType >
Filter< SampleType >* EQ< SampleType >::getBandOfType (FilterType type)
{
    for (auto* filter : filters)
        if (filter->getFilterType() == type)
            return filter;

    return nullptr;
}

template class EQ< float >;
template class EQ< double >;

}  // namespace bav::dsp::FX
