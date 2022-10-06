
namespace bav::dsp::FX
{
template < typename SampleType >
MonoStereoConverter< SampleType >::MonoStereoConverter()
    : toMonoMode (leftOnly), monoStorage (1, 0)
{
}

template < typename SampleType >
void MonoStereoConverter< SampleType >::prepare (int blocksize)
{
    monoStorage.setSize (1, blocksize, true, true, true);
}


template < typename SampleType >
void MonoStereoConverter< SampleType >::setStereoReductionMode (StereoReductionMode newmode)
{
    toMonoMode = newmode;
}


template < typename SampleType >
void MonoStereoConverter< SampleType >::convertStereoToMono (const SampleType* leftIn,
                                                             const SampleType* rightIn,
                                                             SampleType*       monoOut,
                                                             int               numSamples)
{
    switch (toMonoMode)
    {
        case (leftOnly) :
        {
            vecops::copy (leftIn, monoOut, numSamples);
        }
        case (rightOnly) :
        {
            vecops::copy (rightIn, monoOut, numSamples);
        }
        case (mixToMono) :
        {
            monoStorage.copyFrom (0, 0, leftIn, numSamples, SampleType (0.5));
            monoStorage.addFrom (0, 0, rightIn, numSamples, SampleType (0.5));
            vecops::copy (monoStorage.getReadPointer (0), monoOut, numSamples);
        }
    }
}

template < typename SampleType >
void MonoStereoConverter< SampleType >::convertStereoToMono (const Buffer& input, Buffer& output)
{
    jassert (input.getNumSamples() == output.getNumSamples());

    if (input.getNumChannels() == 1)
    {
        dsp::buffers::copy (input, output);
    }
    else
    {
        convertStereoToMono (input.getReadPointer (0),
                             input.getReadPointer (1),
                             output.getWritePointer (0),
                             output.getNumSamples());
    }
}


template < typename SampleType >
void MonoStereoConverter< SampleType >::convertMonoToStereo (const SampleType* monoIn,
                                                             SampleType*       leftOut,
                                                             SampleType*       rightOut,
                                                             int               numSamples)
{
    vecops::copy (monoIn, leftOut, numSamples);
    vecops::copy (monoIn, rightOut, numSamples);
}

template < typename SampleType >
void MonoStereoConverter< SampleType >::convertMonoToStereo (const Buffer& input, Buffer& output)
{
    jassert (input.getNumSamples() == output.getNumSamples());
    jassert (output.getNumChannels() >= 2);

    convertMonoToStereo (input.getReadPointer (0),
                         output.getWritePointer (0),
                         output.getWritePointer (1),
                         output.getNumSamples());
}

template class MonoStereoConverter< float >;
template class MonoStereoConverter< double >;

}  // namespace bav::dsp::FX
