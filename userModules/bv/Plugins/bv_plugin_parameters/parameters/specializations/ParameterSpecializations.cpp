
namespace bav::plugin
{
GainParameter::GainParameter (String                                  paramNameShort,
                              String                                  paramNameVerbose,
                              float                                   defaultVal,
                              juce::AudioProcessorParameter::Category parameterCategory)
    : FloatParameter (-60.f, 0.f, defaultVal,
                      paramNameShort,
                      paramNameVerbose,
                      ParameterValueConversionLambdas::gain_stringFromFloat,
                      ParameterValueConversionLambdas::gain_floatFromString,
                      TRANS ("dB"), true, false, parameterCategory)
{
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

ToggleParameter::ToggleParameter (String paramNameShort,
                                  String paramNameVerbose,
                                  bool   defaultVal)
    : BoolParameter (defaultVal,
                     paramNameShort,
                     paramNameVerbose,
                     ParameterValueConversionLambdas::toggle_stringFromBool,
                     ParameterValueConversionLambdas::toggle_boolFromString)
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

FloatAmountParameter::FloatAmountParameter (String                                  paramNameShort,
                                            String                                  paramNameVerbose,
                                            float                                   defaultVal,
                                            juce::AudioProcessorParameter::Category parameterCategory,
                                            String                                  parameterLabel)
    : FloatParameter (0.f, 1.f, defaultVal,
                      paramNameShort,
                      paramNameVerbose,
                      ParameterValueConversionLambdas::normPcnt_stringFromInt,
                      ParameterValueConversionLambdas::normPcnt_intFromString,
                      parameterLabel, true, false, parameterCategory)
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

FrequencyParameter::FrequencyParameter (String paramNameShort,
                                        String paramNameVerbose,
                                        float  defaultVal)
    : FloatParameter (40.f, 10000.f, defaultVal,
                      paramNameShort,
                      paramNameVerbose,
                      ParameterValueConversionLambdas::hz_stringFromFloat,
                      ParameterValueConversionLambdas::hz_floatFromString,
                      TRANS ("Hz"))
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

PercentParameter::PercentParameter (String paramNameShort,
                                    String paramNameVerbose,
                                    int    defaultVal)
    : IntParameter (0, 100, defaultVal,
                    paramNameShort,
                    paramNameVerbose,
                    ParameterValueConversionLambdas::pcnt_stringFromInt,
                    ParameterValueConversionLambdas::pcnt_intFromString,
                    "%")
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

MeterParameter::MeterParameter (float min, float max, float defaultVal,
                                String paramNameShort, String paramNameVerbose,
                                std::function< String (float, int) >    stringFromValue,
                                std::function< float (String) >         valueFromString,
                                String                                  parameterLabel,
                                juce::AudioProcessorParameter::Category parameterCategory)

    : FloatParameter (min, max, defaultVal,
                      paramNameShort,
                      paramNameVerbose,
                      stringFromValue,
                      valueFromString,
                      parameterLabel,
                      false, false, parameterCategory)
{
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


GainMeterParameter::GainMeterParameter (String                                  paramNameShort,
                                        String                                  paramNameVerbose,
                                        juce::AudioProcessorParameter::Category parameterCategory)

    : MeterParameter (-60.f, 0.f, -60.f,
                      paramNameShort,
                      paramNameVerbose,
                      ParameterValueConversionLambdas::gain_stringFromFloat,
                      ParameterValueConversionLambdas::gain_floatFromString,
                      TRANS ("dB"), parameterCategory)
{
}

}  // namespace bav::plugin
