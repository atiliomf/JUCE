
#if JUCE_IOS

#include <TargetConditionals.h>

#ifdef TARGET_OS_IOS

#include "iosMotion.h"
#include <CoreMotion/CoreMotion.h>


namespace bav
{


@interface MotionManagerWrapper : NSObject
{
    MotionManager* owner;
}

@property (strong, nonatomic) CMMotionManager *motionManager;

@end

@implementation MotionManagerWrapper

- (void) startMotionManager
{
    self.motionManager = [[CMMotionManager alloc] init];
    self.motionManager.deviceMotionUpdateInterval   = 1.0 / 60.0;
    [self.motionManager startDeviceMotionUpdatesToQueue:[NSOperationQueue currentQueue]
                                            withHandler:^(CMDeviceMotion *motion, NSError *error)
     {
        [self outputUserAcceleration    :motion.userAcceleration];
        [self outputGravity             :motion.gravity];
        [self outputRotationRate        :motion.rotationRate];
        [self outputAttitude            :motion.attitude];
    }];
}


- (void) stopMotionManager
{
    [self.motionManager stopGyroUpdates];
    [self.motionManager stopAccelerometerUpdates];
    self.motionManager = nil;
}


- (void) outputUserAcceleration: (CMAcceleration)acceleration
{
    owner->accelerationChanged(acceleration.x, acceleration.y, acceleration.z);
}


- (void) outputGravity: (CMAcceleration)gravity
{
    owner->gravityChanged(gravity.x, gravity.y, gravity.z);
}


- (void) outputRotationRate: (CMRotationRate)rotation
{
    owner->rotationChanged(rotation.x, rotation.y, rotation.z);
}


- (void) outputAttitude: (CMAttitude *)attitude
{
    owner->attitudeChanged(attitude.pitch, attitude.roll, attitude.yaw);
}


- (id) initWithOwner: (MotionManager*) owner_
{
    if ((self = [super init]) != nil) { owner = owner_; };
    return self;
}


- (void) dealloc
{
    [self stopMotionManager];
    [super dealloc];
}

@end


/*===================================================================================*/


    MotionManager::MotionManager()
    {
        running = false;
        MotionManagerWrapper* newManager = [[MotionManagerWrapper alloc] initWithOwner: this];
        [newManager retain];
        motionManagerWrapper = newManager;
    }
    
    MotionManager::~MotionManager()
    {
        [((MotionManagerWrapper*) motionManagerWrapper) release];
    }
    
    void MotionManager::start()
    {
        if (! running)
        {
            [(MotionManagerWrapper*) motionManagerWrapper startMotionManager];
            isRunning = true;
        }
    }
    
    void MotionManager::stop()
    {
        [(MotionManagerWrapper*) motionManagerWrapper stopMotionManager];
        running = false;
    }
    

}  // namespace

#endif
#endif
