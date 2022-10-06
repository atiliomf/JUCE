#pragma once

namespace bav::plugin
{
class PresetManagerBase
{
public:
    PresetManagerBase (State& stateToUse, UndoManager* um = nullptr);
    virtual ~PresetManagerBase() = default;

    virtual std::string getCompanyName()         = 0;
    virtual std::string getProductName()         = 0;
    virtual std::string getPresetFileExtension() = 0;

    File presetsFolder();

    bool loadPreset (const String& presetName);
    void savePreset (const String& presetName);
    void deletePreset (const String& presetName);
    bool renamePreset (const String& previousName, const String& newName);

    void rescanPresetsFolder();

    const juce::StringArray& presetNames();

    events::Broadcaster& getPresetsChangedBroadcaster();
    events::Broadcaster& getPresetLoadedBroadcaster();

private:
    File presetNameToFilePath (const String& presetName);

    UndoManager* undo;
    State&       state;

    juce::StringArray namesOfAvailablePresets;

    events::Broadcaster availablePresetsChanged;
    events::Broadcaster presetLoaded;
};

}  // namespace bav::plugin
