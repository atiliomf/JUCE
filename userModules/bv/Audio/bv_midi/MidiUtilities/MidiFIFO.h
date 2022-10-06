
#pragma once

#include <bv_core/bv_core.h>

namespace bav::midi
{
class MidiFIFO
{
public:
    MidiFIFO (int maxNumMessages = 512);

    ~MidiFIFO() = default;

    void setSize (int maxNumMessages);

    void clear();

    int numStoredEvents() const;

    void pushEvents (const juce::MidiBuffer& source, const int numSamples);

    void popEvents (juce::MidiBuffer& output, const int numSamples);

private:
    int numStoredSamples {0};

    MidiBuffer base;

    MidiBuffer copying;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiFIFO)
};


}  // namespace bav::midi
