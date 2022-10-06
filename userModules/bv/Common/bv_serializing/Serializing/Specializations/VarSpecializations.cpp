
namespace bav
{
template <>
juce::var toVar (juce::Point< float >& point)
{
    return String (point.x) + "_" + String (point.y);
}

template <>
juce::Point< float > fromVar (juce::var var)
{
    const auto string = var.toString();

    juce::Point< float > point;

    point.x = string.upToFirstOccurrenceOf ("_", false, true).getFloatValue();
    point.y = string.fromFirstOccurrenceOf ("_", false, true).getFloatValue();

    return point;
}

}  // namespace bav
