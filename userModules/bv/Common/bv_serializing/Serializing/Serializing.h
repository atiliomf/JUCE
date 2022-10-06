#pragma once

#include "ValueTreeToJson/valuetree_json_converter.h"

#include "Specializations/AudioBuffers.h"
#include "Specializations/VarSpecializations.h"

namespace bav::serializing
{
ValueTree toTree (SerializableData& data);
void      fromTree (const ValueTree& tree, SerializableData& data);

void toBinary (SerializableData& data, File file);
void toBinary (SerializableData& data, juce::MemoryBlock& dest);

void fromBinary (File file, SerializableData& dest);
void fromBinary (const juce::MemoryBlock& data, SerializableData& dest);
void fromBinary (const void* data, size_t dataSizeInBytes, SerializableData& dest);

template < typename IntegerType >
void fromBinary (const void* data, IntegerType dataSizeInBytes, SerializableData& dest)
{
    fromBinary (data, static_cast< size_t > (dataSizeInBytes), dest);
}

std::unique_ptr< juce::XmlElement > toXML (SerializableData& source);
void                                toXML (SerializableData& source, const File& file);

void fromXML (const juce::XmlElement& xml, SerializableData& dest);
void fromXML (std::unique_ptr< juce::XmlElement > xml, SerializableData& dest);
void fromXML (const File& xmlFile, SerializableData& dest);

String toJSON (SerializableData& source);
void   toJSON (SerializableData& source, const File& file);

void fromJSON (const String& jsonText, SerializableData& dest);
void fromJSON (const File& file, SerializableData& dest);

}  // namespace bav::serializing
