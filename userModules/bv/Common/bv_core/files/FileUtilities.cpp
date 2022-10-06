
namespace bav
{
File getPresetsFolder (const String& companyName,
                       const String& pluginName)
{
    File rootFolder;

#if JUCE_WINDOWS
    rootFolder = File::getSpecialLocation (
        File::SpecialLocationType::userDocumentsDirectory);
#else
    rootFolder = File::getSpecialLocation (
        File::SpecialLocationType::userApplicationDataDirectory);

#    if JUCE_MAC
    rootFolder = rootFolder.getChildFile ("Audio").getChildFile ("Presets");
#    endif
#endif

    rootFolder = rootFolder.getChildFile (companyName).getChildFile (pluginName);

    if (! rootFolder.isDirectory())
        rootFolder.createDirectory();

    return rootFolder;
}


File getFileOnDesktop (const String& fileName)
{
    return File::getSpecialLocation (File::userDesktopDirectory).getChildFile (fileName);
}


//==============================================================================


String addFileExtensionIfMissing (const String& string,
                                  const String& extension)
{
    return File::createLegalFileName (
        string.endsWith (extension) ? string.trim() : (string + extension).trim());
}


String removeFileExtensionIfThere (const String& string,
                                   const String& extension)
{
    return (string.endsWith (extension))
             ? string.dropLastCharacters (extension.length()).trim()
             : string.trim();
}


//==============================================================================

bool renameFile (const File& f, const String& newName)
{
    jassert (! newName.isEmpty());

    if (! f.existsAsFile())
        return false;

    return f.moveFileTo (f.getSiblingFile (newName));
}

void deleteFile (const File& f)
{
    if (f.existsAsFile())
        if (! f.moveToTrash())
            f.deleteFile();
}

}  // namespace bav
