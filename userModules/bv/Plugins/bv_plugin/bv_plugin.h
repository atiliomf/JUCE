#pragma once

#if 0

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 bv_plugin
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_plugin
 description:        General plugin utilities & tools
 dependencies:       bv_plugin_parameters bv_mobile_utils

 END_JUCE_MODULE_DECLARATION

#endif


#include <bv_plugin_parameters/bv_plugin_parameters.h>
#include <bv_mobile_utils/bv_mobile_utils.h>


#include "state/state.h"
#include "state/PluginUndo.h"

#include "processors/ProcessorBase.h"

#include "EditorBase/EditorBase.h"

#include "processors/Processors.h"

#include "presets/PresetManager.h"
