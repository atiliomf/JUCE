
#pragma once

namespace bav::plugin
{
struct ParamHolderBase : SerializableData
{
    ParamHolderBase (bool internal = false) : isInternal (internal) { }

    Parameter* operator->() { return getParam(); }

    virtual ~ParamHolderBase() = default;

    virtual Parameter* getParam() const = 0;

    void addTo (juce::AudioProcessor& processor);

    bool isInternal;

protected:
    void serialize (TreeReflector& ref)
    {
        ref.add ("Parameter", *getParam());
        ref.add ("Internal", isInternal);
    }

    bool addedToProcessor = false;
};


//==============================================================================


template < typename ParameterType >
class ParameterHolder : public ParamHolderBase
{
public:
    template < typename... Args >
    explicit ParameterHolder (Args&&... args)
    {
        param = new ParameterType (std::forward< Args > (args)...);
    }

    ~ParameterHolder() override
    {
        if (! ParamHolderBase::addedToProcessor)
            delete param;
    }

    Parameter*     getParam() const final { return param; }
    ParameterType* get() const { return param; }

                   operator ParameterType&() { return *param; }
    ParameterType* operator->() { return param; }
    ParameterType& operator*() { return *param; }

private:
    ParameterType* param;
};

}  // namespace bav::plugin

namespace bav
{
using FloatParam = plugin::ParameterHolder< plugin::FloatParameter >;
using IntParam   = plugin::ParameterHolder< plugin::IntParameter >;
using BoolParam  = plugin::ParameterHolder< plugin::BoolParameter >;

using dbParam      = plugin::ParameterHolder< plugin::GainParameter >;
using ToggleParam  = plugin::ParameterHolder< plugin::ToggleParameter >;
using PercentParam = plugin::ParameterHolder< plugin::PercentParameter >;
using HzParam      = plugin::ParameterHolder< plugin::FrequencyParameter >;

using GainMeter = plugin::ParameterHolder< plugin::GainMeterParameter >;

}  // namespace bav
