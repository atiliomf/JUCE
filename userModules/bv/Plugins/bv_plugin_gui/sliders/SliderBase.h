#pragma once

#include <bv_plugin_parameters/bv_plugin_parameters.h>
#include <bv_gui/bv_gui.h>

namespace bav::gui
{
class SliderBase : public juce::Slider
{
public:
    SliderBase (plugin::Parameter& paramToUse);

private:
    plugin::Parameter& param;

    void startedDragging() final;
    void stoppedDragging() final;
};

}  // namespace bav::gui
