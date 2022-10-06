
namespace bav::plugin
{
void ParamHolderBase::addTo (juce::AudioProcessor& processor)
{
    processor.addParameter (getParam());
    addedToProcessor = true;
}

ParameterList::ParameterList (juce::Identifier name, UndoManager* um)
    : SerializableData (name), undo (um)
{
}

void ParameterList::setUndoManager (UndoManager& um)
{
    undo = &um;

    for (auto* holder : params)
        holder->getParam()->setUndoManager (um);

    for (auto& string : strings)
        string->setUndoManager (um);
}

void ParameterList::add (ParamHolderBase& param)
{
    addParameter (param, false);
}

void ParameterList::add (StringProperty& param)
{
    addStringProperty (param);
}

void ParameterList::addInternal (ParamHolderBase& param)
{
    addParameter (param, true);
}

void ParameterList::addInternal (StringProperty& param)
{
    addStringProperty (param);
}

void ParameterList::addParameter (ParamHolderBase& param, bool isInternal)
{
    if (undo != nullptr)
        param->setUndoManager (*undo);

    param.isInternal = isInternal;
    params.emplace_back (&param);
}

void ParameterList::addStringProperty (StringProperty& property)
{
    if (undo != nullptr)
        property->setUndoManager (*undo);

    strings.emplace_back (&property);
}

void ParameterList::setPitchbendParameter (IntParam& param)
{
    pitchwheelParameter = param.get();

    //    if (! params.contains (param))
    //        addParameter (param, false);
}

void ParameterList::setLastMovedMidiControllerNumberParameter (IntParam& param)
{
    lastMovedControllerNumberParameter = param.get();

    //    if (! params.contains (param))
    //        addParameter (param, true);
}

void ParameterList::setLastMovedMidiControllerValueParameter (IntParam& param)
{
    lastMovedControllerValueParameter = param.get();

    //    if (! params.contains (param))
    //        addParameter (param, true);
}

void ParameterList::addParametersTo (juce::AudioProcessor& processor)
{
    for (auto* holder : params)
    {
        if (holder->isInternal)
            holder->addTo (processor);
        else
            holder->addTo (dummyProcessor);
    }
}

void ParameterList::addAllParametersAsInternal()
{
    for (auto* holder : params)
        holder->addTo (dummyProcessor);
}

void ParameterList::serialize (TreeReflector& ref)
{
    ref.add ("Parameter", params);
    ref.add ("String", strings);
}

void ParameterList::refreshAllDefaults()
{
    for (auto* holder : params)
        holder->getParam()->refreshDefault();

    for (auto& string : strings)
        string->refreshDefault();
}

void ParameterList::resetAllToDefault()
{
    for (auto* holder : params)
        holder->getParam()->resetToDefault();

    for (auto& string : strings)
        string->resetToDefault();
}

void ParameterList::processMidi (const MidiBuffer& midiMessages)
{
    for (auto meta : midiMessages)
        processMidiMessage (meta.getMessage());
}

void ParameterList::processMidiMessage (const MidiMessage& message)
{
    if (message.isController())
        processNewControllerMessage (message.getControllerNumber(), message.getControllerValue());
    else if (message.isPitchWheel())
        processNewPitchwheelMessage (message.getPitchWheelValue());
}

void ParameterList::processNewControllerMessage (int controllerNumber, int controllerValue)
{
    for (auto* holder : params)
        holder->getParam()->processNewControllerMessage (controllerNumber, controllerValue);

    if (auto* num = lastMovedControllerNumberParameter)
    {
        num->set (controllerNumber);
    }

    if (auto* val = lastMovedControllerValueParameter)
    {
        val->set (controllerValue);
    }
}

void ParameterList::processNewPitchwheelMessage (int pitchwheelValue)
{
    if (auto* param = pitchwheelParameter)
    {
        param->set (juce::jmap (pitchwheelValue, 0, 16383, param->getMinimum(), param->getMaximum()));
    }
}


ParameterList::Listener::Listener (ParameterList& list,
                                   std::function< void (Parameter&) >
                                       onParamChange,
                                   std::function< void (Parameter&, bool) >
                                        onGestureGhange,
                                   bool includeInternalParams)
{
    for (auto* base : list.params)
    {
        if (includeInternalParams || ! base->isInternal)
        {
            auto* param = base->getParam();

            updaters.add (new ParamUpdater (
                *param,
                [=]
                { onParamChange (*param); },
                [=] (bool starting)
                { onGestureGhange (*param, starting); }));
        }
    }
}

}  // namespace bav::plugin
