namespace bav::gui
{
PopupComponent::PopupComponent (std::function< void() > toClose)
    : closeFunc (toClose)
{
    setWantsKeyboardFocus (true);
}

void PopupComponent::close() { closeFunc(); }

void PopupComponent::resized()
{
    grabKeyboardFocus();
    resizeTriggered();
}

void PopupComponent::resizeTriggered() { }

bool PopupComponent::keyPressed (const juce::KeyPress& key)
{
    if (key.getKeyCode() == juce::KeyPress::escapeKey)
    {
        closeFunc();
        return true;
    }

    return keyPressRecieved (key);
}

bool PopupComponent::keyPressRecieved (const juce::KeyPress&) { return false; }

}  // namespace bav::gui
