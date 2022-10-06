#pragma once

namespace bav::gui
{
struct TextButton : juce::TextButton
{
    TextButton (const String& buttonText, std::function< void() > whenClicked);
};


struct ToggleTextButton : TextButton
{
    ToggleTextButton (const String& buttonText, std::function< void (bool) > on_click);

private:
    void buttonclicked();

    std::function< void (bool) > whenClicked;
};


struct ImageButton : juce::ImageButton
{
    ImageButton (const juce::Image& image, std::function< void() > whenClicked, float opacityRange = 0.2f);
};

}  // namespace bav::gui
