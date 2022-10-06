
#pragma once

namespace bav::dsp::FX
{
template < typename SampleType >
class MonoStereoConverter
{
    using Buffer = juce::AudioBuffer< SampleType >;

public:
    MonoStereoConverter();
    virtual ~MonoStereoConverter() = default;

    enum StereoReductionMode
    {
        leftOnly,
        rightOnly,
        mixToMono
    };

    void prepare (int blocksize);

    void                setStereoReductionMode (StereoReductionMode newmode);
    StereoReductionMode getStereoReductionMode() { return toMonoMode; }

    void convertStereoToMono (const SampleType* leftIn,
                              const SampleType* rightIn,
                              SampleType*       monoOut,
                              int               numSamples);

    void convertStereoToMono (const Buffer& input, Buffer& output);

    void convertMonoToStereo (const SampleType* monoIn,
                              SampleType*       leftOut,
                              SampleType*       rightOut,
                              int               numSamples);

    void convertMonoToStereo (const Buffer& input, Buffer& output);


private:
    StereoReductionMode toMonoMode;

    juce::AudioBuffer< SampleType > monoStorage;
};

}  // namespace bav::dsp::FX
