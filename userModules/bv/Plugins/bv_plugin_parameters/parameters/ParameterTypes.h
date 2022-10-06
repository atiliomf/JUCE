#pragma once

namespace bav::plugin
{
template < typename ValueType >
class TypedParameter : public Parameter
{
public:
    TypedParameter (ValueType                                  minimum,
                    ValueType                                  maximum,
                    ValueType                                  defaultValue,
                    String                                     paramNameShort,
                    String                                     paramNameVerbose,
                    std::function< String (ValueType, int) >   stringFromValue   = nullptr,
                    std::function< ValueType (const String&) > valueFromString   = nullptr,
                    String                                     paramLabel        = {},
                    bool                                       isAutomatable     = true,
                    bool                                       metaParam         = false,
                    juce::AudioProcessorParameter::Category    parameterCategory = AudioProcessorParameter::genericParameter);

    const juce::NormalisableRange< float >& getNormalisableRange() const;

    ValueType get() const;
    void      set (ValueType newValue);

    ValueType getDefault() const;
    void      setDefault (ValueType newDefault);

    ValueType getMinimum() const;
    ValueType getMaximum() const;

    String getStringForValue (ValueType value, int maxLength = 50) const;
    String getStringForCurrentValue (int maxLength = 50) const;

    ValueType getValueForString (const String& string) const;

    //--------------------------------------

    struct Listener : Parameter::Listener
    {
        Listener (TypedParameter< ValueType >& param);

        virtual void paramValueChanged (ValueType) { }
        virtual void paramDefaultChanged (ValueType) { }

    private:
        void parameterValueChanged (float) final;
        void parameterDefaultChanged (float) final;

        TypedParameter< ValueType >& parameter;
    };

    //--------------------------------------

private:
    juce::NormalisableRange< float > createRange (ValueType minimum, ValueType maximum) const;

    std::function< String (ValueType, int) >   createDefaultStringFromValueFunc() const;
    std::function< ValueType (const String&) > createDefaultValueFromStringFunc() const;

    void serialize (TreeReflector& ref) final;

    const juce::NormalisableRange< float > range;

    std::function< String (ValueType, int) >   stringFromValueFunction;
    std::function< ValueType (const String&) > valueFromStringFunction;
};


using FloatParameter = TypedParameter< float >;
using IntParameter   = TypedParameter< int >;


struct BoolParameter : TypedParameter< bool >
{
    BoolParameter (bool                                    defaultValue,
                   String                                  paramNameShort,
                   String                                  paramNameVerbose,
                   std::function< String (bool, int) >     stringFromValue   = nullptr,
                   std::function< bool (const String&) >   valueFromString   = nullptr,
                   String                                  paramLabel        = {},
                   bool                                    isAutomatable     = true,
                   bool                                    metaParam         = false,
                   juce::AudioProcessorParameter::Category parameterCategory = AudioProcessorParameter::genericParameter);
};

}  // namespace bav::plugin
