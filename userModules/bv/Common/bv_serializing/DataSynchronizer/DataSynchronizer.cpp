
namespace bav::serializing
{
DataSynchronizer::DataSynchronizer (SerializableData& dataToUse)
    : managedState (dataToUse)
{
}

void DataSynchronizer::applyRecievedData (const juce::MemoryBlock& recievedData)
{
    serializing::fromBinary (recievedData, managedState);
}

void DataSynchronizer::callback()
{
    serializing::toBinary (managedState, cachedData);

    sendData (cachedData);
}

}  // namespace bav::serializing
