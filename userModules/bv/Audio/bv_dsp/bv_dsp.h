#pragma once

#if 0

BEGIN_JUCE_MODULE_DECLARATION

ID:                 bv_dsp
vendor:             Ben Vining
version:            0.0.1
name:               bv_dsp
description:        DSP/audio utilities and effects
dependencies:       juce_audio_utils juce_dsp bv_midi

END_JUCE_MODULE_DECLARATION

#endif

#include "FIFOs/CircularBuffer.h"
#include "FIFOs/AudioFIFO.h"
#include "FIFOs/AudioAndMidiFIFO.h"

#include "engines/AudioEngine.h"
#include "engines/LatencyEngine.h"

#include "filters/Filter.h"

#include "BufferUtils/BufferUtils.h"

#include "PitchDetector/PitchDetector.h"

#include "BasicProcessor/BasicProcessor.h"
