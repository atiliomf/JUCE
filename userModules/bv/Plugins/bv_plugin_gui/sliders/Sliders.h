
#pragma once

#include "SliderBase.h"

namespace bav::gui
{
class FloatSlider : public SliderBase,
                    private plugin::FloatParameter::Listener
{
public:
    FloatSlider (plugin::FloatParameter& paramToUse, SliderStyle style = SliderStyle::LinearVertical);

    plugin::FloatParameter& param;

private:
    void paramValueChanged (float newValue) final;
    void paramDefaultChanged (float newDefault) final;

    void valueChanged() final;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class IntSlider : public SliderBase,
                  private plugin::IntParameter::Listener
{
public:
    IntSlider (plugin::IntParameter& paramToUse, SliderStyle style = SliderStyle::LinearVertical);

    plugin::IntParameter& param;

private:
    void paramValueChanged (int newValue) final;
    void paramDefaultChanged (int newDefault) final;

    void valueChanged() final;
};

}  // namespace bav::gui
