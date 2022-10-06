
#if ! JUCE_ANDROID
#    error
#endif

#pragma once

#include <android/sensor.h>


namespace bav
{
class MotionManager : public MotionManagerInterface,
                      private juce::Timer
{
public:
    MotionManager();
    virtual ~MotionManager() override;

    virtual void start() final;
    virtual void stop() final;
    virtual bool isRunning() final { return running; }

private:
    void timerCallback() final;

    bool running;

    const int LOOPER_ID_USER           = 3;
    int       SENSOR_REFRESH_RATE_HZ   = 100;
    int       SENSOR_REFRESH_PERIOD_US = 1000000 / SENSOR_REFRESH_RATE_HZ;

    ASensorManager*    sensorManager;
    ASensorEventQueue* motionEventQueue;
    ALooper*           looper;

    const ASensor* accelerometer;
    const ASensor* rotation;
    const ASensor* gravity;
    const ASensor* attitude;

    void update();
};

}  // namespace bav
