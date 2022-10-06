#pragma once

#if 0

BEGIN_JUCE_MODULE_DECLARATION

ID:                 bv_synth
vendor:             Ben Vining
version:            0.0.1
name:               bv_synth
description:        Infrastructure for a polyphonic synthesizer instrument
dependencies:       bv_audio_effects bv_oscillators

END_JUCE_MODULE_DECLARATION

#endif

#include "SynthVoice/SynthVoice.h"

#include "Synth/Synth.h"

#include "SineSynth/sine_synth.h"

#include "SynthState/SynthState.h"
