
namespace bav
{
void MotionManagerInterface::accelerationChanged (double x, double y, double z)
{
    acceleration.x = x;
    acceleration.y = y;
    acceleration.z = z;

    listeners.call ([this] (MotionListener& l)
                    { l.accelerationChanged (acceleration.x, acceleration.y, acceleration.z); });
}

void MotionManagerInterface::gravityChanged (double x, double y, double z)
{
    gravity.x = x;
    gravity.y = y;
    gravity.z = z;

    listeners.call ([this] (MotionListener& l)
                    { l.gravityChanged (gravity.x, gravity.y, gravity.z); });
}

void MotionManagerInterface::rotationChanged (double x, double y, double z)
{
    rotation.x = x;
    rotation.y = y;
    rotation.z = z;

    listeners.call ([this] (MotionListener& l)
                    { l.rotationChanged (rotation.x, rotation.y, rotation.z); });
}

void MotionManagerInterface::attitudeChanged (double x, double y, double z)
{
    attitude.x = x;
    attitude.y = y;
    attitude.z = z;

    listeners.call ([this] (MotionListener& l)
                    { l.attitudeChanged (attitude.x, attitude.y, attitude.z); });
}

}  // namespace bav
