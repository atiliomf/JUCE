
#pragma once

#include <bv_dsp/bv_dsp.h>

namespace bav::ParameterValueConversionLambdas
{
/* Boolean toggles */
static const auto
    toggle_stringFromBool = [] (bool value, int maxLength)
{
    return value ? TRANS ("On").substring (0, maxLength)
                 : TRANS ("Off").substring (0, maxLength);
};

static const auto toggle_boolFromString =
    [] (const String& text)
{
    return (text.containsIgnoreCase (TRANS ("On"))
            || text.containsIgnoreCase (TRANS ("Yes"))
            || text.containsIgnoreCase (TRANS ("True")));
};

/* Float gain */
static const auto
    gain_stringFromFloat = [] (float value, int maxLength)
{
    return (String (value) + " " + TRANS ("dB")).substring (0, maxLength);
};

static const auto gain_floatFromString =
    [] (const String& text)
{
    const auto token_location = text.indexOfWholeWordIgnoreCase (TRANS ("dB"));

    if (token_location > -1)
        return text.substring (0, token_location).trim().getFloatValue();

    return text.trim().getFloatValue();
};

/* Integer percent */
static const auto
    pcnt_stringFromInt = [] (int value, int maxLength)
{
    return (String (value) + "%").substring (0, maxLength);
};

static const auto pcnt_intFromString =
    [] (const String& text)
{
    const auto token_location = text.indexOf ("%");

    if (token_location > -1)
        return text.substring (0, token_location).trim().getIntValue();

    return text.trim().getIntValue();
};

/* Seconds (as float) */
static const auto
    sec_stringFromFloat = [] (float value, int maxLength)
{
    return (String (value) + " " + TRANS ("sec")).substring (0, maxLength);
};

static const auto sec_floatFromString =
    [] (const String& text)
{
    const auto token_location = text.indexOfWholeWordIgnoreCase (TRANS ("sec"));

    if (token_location > -1)
        return text.substring (0, token_location).trim().getFloatValue();

    return text.trim().getFloatValue();
};

/* Hz - as float */
static const auto
    hz_stringFromFloat = [] (float value, int maxLength)
{
    auto string = (value < 1000.0f)
                    ? String (value) + " " + TRANS ("Hz")
                    : String (value * 0.001f) + " " + TRANS ("kHz");

    return string.substring (0, maxLength);
};

static const auto hz_floatFromString =
    [] (const String& text)
{
    const auto kHz_token_location = text.indexOfWholeWordIgnoreCase (TRANS ("kHz"));

    if (kHz_token_location > -1)
        return text.substring (0, kHz_token_location).trim().getFloatValue()
             * 1000.0f;

    const auto hz_token_location = text.indexOfWholeWordIgnoreCase (TRANS ("Hz"));

    if (hz_token_location > -1)
        return text.substring (0, hz_token_location).trim().getFloatValue();

    return text.trim().getFloatValue();
};

/* Semitones (as integer) */
static const auto
    st_stringFromInt = [] (int value, int maxLength)
{
    return (String (value) + " " + TRANS ("st")).substring (0, maxLength);
};

static auto st_intFromString =
    [] (const String& text)
{
    const auto token_location = text.indexOfWholeWordIgnoreCase (TRANS ("st"));

    if (token_location > -1)
        return text.substring (0, token_location).trim().getIntValue();

    return text.trim().getIntValue();
};

/* Midipitch (as integer) */
static const auto
    pitch_stringFromInt = [] (int value, int maxLength)
{
    return pitchToString (value, true).substring (0, maxLength);
};

static const auto pitch_intFromString =
    [] (const String& text)
{
    static const auto pitchClassTokens =
        String ("AaBbCcDdEeFfGg#") + getSharpSymbol() + getFlatSymbol();

    if (text.containsAnyOf (pitchClassTokens)) return stringToPitch (text.trim());

    return text.trim().getIntValue();
};

/* Normalized percent as integer */
static const auto
    normPcnt_stringFromInt = [] (int value, int maxLength)
{
    return (String (value * 100.0f) + "%").substring (0, maxLength);
};

static const auto normPcnt_intFromString =
    [] (const String& text)
{
    const auto token_location = text.indexOf ("%");

    if (token_location > -1)
        return text.substring (0, token_location).trim().getFloatValue() * 0.01f;

    return text.trim().getFloatValue();
};

/* MIDI pan -> string (eg, 127 = 50R, 64 = C, 0 = 50L) */
static const auto
    midiPan_stringFromInt = [] (int value, int maxLength)
{
    return midiPanIntToString (value).substring (0, maxLength);
};

static const auto midiPan_intFromString =
    [] (const String& text)
{
    return midiPanStringToInt (text);
};

}  // namespace bav::ParameterValueConversionLambdas
