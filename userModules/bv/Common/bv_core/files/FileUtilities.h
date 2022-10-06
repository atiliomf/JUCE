
#pragma once

namespace bav
{
/*
 this function attempts to return the default location your plugin's preset files should be saved to and loaded from.
 if the directory cannot be found for your plugin, calling this function will attempt to create it.
 */
File getPresetsFolder (const String& companyName,
                       const String& pluginName);

File getFileOnDesktop (const String& fileName);


//==============================================================================


String addFileExtensionIfMissing (const String& string,
                                  const String& extension);

String removeFileExtensionIfThere (const String& string,
                                   const String& extension);


//==============================================================================


bool renameFile (const File& f, const String& newName);

void deleteFile (const File& f);


}  // namespace bav
