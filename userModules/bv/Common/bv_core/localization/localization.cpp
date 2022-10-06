
namespace bav
{
static String getCountryCodeToUse()
{
    return juce::SystemStats::getDisplayLanguage().upToFirstOccurrenceOf ("-", false, false);
}


RawData getDefaultTranslationFile (String defaultCountryCode = "en")
{
#if BV_HAS_BINARY_DATA
    const auto countryCodeToTranslationFileName = [] (const String& countryCode)
    {
        return String ("trans_") + countryCode + ".txt";
    };

    auto data = RawData (countryCodeToTranslationFileName (getCountryCodeToUse()));

    if (data.isValid())
        return data;

    return RawData (countryCodeToTranslationFileName (defaultCountryCode));
#else
    return {};
#endif
}


void initializeTranslations (const File& translationFile,
                             bool        ignoreCaseOfKeys)
{
    if (translationFile.existsAsFile())
        juce::LocalisedStrings::setCurrentMappings (
            new juce::LocalisedStrings (translationFile, ignoreCaseOfKeys));
}


void initializeTranslationsFromBinaryData (RawData data,
                                           bool    ignoreCaseOfKeys)
{
    if (! data.isValid())
        return;

    juce::LocalisedStrings::setCurrentMappings (
        new juce::LocalisedStrings ({data.data, static_cast< size_t > (data.size)},
                                    ignoreCaseOfKeys));
}


void initializeDefaultTranslations()
{
    initializeTranslationsFromBinaryData (getDefaultTranslationFile());
}

}  // namespace bav
