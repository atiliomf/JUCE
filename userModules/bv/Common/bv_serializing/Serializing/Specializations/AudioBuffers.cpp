
namespace bav
{
juce::MemoryBlock toMemory (const juce::AudioBuffer< float >& buffer)
{
    juce::MemoryBlock        block;
    juce::MemoryOutputStream stream {block, false};
    juce::FlacAudioFormat    format;

    if (auto* writer = format.createWriterFor (&stream, 48000, (unsigned) buffer.getNumChannels(), 24, {}, 0))
        writer->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());

    return block;
}

juce::MemoryBlock toMemory (const juce::AudioBuffer< double >& buffer)
{
    const auto numSamples  = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();

    juce::AudioBuffer< float > temp {numSamples, numChannels};

    for (int chan = 0; chan < numChannels; ++chan)
        vecops::convert (temp.getWritePointer (chan), buffer.getReadPointer (chan), numSamples);

    return toMemory (temp);
}

template < typename SampleType >
String bufferToString (const juce::AudioBuffer< SampleType >& buffer)
{
    return toMemory (buffer).toBase64Encoding();
}
template String bufferToString (const juce::AudioBuffer< float >&);
template String bufferToString (const juce::AudioBuffer< double >&);


template <>
juce::var toVar (juce::AudioBuffer< float >& buffer)
{
    return bufferToString (buffer);
}

template <>
juce::var toVar (juce::AudioBuffer< double >& buffer)
{
    return bufferToString (buffer);
}


void stringToBuffer (const String& string, juce::AudioBuffer< float >& buffer)
{
    juce::MemoryBlock mem;

    if (mem.fromBase64Encoding (string))
    {
        juce::MemoryInputStream in {mem.getData(), mem.getSize(), false};
        juce::FlacAudioFormat   format;

        if (auto* reader = format.createReaderFor (&in, false))
        {
            const auto numChannels = (int) reader->numChannels;
            const auto numSamples  = (int) reader->lengthInSamples;

            buffer.setSize (numChannels, numSamples);
            reader->read (&buffer, 0, numSamples, 0, true, numChannels > 1);
        }
    }
}

void stringToBuffer (const String& string, juce::AudioBuffer< double >& buffer)
{
    const auto numSamples  = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();

    juce::AudioBuffer< float > temp {numSamples, numChannels};

    for (int chan = 0; chan < numChannels; ++chan)
        vecops::convert (temp.getWritePointer (chan), buffer.getReadPointer (chan), numSamples);

    stringToBuffer (string, temp);

    for (int chan = 0; chan < numChannels; ++chan)
        vecops::convert (buffer.getWritePointer (chan), temp.getReadPointer (chan), numSamples);
}


template <>
juce::AudioBuffer< float > fromVar (juce::var var)
{
    juce::AudioBuffer< float > buffer;
    stringToBuffer (var.toString(), buffer);
    return buffer;
}

template <>
juce::AudioBuffer< double > fromVar (juce::var var)
{
    juce::AudioBuffer< double > buffer;
    stringToBuffer (var.toString(), buffer);
    return buffer;
}


}  // namespace bav
