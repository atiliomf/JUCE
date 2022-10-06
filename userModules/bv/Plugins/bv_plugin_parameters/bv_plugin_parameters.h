#pragma once

#if 0

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 bv_plugin_parameters
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_plugin_parameters
 description:        Plugin parameter classes, done right
 dependencies:       bv_dsp bv_serializing

 END_JUCE_MODULE_DECLARATION

#endif

#include <bv_dsp/bv_dsp.h>
#include <bv_serializing/bv_serializing.h>

#include "parameters/helpers/ParameterValueConversionLambdas.h"
#include "parameters/Parameter.h"
#include "parameters/ParameterTypes.h"
#include "parameters/specializations/ParameterSpecializations.h"
#include "parameters/helpers/ParamUpdater.h"

#include "ParameterList/ParameterHolder.h"

#include "StringProperty/StringProperty.h"

#include "RealtimeStateObject/RealtimeStateObject.h"

#include "ParameterList/ParameterList.h"

#include "processor/ParameterProcessor.h"
