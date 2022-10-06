namespace bav::serializing
{
ValueTree toTree (SerializableData& data)
{
    return data.serialize();
}

void fromTree (const ValueTree& tree, SerializableData& data)
{
    data.deserialize (tree);
}

void toBinary (SerializableData& data, File file)
{
    juce::FileOutputStream stream (file);
    data.serialize().writeToStream (stream);
}

void toBinary (SerializableData& data, juce::MemoryBlock& dest)
{
    juce::MemoryOutputStream stream (dest, false);
    data.serialize().writeToStream (stream);
}

void fromBinary (File file, SerializableData& dest)
{
    juce::MemoryBlock     data;
    juce::FileInputStream stream (file);
    stream.readIntoMemoryBlock (data);
    fromBinary (data.getData(), data.getSize(), dest);
}

void fromBinary (const juce::MemoryBlock& data, SerializableData& dest)
{
    fromBinary (data.getData(), data.getSize(), dest);
}

void fromBinary (const void* data, size_t dataSizeInBytes, SerializableData& dest)
{
    dest.deserialize (ValueTree::readFromData (data, dataSizeInBytes));
}

void fromXML (const juce::XmlElement& xml, SerializableData& dest)
{
    dest.deserialize (ValueTree::fromXml (xml));
}

void fromXML (std::unique_ptr< juce::XmlElement > xml, SerializableData& dest)
{
    if (auto* x = xml.get())
        fromXML (*x, dest);
}

void fromXML (const File& xmlFile, SerializableData& dest)
{
    fromXML (juce::XmlDocument::parse (xmlFile), dest);
}

std::unique_ptr< juce::XmlElement > toXML (SerializableData& source)
{
    return source.serialize().createXml();
}

void toXML (SerializableData& source, const File& file)
{
    if (! file.existsAsFile())
        file.create();

    toXML (source)->writeTo (file);
}

void fromJSON (const String& jsonText, SerializableData& dest)
{
    dest.deserialize (valueTreeFromJSON (jsonText));
}

void fromJSON (const File& file, SerializableData& dest)
{
    fromJSON (file.loadFileAsString(), dest);
}

String toJSON (SerializableData& source)
{
    return valueTreeToJSON (source.serialize());
}

void toJSON (SerializableData& source, const File& file)
{
    if (! file.exists())
        file.create();

    file.replaceWithText (toJSON (source));
}

}  // namespace bav::serializing
