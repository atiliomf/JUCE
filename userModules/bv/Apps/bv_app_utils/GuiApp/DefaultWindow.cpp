
namespace bav
{
DefaultWindowBase::DefaultWindowBase (const String& appName)
    : DocumentWindow (appName, getBackgroundColour(), allButtons)
{
    setUsingNativeTitleBar (true);
}

juce::Colour DefaultWindowBase::getBackgroundColour()
{
    return juce::Desktop::getInstance().getDefaultLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId);
}

void DefaultWindowBase::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}

void DefaultWindowBase::init()
{
    if (Platform::isMobile())
    {
        setFullScreen (true);
    }
    else
    {
        setResizable (true, true);
        centreWithSize (getWidth(), getHeight());
    }

    setVisible (true);
}

}  // namespace bav
