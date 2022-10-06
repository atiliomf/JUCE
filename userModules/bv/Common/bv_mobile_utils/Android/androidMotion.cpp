
namespace bav
{
MotionManager::MotionManager()
{
    sensorManager = ASensorManager_getInstance();
    accelerometer = ASensorManager_getDefaultSensor (
        sensorManager, ASENSOR_TYPE_LINEAR_ACCELERATION);
    rotation =
        ASensorManager_getDefaultSensor (sensorManager, ASENSOR_TYPE_GYROSCOPE);
    gravity =
        ASensorManager_getDefaultSensor (sensorManager, ASENSOR_TYPE_GRAVITY);
    attitude = ASensorManager_getDefaultSensor (
        sensorManager, ASENSOR_TYPE_GAME_ROTATION_VECTOR);
    looper           = ALooper_prepare (ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    motionEventQueue = ASensorManager_createEventQueue (
        sensorManager, looper, LOOPER_ID_USER, NULL, NULL);
}

MotionManager::~MotionManager()
{
    ASensorManager_destroyEventQueue (sensorManager, motionEventQueue);
}

void MotionManager::timerCallback()
{
    if (running) update();
}

void MotionManager::start()
{
    if (! running)
    {
        running = true;
        ASensorEventQueue_enableSensor (motionEventQueue, accelerometer);
        ASensorEventQueue_setEventRate (
            motionEventQueue, accelerometer, SENSOR_REFRESH_PERIOD_US);
        ASensorEventQueue_enableSensor (motionEventQueue, rotation);
        ASensorEventQueue_setEventRate (
            motionEventQueue, rotation, SENSOR_REFRESH_PERIOD_US);
        ASensorEventQueue_enableSensor (motionEventQueue, gravity);
        ASensorEventQueue_setEventRate (
            motionEventQueue, gravity, SENSOR_REFRESH_PERIOD_US);
        ASensorEventQueue_enableSensor (motionEventQueue, attitude);
        ASensorEventQueue_setEventRate (
            motionEventQueue, attitude, SENSOR_REFRESH_PERIOD_US);

        Timer::startTimerHz (SENSOR_REFRESH_RATE_HZ);
    }
}

void MotionManager::stop()
{
    running = false;
    ASensorEventQueue_disableSensor (motionEventQueue, accelerometer);
    ASensorEventQueue_disableSensor (motionEventQueue, rotation);
    ASensorEventQueue_disableSensor (motionEventQueue, gravity);
    ASensorEventQueue_disableSensor (motionEventQueue, attitude);
    Timer::stopTimer();
}

void MotionManager::update()
{
    ALooper_pollAll (0, NULL, NULL, NULL);
    ASensorEvent event;

    while (ASensorEventQueue_getEvents (motionEventQueue, &event, 1) > 0)
    {
        if (event.type == ASENSOR_TYPE_LINEAR_ACCELERATION)
        {
            accelerationChanged (event.acceleration.x * -0.08,
                                 event.acceleration.y * 0.08,
                                 event.acceleration.z * 0.08);
        }
        else if (event.type == ASENSOR_TYPE_GYROSCOPE)
        {
            rotationChanged (event.vector.x * 1.0,
                             event.vector.y * 1.0,
                             event.vector.z * 1.0);
        }
        else if (event.type == ASENSOR_TYPE_GRAVITY)
        {
            gravityChanged (event.acceleration.x * -0.1,
                            event.acceleration.y * 0.1,
                            event.acceleration.z * 0.1);
        }
        else if (event.type == ASENSOR_TYPE_GAME_ROTATION_VECTOR)
        {
            rotationChanged (event.vector.y * 2.0,
                             event.vector.x * 2.0,
                             event.vector.z * 2.0);
        }
    }
}

}  // namespace bav
