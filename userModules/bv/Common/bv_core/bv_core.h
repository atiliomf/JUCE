#pragma once

#if 0

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                bv_core
 vendor:            Ben Vining
 version:           0.0.1
 name:              bv_core
 description:       General utilities useful for developing plugins.
 dependencies:      juce_audio_utils
 OSXFrameworks:     Accelerate
 iOSFrameworks:     Accelerate

 END_JUCE_MODULE_DECLARATION

#endif


//==============================================================================
/** Config: BV_HAS_BINARY_DATA
 
    Set this to 1 if your project has a juce binary data target.
 */
#ifndef BV_HAS_BINARY_DATA
#    define BV_HAS_BINARY_DATA 0
#endif


//==============================================================================
/** Config: BV_USE_VDSP
 
    Set this to 1 to use Apple's vDSP library for vecops SIMD.
    vDSP ships with the OS on Mac and iOS, which is why it's the default on Apple platforms.
    (Setting this to 1 disables IPP, Ne10 and MIPP.)
 */
#ifndef BV_USE_VDSP
#    if JUCE_IOS || JUCE_MAC
#        define BV_USE_VDSP 1
#    else
#        define BV_USE_VDSP 0
#    endif
#endif

#undef JUCE_USE_VDSP_FRAMEWORK
#define JUCE_USE_VDSP_FRAMEWORK BV_USE_VDSP

/*=======================================================================*/

#include <juce_audio_utils/juce_audio_utils.h>

#include "misc/Preprocessor.h"


namespace bav
{
using juce::File;
using juce::MidiBuffer;
using juce::MidiMessage;
using juce::String;
using juce::ValueTree;
}  // namespace bav


#include "misc/misc.h"
#include "misc/ValueSmoother.h"
#include "misc/TypeTraits.h"

#include "events/Broadcaster.h"
#include "events/Listener.h"
#include "events/Timers.h"

#include "files/FileUtilities.h"

#include "binary_data/BinaryDataHelpers.h"

#include "localization/localization.h"

#include "math/mathHelpers.h"
#include "math/vecops/vecops.h"
