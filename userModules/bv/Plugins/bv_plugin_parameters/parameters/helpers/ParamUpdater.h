#pragma once

namespace bav::plugin
{
struct ParamUpdater : Parameter::Listener
{
    ParamUpdater (Parameter& param,
                  std::function< void() >
                                               onValueChange,
                  std::function< void (bool) > onGestureChange = {});

private:
    void parameterValueChanged (float) final;
    void gestureStateChanged (bool gestureIsStarting) final;

    std::function< void() >      valueCallback;
    std::function< void (bool) > gestureCallback;
};

}  // namespace bav::plugin
