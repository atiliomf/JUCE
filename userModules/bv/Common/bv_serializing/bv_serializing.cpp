
#include "bv_serializing.h"


#include "SerializableData/TreeReflector/TreeReflector.cpp"
#include "SerializableData/TreeReflector/detail/helpers.cpp"
#include "SerializableData/SerializableData.cpp"

#include "Serializing/Specializations/VarSpecializations.cpp"
#include "Serializing/Specializations/AudioBuffers.cpp"
#include "Serializing/ValueTreeToJson/valuetree_json_converter.cpp"
#include "Serializing/Serializing.cpp"

#include "undo/UndoManager.cpp"

#include "DataSynchronizer/DataSynchronizer.cpp"
