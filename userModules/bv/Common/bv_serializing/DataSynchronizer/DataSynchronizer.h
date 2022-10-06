
#pragma once

namespace bav::serializing
{
class DataSynchronizer
{
public:
    explicit DataSynchronizer (SerializableData& dataToUse);

    void applyRecievedData (const juce::MemoryBlock& recievedData);

private:
    virtual void sendData (const juce::MemoryBlock& outgoingData) = 0;

    void callback();

    SerializableData& managedState;

    juce::MemoryBlock cachedData;

    events::TimerCallback t {[&]
                             { callback(); },
                             10};
};

}  // namespace bav::serializing
