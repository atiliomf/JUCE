#pragma once

#if 0

BEGIN_JUCE_MODULE_DECLARATION

ID:                 bv_audio_effects
vendor:             Ben Vining
version:            0.0.1
name:               bv_audio_effects
description:        DSP effects for plugin development
dependencies:       bv_dsp

END_JUCE_MODULE_DECLARATION

#endif

#include "AudioEffects/AudioEffect.h"
#include "AudioEffects/AudioEffectManager.h"
#include "ReorderableFxChain/ReorderableFxChain.h"

#include "dynamics/SmoothedGain.h"
#include "dynamics/NoiseGate.h"
#include "dynamics/Compressor.h"
#include "dynamics/Limiter.h"

#include "misc/DeEsser.h"
#include "misc/Reverb.h"
#include "misc/DryWet.h"

#include "stereo_image/MonoStereoConverter.h"
#include "stereo_image/panning/MonoToStereoPanner.h"
#include "stereo_image/panning/StereoPanner.h"

#include "time/Delay.h"

#include "EQ/Filter.h"
#include "EQ/EQ.h"
