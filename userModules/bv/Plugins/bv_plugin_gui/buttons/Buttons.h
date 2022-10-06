#pragma once

namespace bav::gui
{
struct StringPropertyTextButton : TextButton, private plugin::StringProperty::Listener
{
    StringPropertyTextButton (plugin::StringProperty& property, std::function< void() > whenClicked);

private:
    void propertyValueChanged (const String& newValue) final;
};


class ToggleButton : public ToggleTextButton
{
public:
    ToggleButton (plugin::BoolParameter& paramToUse, std::function< void (bool) > cb = {});

    plugin::BoolParameter& param;

private:
    void clicked_callback (bool state);

    std::function< void (bool) > callback;
};

}  // namespace bav::gui
