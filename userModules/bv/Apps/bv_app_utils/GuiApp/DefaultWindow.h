#pragma once

#include <bv_serializing/bv_serializing.h>
#include <bv_mobile_utils/bv_mobile_utils.h>

namespace bav
{
struct DefaultWindowBase : public juce::DocumentWindow
{
public:
    DefaultWindowBase (const String& appName);

protected:
    void init();

private:
    void         closeButtonPressed() final;
    juce::Colour getBackgroundColour();
};


template < typename ComponentType >
struct DefaultWindow : public DefaultWindowBase
{
    DefaultWindow (const String& appName)
        : DefaultWindowBase (appName)
    {
        setContentOwned (new ComponentType(), true);
        init();
    }
};

}  // namespace bav
