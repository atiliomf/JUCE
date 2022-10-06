#pragma once

#if 0

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 bv_midi
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_midi
 description:        Midi utilities
 dependencies:       bv_core

 END_JUCE_MODULE_DECLARATION

#endif


//==============================================================================
/** Config: BV_USE_MTS_ESP
 
 Set this to 1 if your project is using the MTS-ESP library.
 If this is 0, this module will use the fallback implementation for the PitchConverter class.
 */
#ifndef BV_USE_MTS_ESP
#    define BV_USE_MTS_ESP 0
#endif


#include "MidiUtilities/MidiFIFO.h"
#include "MidiUtilities/MidiUtilities.h"
#include "MidiUtilities/PitchbendTracker.h"

#include "processors/MidiProcessor.h"
#include "processors/MidiChoppingProcessor/MidiChoppingProcessor.h"

#include "PitchConverter/PitchConverter.h"

#include "MidiUtilities/PitchPipeline.h"
