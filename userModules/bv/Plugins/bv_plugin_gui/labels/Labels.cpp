
namespace bav::gui
{
StringPropertyLabel::StringPropertyLabel (plugin::StringProperty& propertyToUse)
    : Label (propertyToUse.getPropertyName()), plugin::StringProperty::Listener (propertyToUse), property (propertyToUse)
{
    this->onTextChange = [&]
    { property->set (getText()); };
}

void StringPropertyLabel::propertyValueChanged (const String& newValue)
{
    this->set (newValue);
}

}  // namespace bav::gui
