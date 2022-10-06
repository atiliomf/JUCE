
#pragma once

namespace bav::plugin
{
struct Dimensions : SerializableData
{
    using SerializableData::SerializableData;

    void serialize (TreeReflector& ref) final;

    int width, height;
};

class State : public SerializableData
{
public:
    State (ParameterList& listToUse, String pluginName = "");

    ParameterList& getParameters();

    void setDimensions (int width, int height);

    virtual void addTo (juce::AudioProcessor& processor);


    ToggleParam mainBypass {"Main bypass", "Main bypass", false};

    Dimensions dimensions;

protected:
    void serialize (TreeReflector& ref) override;

private:
    ParameterList& list;
};

}  // namespace bav::plugin
