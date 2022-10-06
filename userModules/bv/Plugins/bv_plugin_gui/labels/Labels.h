#pragma once

namespace bav::gui
{
struct StringPropertyLabel : Label, private plugin::StringProperty::Listener
{
    StringPropertyLabel (plugin::StringProperty& propertyToUse);

private:
    void propertyValueChanged (const String& newValue) final;

    plugin::StringProperty& property;
};

}  // namespace bav::gui
