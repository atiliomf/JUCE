
namespace bav::plugin
{
ParamUpdater::ParamUpdater (Parameter& param, std::function< void() > onValueChange, std::function< void (bool) > onGestureChange)
    : Listener (param), valueCallback (onValueChange), gestureCallback (onGestureChange)
{
}

void ParamUpdater::parameterValueChanged (float)
{
    valueCallback();
}

void ParamUpdater::gestureStateChanged (bool gestureIsStarting)
{
    gestureCallback (gestureIsStarting);
}

}  // namespace bav::plugin
