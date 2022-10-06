
#pragma once

namespace bav::plugin
{
class Parameter : public juce::RangedAudioParameter, public SerializableData
{
public:
    Parameter (String                                  paramNameShort,
               String                                  paramNameVerbose,
               String                                  paramLabel        = {},
               bool                                    isAutomatable     = true,
               bool                                    metaParam         = false,
               juce::AudioProcessorParameter::Category parameterCategory = AudioProcessorParameter::genericParameter);

    float getParameterMax() const;
    float getParameterMin() const;

    int  getMidiControllerNumber() const;
    bool isMidiControllerMapped() const;
    void setMidiControllerNumber (int newControllerNumber);
    void resetMidiControllerMapping();
    void processNewControllerMessage (int controllerNumber, int controllerValue);

    void  refreshDefault();  // sets the parameter's current value to be the default value
    void  resetToDefault();  // resets the parameter's value to the default
    float getNormalizedDefault() const;
    float getDenormalizedDefault() const;
    void  setNormalizedDefault (float value);
    void  setDenormalizedDefault (float value);

    float getNormalizedValue() const;
    float getDenormalizedValue() const;
    void  setNormalizedValue (float value);
    void  setDenormalizedValue (float value);

    void beginGesture();
    void endGesture();
    bool isChanging() const;

    float normalize (float input) const;
    float denormalize (float input) const;

    void setUndoManager (UndoManager& managerToUse);

    bool isAutomatable() const final;
    bool isMetaParameter() const final;

    float getValueForText (const String& text) const final;

    //==============================================================================

    const String parameterNameShort;
    const String parameterNameVerbose;

    //==============================================================================

    struct Listener
    {
        Listener (Parameter& paramToUse);
        virtual ~Listener();

        virtual void parameterValueChanged (float newNormalizedValue);
        virtual void gestureStateChanged (bool gestureIsStarting);
        virtual void parameterDefaultChanged (float newNormalizedDefault);
        virtual void controllerNumberChanged (int newControllerNumber);

    private:
        Parameter& param;
    };

    //==============================================================================

protected:
    void setValueInternal (float value);
    void setDefaultInternal (float value);
    void setMidiControllerInternal (int controller);

private:
    float getValue() const final;
    void  setValue (float newValue) final;

    float getDefaultValue() const final;

    const bool automatable;
    const bool metaParameter;

    std::atomic< float > currentValue;
    std::atomic< float > currentDefault;
    std::atomic< bool >  changing {false};
    std::atomic< int >   midiControllerNumber {-1};

    UndoManager* um = nullptr;

    juce::ListenerList< Listener > listeners;

    const String valueChangeTransactionName;
    const String defaultChangeTransactionName;
    const String midiControllerChangeTransactionName;
};

}  // namespace bav::plugin
