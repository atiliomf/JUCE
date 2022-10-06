#pragma once

namespace bav
{
template <>
juce::var toVar (juce::Point< float >& point);

template <>
juce::Point< float > fromVar (juce::var var);

}  // namespace bav
