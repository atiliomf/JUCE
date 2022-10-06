
#pragma once

namespace bav::gui
{
class PopupComponent : public juce::Component
{
public:
    PopupComponent (std::function< void() > toClose);

    void close();

private:
    void resized() final;
    bool keyPressed (const juce::KeyPress& key) final;

    virtual void resizeTriggered();
    virtual bool keyPressRecieved (const juce::KeyPress& key);

    std::function< void() > closeFunc;
};


template < typename ContentType, std::enable_if_t< std::is_base_of< PopupComponent, ContentType >::value >* = nullptr >
class Popup : public juce::Component
{
public:
    Popup()
    {
        setInterceptsMouseClicks (false, true);
    }

    void resized() final
    {
        if (window.get() != nullptr)
            window->setBounds (getLocalBounds());
    }

    bool isVisible() const
    {
        return window.get() != nullptr;
    }

    template < typename... Args >
    void create (Args&&... args)
    {
        window.reset (new ContentType (std::forward< Args > (args)...));
        getTopLevelComponent()->addAndMakeVisible (window.get());
        resized();
    }

    void destroy()
    {
        if (window.get() != nullptr)
        {
            window.reset();
        }
    }

private:
    std::unique_ptr< ContentType > window;
};

}  // namespace bav::gui
