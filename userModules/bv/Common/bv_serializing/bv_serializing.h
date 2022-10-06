#pragma once

#if 0

 BEGIN_JUCE_MODULE_DECLARATION

 ID:                bv_serializing
 vendor:            Ben Vining
 version:           0.0.1
 name:              bv_serializing
 description:       Easily serialize any C++ object to and from a ValueTree or JSON
 dependencies:      bv_core

 END_JUCE_MODULE_DECLARATION

#endif

#include <bv_core/bv_core.h>

namespace bav
{
template < typename Type >
juce::var toVar (Type& object)
{
    return {object};
}

template < typename Type >
Type fromVar (juce::var var)
{
    return {var};
}


struct SerializableData;


template < typename Type >
constexpr bool isSerializable()
{
    return std::is_base_of< SerializableData, Type >() && ! std::is_const< Type >();
}

}  // namespace bav


#include "SerializableData/SerializableData.h"

#include "Serializing/Serializing.h"

#include "undo/UndoManager.h"

#include "DataSynchronizer/DataSynchronizer.h"
