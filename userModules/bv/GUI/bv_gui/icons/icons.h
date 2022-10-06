
#pragma once

namespace bav::gui
{
/*
 List of icons to be used with the createIcon method.
 */
enum IconType
{
    Stop,
    Play,
    Pause,
    Next,
    Previous,
    ShuffleForward,
    ShuffleBack,
    Eject,
    Add,
    Search,
    Power,
    Bypass,
    Info,
    Loop,
    Speaker,
    MutedSpeaker
};


/*
 Creates an icon in a given colour.
 */
extern juce::DrawablePath createIcon (IconType icon, juce::Colour colour = juce::Colours::black);


}  // namespace bav::gui
