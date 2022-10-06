namespace bav::plugin
{
template < typename ValueType >
TypedParameter< ValueType >::TypedParameter (ValueType minimum,
                                             ValueType maximum,
                                             ValueType defaultValue,
                                             String    paramNameShort,
                                             String    paramNameVerbose,
                                             std::function< String (ValueType, int) >
                                                 stringFromValue,
                                             std::function< ValueType (const String&) >
                                                                                     valueFromString,
                                             String                                  paramLabel,
                                             bool                                    isAutomatable,
                                             bool                                    metaParam,
                                             juce::AudioProcessorParameter::Category parameterCategory)
    : Parameter (paramNameShort, paramNameVerbose, paramLabel, isAutomatable, metaParam, parameterCategory),
      range (createRange (minimum, maximum)),
      stringFromValueFunction (stringFromValue),
      valueFromStringFunction (valueFromString)
{
    if (stringFromValueFunction == nullptr)
        stringFromValueFunction = createDefaultStringFromValueFunc();

    if (valueFromStringFunction == nullptr)
        valueFromStringFunction = createDefaultValueFromStringFunc();
}


template <>
juce::NormalisableRange< float > TypedParameter< float >::createRange (float minimum, float maximum) const
{
    return {minimum, maximum, 0.01f};
}

template <>
juce::NormalisableRange< float > TypedParameter< int >::createRange (int minimum, int maximum) const
{
    juce::NormalisableRange< float > rangeWithInterval {(float) minimum, (float) maximum,
                                                        [] (float start, float end, float v)
                                                        { return juce::jlimit (start, end, v * (end - start) + start); },
                                                        [] (float start, float end, float v)
                                                        { return juce::jlimit (0.f, 1.f, (v - start) / (end - start)); },
                                                        [] (float start, float end, float v)
                                                        { return (float) juce::roundToInt (juce::jlimit (start, end, v)); }};
    rangeWithInterval.interval = 1.f;
    return rangeWithInterval;
}

template <>
juce::NormalisableRange< float > TypedParameter< bool >::createRange (bool, bool) const
{
    return {0.f, 1.f, 1.f};
}


template <>
std::function< String (float, int) > TypedParameter< float >::createDefaultStringFromValueFunc() const
{
    auto numDecimalPlacesToDisplay = [this]
    {
        int numDecimalPlaces = 7;

        if (range.interval != 0.0f)
        {
            if (juce::approximatelyEqual (std::abs (range.interval - std::floor (range.interval)), 0.0f))
                return 0;

            auto v = std::abs (juce::roundToInt (range.interval * pow (10, numDecimalPlaces)));

            while ((v % 10) == 0 && numDecimalPlaces > 0)
            {
                --numDecimalPlaces;
                v /= 10;
            }
        }

        return numDecimalPlaces;
    }();

    return [numDecimalPlacesToDisplay] (float v, int length)
    {
        String asText (v, numDecimalPlacesToDisplay);
        return length > 0 ? asText.substring (0, length) : asText;
    };
}

template <>
std::function< float (const String&) > TypedParameter< float >::createDefaultValueFromStringFunc() const
{
    return [] (const String& text)
    { return text.getFloatValue(); };
}

template <>
std::function< String (int, int) > TypedParameter< int >::createDefaultStringFromValueFunc() const
{
    return [] (int v, int num)
    { return num > 0 ? String (v).substring (0, num) : String (v); };
}

template <>
std::function< int (const String&) > TypedParameter< int >::createDefaultValueFromStringFunc() const
{
    return [] (const String& text)
    { return text.getIntValue(); };
}

template <>
std::function< String (bool, int) > TypedParameter< bool >::createDefaultStringFromValueFunc() const
{
    return [] (bool v, int)
    { return v ? TRANS ("On") : TRANS ("Off"); };
}

template <>
std::function< bool (const String&) > TypedParameter< bool >::createDefaultValueFromStringFunc() const
{
    juce::StringArray onStrings;
    onStrings.add (TRANS ("on"));
    onStrings.add (TRANS ("yes"));
    onStrings.add (TRANS ("true"));

    juce::StringArray offStrings;
    offStrings.add (TRANS ("off"));
    offStrings.add (TRANS ("no"));
    offStrings.add (TRANS ("false"));

    return [onStrings, offStrings] (const String& text)
    {
        String lowercaseText (text.toLowerCase());

        for (auto& testText : onStrings)
            if (lowercaseText == testText)
                return true;

        for (auto& testText : offStrings)
            if (lowercaseText == testText)
                return false;

        return text.getIntValue() != 0;
    };
}


template < typename ValueType >
const juce::NormalisableRange< float >& TypedParameter< ValueType >::getNormalisableRange() const
{
    return range;
}

template <>
float TypedParameter< float >::get() const
{
    return this->getDenormalizedValue();
}

template <>
int TypedParameter< int >::get() const
{
    return juce::roundToInt (this->getDenormalizedValue());
}

template <>
bool TypedParameter< bool >::get() const
{
    return this->getNormalizedValue() >= 0.5f;
}

template < typename ValueType >
void TypedParameter< ValueType >::set (ValueType newValue)
{
    this->setDenormalizedValue (static_cast< float > (newValue));
}

template <>
float TypedParameter< float >::getDefault() const
{
    return this->getDenormalizedDefault();
}

template <>
int TypedParameter< int >::getDefault() const
{
    return juce::roundToInt (this->getDenormalizedDefault());
}

template <>
bool TypedParameter< bool >::getDefault() const
{
    return this->getNormalizedDefault() >= 0.5f;
}

template < typename ValueType >
void TypedParameter< ValueType >::setDefault (ValueType newDefault)
{
    this->setDenormalizedDefault (static_cast< float > (newDefault));
}

template < typename ValueType >
String TypedParameter< ValueType >::getStringForValue (ValueType value, int maxLength) const
{
    return stringFromValueFunction (value, maxLength);
}

template < typename ValueType >
String TypedParameter< ValueType >::getStringForCurrentValue (int maxLength) const
{
    return stringFromValueFunction (get(), maxLength);
}

template < typename ValueType >
ValueType TypedParameter< ValueType >::getValueForString (const String& string) const
{
    return valueFromStringFunction (string);
}

template < typename ValueType >
ValueType TypedParameter< ValueType >::getMinimum() const
{
    return static_cast< ValueType > (range.start);
}

template < typename ValueType >
ValueType TypedParameter< ValueType >::getMaximum() const
{
    return static_cast< ValueType > (range.end);
}

template < typename ValueType >
void TypedParameter< ValueType >::serialize (TreeReflector& ref)
{
    auto& tree = ref.getRawDataTree();

    if (ref.isLoading())
    {
        setValueInternal (tree.getProperty ("Value", get()));
        setDefaultInternal (tree.getProperty ("DefaultValue", getDefault()));
        setMidiControllerInternal (tree.getProperty ("MidiControllerNumber", getMidiControllerNumber()));
    }
    else
    {
        tree.setProperty ("Value", get(), nullptr);
        tree.setProperty ("DefaultValue", getDefault(), nullptr);
        tree.setProperty ("MidiControllerNumber", getMidiControllerNumber(), nullptr);
    }
}

template < typename ValueType >
TypedParameter< ValueType >::Listener::Listener (TypedParameter< ValueType >& param)
    : Parameter::Listener (param), parameter (param)
{
}

template < typename ValueType >
void TypedParameter< ValueType >::Listener::parameterValueChanged (float)
{
    paramValueChanged (parameter.get());
}

template < typename ValueType >
void TypedParameter< ValueType >::Listener::parameterDefaultChanged (float)
{
    paramDefaultChanged (parameter.getDefault());
}


template class TypedParameter< float >;
template class TypedParameter< int >;
template class TypedParameter< bool >;


BoolParameter::BoolParameter (bool   defaultValue,
                              String paramNameShort,
                              String paramNameVerbose,
                              std::function< String (bool, int) >
                                  stringFromValue,
                              std::function< bool (const String&) >
                                                                      valueFromString,
                              String                                  paramLabel,
                              bool                                    isAutomatable,
                              bool                                    metaParam,
                              juce::AudioProcessorParameter::Category parameterCategory)
    : TypedParameter< bool > (false, true, defaultValue, paramNameShort, paramNameVerbose, stringFromValue, valueFromString, paramLabel, isAutomatable, metaParam, parameterCategory)
{
}

}  // namespace bav::plugin
