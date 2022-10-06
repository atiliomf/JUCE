#pragma once

#if 0

BEGIN_JUCE_MODULE_DECLARATION

ID:                 bv_psola
vendor:             Ben Vining
version:            0.0.1
name:               bv_psola
description:        Classes for PSOLA pitch shifting of monophonic audio
dependencies:       bv_dsp

END_JUCE_MODULE_DECLARATION

#endif

#include "analysis/psola_analyzer.h"
#include "resynthesis/psola_shifter.h"

#include "PitchCorrector/PitchCorrector.h"
