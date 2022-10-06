
namespace bav::gui
{
TextButton::TextButton (const String& buttonText, std::function< void() > whenClicked)
    : juce::TextButton (buttonText, buttonText)
{
    onClick = std::move (whenClicked);

    this->setName (buttonText);
    this->setComponentID (buttonText);
}


ToggleTextButton::ToggleTextButton (const String& buttonText, std::function< void (bool) > on_click)
    : TextButton (buttonText, [&]
                  { buttonclicked(); }),
      whenClicked (on_click)
{
}

void ToggleTextButton::buttonclicked()
{
    whenClicked (getToggleState());
}


ImageButton::ImageButton (const juce::Image& image, std::function< void() > whenClicked, float opacityRange)
{
    onClick = std::move (whenClicked);

    this->setImages (false, true, true,
                     image, 1.f - opacityRange, {},
                     {}, 1.f, {},
                     {}, 1.f, juce::Colours::lightgrey);
}


}  // namespace bav::gui
