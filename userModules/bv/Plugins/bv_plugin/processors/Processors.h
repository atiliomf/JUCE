#pragma once

namespace bav::plugin
{
template < class StateType, template < typename SampleType > class EngineType,
           BV_MUST_INHERIT_FROM (StateType, State),
           BV_MUST_INHERIT_FROM (EngineType< float >, dsp::Engine< float >) >
class Processor : public ProcessorBase
{
public:
    Processor (juce::AudioProcessor::BusesProperties busesLayout)
        : ProcessorBase (state, floatEngine, doubleEngine, busesLayout)
    {
        state.addTo (*this);
    }

protected:
    StateType state;

private:
    EngineType< float >  floatEngine {state};
    EngineType< double > doubleEngine {state};
};


template < class ProcessorType, class ComponentType,
           BV_MUST_INHERIT_FROM (ProcessorType, ProcessorBase) >
struct ProcessorWithEditor : ProcessorType
{
    ProcessorWithEditor (int width = 450, int height = 300)
        : w (width), h (height)
    {
    }

    bool hasEditor() const final { return true; }

    juce::AudioProcessorEditor* createEditor() final
    {
        return new PluginEditor< ComponentType > (*this, {w, h}, this->state);
    }

private:
    int w, h;
};

}  // namespace bav::plugin
