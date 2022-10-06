#pragma once

#include "Coeffecients.h"

namespace bav::dsp::filters
{
template < typename SampleType >
class Filter
{
public:
    void reset (SampleType resetToValue = SampleType (0));
    void prepare() noexcept;

    void process (SampleType* buffer, int numSamples);

    Coefficients< SampleType > coefs;

private:
    void snapToZero() noexcept;

    juce::Array< SampleType > state;
    int                       order = 0;
};

/*--------------------------------------------------------------------------------------------------------------*/

template < typename SampleType, size_t numChannels = 2 >
struct MultiFilter
{
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    MultiFilter();

    void reset (SampleType resetToValue = SampleType (0));
    void prepare() noexcept;

    void process (AudioBuffer& buffer) noexcept;
    void processChannel (int channel, SampleType* audio, int numSamples) noexcept;

    Coefficients< SampleType >               coefs;
    juce::OwnedArray< Filter< SampleType > > filters;
};

}  // namespace bav::dsp::filters
