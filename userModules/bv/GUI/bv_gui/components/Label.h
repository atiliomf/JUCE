#pragma once

namespace bav::gui
{
struct Label : juce::Label
{
    Label (const String& textToUse = {});

    void set (const String& newText);
};

}  // namespace bav::gui
