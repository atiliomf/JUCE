
#pragma once

namespace bav
{
/*=====================================================================================================
 Classes dealing with motion & movement sensors
 =====================================================================================================*/

struct MotionListener
{
    virtual ~MotionListener() = default;

    virtual void accelerationChanged (double, double, double) { }
    virtual void gravityChanged (double, double, double) { }
    virtual void rotationChanged (double, double, double) { }
    virtual void attitudeChanged (double, double, double) { }
};


class MotionManagerInterface
{
public:
    struct Coords
    {
        double x = 0;
        double y = 0;
        double z = 0;
    };

    MotionManagerInterface()          = default;
    virtual ~MotionManagerInterface() = default;

    virtual void start()     = 0;
    virtual void stop()      = 0;
    virtual bool isRunning() = 0;

    Coords getAcceleration() const { return acceleration; }
    Coords getGravity() const { return gravity; }
    Coords getRotation() const { return rotation; }
    Coords getAttitude() const { return attitude; }

    void addListener (MotionListener* l) { listeners.add (l); }
    void removeListener (MotionListener* l) { listeners.remove (l); }

protected:
    void accelerationChanged (double x, double y, double z);
    void gravityChanged (double x, double y, double z);
    void rotationChanged (double x, double y, double z);
    void attitudeChanged (double x, double y, double z);

    Coords acceleration;
    Coords gravity;
    Coords rotation;
    Coords attitude;

    juce::ListenerList< MotionListener > listeners;
};


}  // namespace bav
