
#pragma once

namespace bav
{
template <>
juce::var toVar (juce::AudioBuffer< float >& buffer);

template <>
juce::var toVar (juce::AudioBuffer< double >& buffer);

template <>
juce::AudioBuffer< float > fromVar (juce::var var);

template <>
juce::AudioBuffer< double > fromVar (juce::var var);

}  // namespace bav
