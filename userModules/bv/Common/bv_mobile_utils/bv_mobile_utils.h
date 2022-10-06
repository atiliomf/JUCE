#pragma once

#if 0

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 bv_mobile_utils
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_mobile_utils
 description:        JUCE module that provides some useful utilities for crossplatform iOS and Android development
 dependencies:       juce_gui_basics
 iOSFrameworks:      CoreMotion

 END_JUCE_MODULE_DECLARATION

#endif


#include <juce_gui_basics/juce_gui_basics.h>

#include "all_platforms/motion.h"
#include "all_platforms/rotation.h"

#if JUCE_IOS
#    include "iOS/iosMotion.h"
#elif JUCE_ANDROID
#    include "Android/androidMotion.h"
#else
#    include "dummy_desktop_versions/DummyMotionManager.h"
#endif


namespace bav
{
// Control the auto-locking of the screen on mobile devices. On desktop, this disables the computer's screensaver.
struct AutoLock
{
    static void setEnabled (bool shouldBeEnabled);
};


struct Platform
{
    static constexpr bool isMobile()
    {
#if JUCE_IOS || JUCE_ANDROID
        return true;
#endif
        return false;
    }
};


}  // namespace bav
