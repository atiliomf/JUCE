
#pragma once

namespace bav::plugin
{
template < typename SampleType >
class ParameterProcessorBase : public dsp::MidiChoppingProcessor< SampleType >
{
public:
    ParameterProcessorBase (ParameterList& listToUse);
    virtual ~ParameterProcessorBase() = default;

private:
    void handleMidiMessage (const MidiMessage& m) final;

    ParameterList& list;
};

}  // namespace bav::plugin
