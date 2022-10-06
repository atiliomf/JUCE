#pragma once

#if 0

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                 bv_custom_standalone
 vendor:             Ben Vining
 version:            0.0.1
 name:               bv_custom_standalone
 description:        Custom standalone wrapper for juce plugins
 dependencies:       bv_plugin bv_app_utils

 END_JUCE_MODULE_DECLARATION

#endif


#ifndef JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP
#    define JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP 0
#endif

#ifndef JucePlugin_Build_Standalone
#    define JucePlugin_Build_Standalone 0
#endif

#if JucePlugin_Build_Standalone && JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP

//#    include <bv_plugin/bv_plugin.h>
//#    include <bv_app_utils/bv_app_utils.h>

//#    include "Window/StandaloneFilterWindow.h"
//#    include "App/StandaloneFilterApp.h"

#endif
