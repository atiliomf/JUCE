
#pragma once

namespace bav::plugin
{
struct GainParameter : public FloatParameter
{
    GainParameter (String                                  paramNameShort,
                   String                                  paramNameVerbose,
                   float                                   defaultVal,
                   juce::AudioProcessorParameter::Category parameterCategory =
                       juce::AudioProcessorParameter::genericParameter);

    using Listener = FloatParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

struct ToggleParameter : public BoolParameter
{
    ToggleParameter (String paramNameShort,
                     String paramNameVerbose,
                     bool   defaultVal);

    using Listener = BoolParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/* This parameter is for a value in a normalized 0.0-1.0 range */
struct FloatAmountParameter : public FloatParameter
{
    FloatAmountParameter (String                                  paramNameShort,
                          String                                  paramNameVerbose,
                          float                                   defaultVal,
                          juce::AudioProcessorParameter::Category parameterCategory =
                              juce::AudioProcessorParameter::genericParameter,
                          String parameterLabel = String());

    using Listener = FloatParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/* This parameter is for a frequency in Hz */
struct FrequencyParameter : public FloatParameter
{
    FrequencyParameter (String paramNameShort,
                        String paramNameVerbose,
                        float  defaultVal);

    using Listener = FloatParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/* This parameter is for an integer value in the range 0 - 100 */
struct PercentParameter : public IntParameter
{
    PercentParameter (String paramNameShort,
                      String paramNameVerbose,
                      int    defaultVal);

    using Listener = IntParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

struct MeterParameter : public FloatParameter
{
    MeterParameter (float min, float max, float defaultVal,
                    String paramNameShort, String paramNameVerbose,
                    std::function< String (float, int) >    stringFromValue   = nullptr,
                    std::function< float (String) >         valueFromString   = nullptr,
                    String                                  parameterLabel    = {},
                    juce::AudioProcessorParameter::Category parameterCategory = juce::AudioProcessorParameter::genericParameter);

    using Listener = FloatParameter::Listener;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


struct GainMeterParameter : public MeterParameter
{
    GainMeterParameter (String                                  paramNameShort,
                        String                                  paramNameVerbose,
                        juce::AudioProcessorParameter::Category parameterCategory =
                            juce::AudioProcessorParameter::genericParameter);

    using Listener = MeterParameter::Listener;
};

}  // namespace bav::plugin
