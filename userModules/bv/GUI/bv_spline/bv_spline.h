#pragma once

#if 0

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 bv_spline
 vendor:             Ben Vining and Florian Mrugralla
 version:            0.0.1
 name:               bv_spline
 description:        Module for creating, editing, and interpolating B-splines as real-time audio paramters
 dependencies:       bv_serializing, juce_graphics

 END_JUCE_MODULE_DECLARATION

#endif

#include "spline/Spline.h"

#include "editor/SplineEditor.h"
