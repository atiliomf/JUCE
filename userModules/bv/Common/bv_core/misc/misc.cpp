
namespace bav
{
bool thisIsTheMessageThread() noexcept
{
    return juce::MessageManager::getInstance()->isThisTheMessageThread();
}

const juce::juce_wchar getSharpSymbol() noexcept
{
    return *juce::CharPointer_UTF8 ("\xe2\x99\xaf");
}

const juce::juce_wchar getFlatSymbol() noexcept
{
    return *juce::CharPointer_UTF8 ("\xe2\x99\xad");
}

const juce::juce_wchar getNaturalSymbol() noexcept
{
    return *juce::CharPointer_UTF8 ("\xe2\x99\xae");
}

int midiPanStringToInt (const String& string)
{
    if (string.endsWithIgnoreCase (TRANS ("C"))) return 64;

    if (string.endsWithIgnoreCase (TRANS ("R")))
        return juce::jmap (
            string.dropLastCharacters (1).getIntValue(), 1, 50, 65, 127);

    if (string.endsWithIgnoreCase (TRANS ("L")))
        return juce::jmap (
            string.dropLastCharacters (1).getIntValue(), 1, 50, 63, 0);

    return string.getIntValue();
}

String midiPanIntToString (const int midiPan)
{
    if (midiPan == 64) return TRANS ("C");

    if (midiPan > 64)
    {
        const auto amtRight = juce::jmap (midiPan, 65, 127, 1, 50);
        return String (amtRight) + TRANS ("R");
    }

    const auto amtLeft = juce::jmap (midiPan, 63, 0, 1, 50);
    return String (amtLeft) + TRANS ("L");
}

String pitchToString (const int  midiNoteNumber,
                      const bool asSharps)
{
    jassert (midiNoteNumber >= 0 && midiNoteNumber <= 127);

    const auto pitchClass = midiNoteNumber % 12;

    constexpr auto inv12  = 1.0f / 12.0f;
    const auto     octave = juce::roundToInt (float (midiNoteNumber) * inv12) - 1;

    static const char* const sharpNoteNames[] = {
        "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    static const char* const flatNoteNames[] = {
        "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};
    auto noteName = asSharps ? String (sharpNoteNames[pitchClass])
                             : String (flatNoteNames[pitchClass]);

    return noteName + String (octave);
}

int pitchClassFromString (const String& pitchClassName) noexcept
{
    int        pitchClass = -1;
    const auto numChars   = pitchClassName.length();

    if (numChars > 0)
    {
        const auto base = pitchClassName.toLowerCase()[0];

        switch (base)
        {
            case 'c' : pitchClass = 0; break;
            case 'd' : pitchClass = 2; break;
            case 'e' : pitchClass = 4; break;
            case 'f' : pitchClass = 5; break;
            case 'g' : pitchClass = 7; break;
            case 'a' : pitchClass = 9; break;
            case 'b' : pitchClass = 11; break;
            default : pitchClass = -1; break;
        }
    }

    if (numChars > 1)
    {
        const auto sharpOrFlat = pitchClassName[1];

        switch (sharpOrFlat)
        {
            case '#' : ++pitchClass; break;
            case 'b' : --pitchClass; break;
            default : break;
        }

        if (sharpOrFlat == getSharpSymbol())
            ++pitchClass;
        else if (sharpOrFlat == getFlatSymbol())
            --pitchClass;

        pitchClass %= 12;
    }

    return pitchClass;
}

int stringToPitch (const String& string)
{
    const auto octaveName = string.retainCharacters ("0123456789");

    const auto pitchClassTokens = juce::String ("abcdefg#") + getSharpSymbol()
                                + getFlatSymbol() + getNaturalSymbol();
    const auto pitchClassName =
        string.toLowerCase().retainCharacters (pitchClassTokens);

    const auto octave     = octaveName.getIntValue();
    const auto pitchClass = pitchClassFromString (pitchClassName);
    jassert (pitchClass > -1);

    return (octave * 12) + pitchClass;
}

SystemInitializer::SystemInitializer()
{
#if BV_USE_NE10
    ne10_init();
#endif

    initializeDefaultTranslations();
}

}  // namespace bav
