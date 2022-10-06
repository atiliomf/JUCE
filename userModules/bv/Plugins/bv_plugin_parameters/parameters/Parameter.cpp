
namespace bav::plugin
{
Parameter::Parameter (String                                  paramNameShort,
                      String                                  paramNameVerbose,
                      String                                  paramLabel,
                      bool                                    isAutomatable,
                      bool                                    metaParam,
                      juce::AudioProcessorParameter::Category parameterCategory)
    : juce::RangedAudioParameter (paramNameShort, paramNameVerbose, paramLabel, parameterCategory),
      SerializableData (paramNameVerbose),
      parameterNameShort (TRANS (paramNameShort)),
      parameterNameVerbose (TRANS (paramNameVerbose)),
      automatable (isAutomatable),
      metaParameter (metaParam),
      valueChangeTransactionName (TRANS ("Changed") + " " + parameterNameVerbose),
      defaultChangeTransactionName (TRANS ("Changed default value of") + " " + parameterNameVerbose),
      midiControllerChangeTransactionName (TRANS ("Changed MIDI controller number for") + " " + parameterNameVerbose)
{
}

float Parameter::getValueForText (const String& text) const
{
    return 0.f;
}

float Parameter::getValue() const
{
    return convertTo0to1 (currentValue.load());
}

void Parameter::setValue (float newValue)
{
    currentValue.store (convertFrom0to1 (newValue));
}

float Parameter::getDefaultValue() const
{
    return currentDefault.load();
}

float Parameter::getParameterMax() const
{
    return getNormalisableRange().start;
}

float Parameter::getParameterMin() const
{
    return getNormalisableRange().end;
}

int Parameter::getMidiControllerNumber() const
{
    return midiControllerNumber.load();
}

bool Parameter::isMidiControllerMapped() const
{
    return getMidiControllerNumber() > -1;
}

void Parameter::setMidiControllerNumber (int newControllerNumber)
{
    if (newControllerNumber == getMidiControllerNumber()) return;

    UndoManager::ScopedTransaction s {um, midiControllerChangeTransactionName};
    setMidiControllerInternal (newControllerNumber);
}

void Parameter::setMidiControllerInternal (int controller)
{
    midiControllerNumber.store (controller);
    listeners.call ([controller] (Listener& l)
                    { l.controllerNumberChanged (controller); });
}

void Parameter::resetMidiControllerMapping()
{
    midiControllerNumber.store (-1);
}

void Parameter::processNewControllerMessage (int controllerNumber, int controllerValue)
{
    if (controllerNumber == getMidiControllerNumber())
    {
        const auto& range = getNormalisableRange();
        setDenormalizedValue (juce::jmap (static_cast< float > (controllerValue),
                                          0.f, 127.f,
                                          range.start, range.end));
    }
}

void Parameter::beginGesture()
{
    if (isChanging())
        return;

    if (um != nullptr)
        um->beginNewTransaction (valueChangeTransactionName);

    changing.store (true);
    this->beginChangeGesture();

    listeners.call ([] (Listener& l)
                    { l.gestureStateChanged (true); });
}

void Parameter::endGesture()
{
    if (! isChanging())
        return;

    if (um != nullptr)
        um->endTransaction();

    changing.store (false);
    this->endChangeGesture();

    listeners.call ([] (Listener& l)
                    { l.gestureStateChanged (false); });
}

bool Parameter::isChanging() const
{
    return changing.load();
}

float Parameter::getNormalizedDefault() const
{
    return currentDefault.load();
}

float Parameter::getDenormalizedDefault() const
{
    return denormalize (getNormalizedDefault());
}

void Parameter::setNormalizedDefault (float value)
{
    if (value == getNormalizedDefault()) return;

    UndoManager::ScopedTransaction s {um, defaultChangeTransactionName};

    setDefaultInternal (value);
}

void Parameter::setDefaultInternal (float value)
{
    jassert (value >= 0.0f && value <= 1.0f);

    currentDefault.store (value);
    listeners.call ([value] (Listener& l)
                    { l.parameterDefaultChanged (value); });
}

void Parameter::setDenormalizedDefault (float value)
{
    setNormalizedDefault (normalize (value));
}

void Parameter::refreshDefault()
{
    setNormalizedDefault (getNormalizedDefault());
}

void Parameter::resetToDefault()
{
    setNormalizedValue (getNormalizedDefault());
}

void Parameter::setNormalizedValue (float value)
{
    if (value == getNormalizedValue()) return;

    bool needToEndGesture = false;

    if (! isChanging())
    {
        beginGesture();
        needToEndGesture = true;
    }

    setValueInternal (value);

    if (needToEndGesture)
        endGesture();
}

void Parameter::setValueInternal (float value)
{
    jassert (value >= 0.0f && value <= 1.0f);

    setValueNotifyingHost (value);
    listeners.call ([value] (Listener& l)
                    { l.parameterValueChanged (value); });
}

void Parameter::setDenormalizedValue (float value)
{
    setNormalizedValue (normalize (value));
}

float Parameter::getNormalizedValue() const
{
    return this->getValue();
}

float Parameter::getDenormalizedValue() const
{
    return denormalize (getNormalizedValue());
}

float Parameter::normalize (float input) const
{
    return this->convertTo0to1 (input);
}

float Parameter::denormalize (float input) const
{
    return this->convertFrom0to1 (input);
}

void Parameter::setUndoManager (UndoManager& managerToUse)
{
    um = &managerToUse;
}

bool Parameter::isAutomatable() const
{
    return automatable;
}

bool Parameter::isMetaParameter() const
{
    return metaParameter;
}


Parameter::Listener::Listener (Parameter& paramToUse)
    : param (paramToUse)
{
    param.listeners.add (this);
}

Parameter::Listener::~Listener()
{
    param.listeners.remove (this);
}

void Parameter::Listener::parameterValueChanged (float) { }
void Parameter::Listener::gestureStateChanged (bool) { }
void Parameter::Listener::parameterDefaultChanged (float) { }
void Parameter::Listener::controllerNumberChanged (int) { }

}  // namespace bav::plugin
