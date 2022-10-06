#pragma once

namespace bav::dsp::buffers
{
template < typename SampleType >
void copy (const juce::AudioBuffer< SampleType >& source, juce::AudioBuffer< SampleType >& dest);

template < typename Type1, typename Type2 >
void convert (const juce::AudioBuffer< Type1 >& source, juce::AudioBuffer< Type2 >& dest);

}  // namespace bav::dsp::buffers
