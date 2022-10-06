namespace bav
{
UndoManager::UndoManager (SerializableData& stateToManage)
    : state (stateToManage)
{
    storedStates.reserve (50);
    //  saveState();
}

bool UndoManager::isInMiddleOfTransaction() const
{
    return changing;
}

String UndoManager::getCurrentTransactionName() const
{
    return transactionName;
}

events::Broadcaster& UndoManager::getBroadcaster()
{
    return broadcaster;
}

bool UndoManager::undo()
{
    if (hasUndo())
    {
        loadState (storedStates[--currentStep]);
        return true;
    }

    return false;
}

bool UndoManager::redo()
{
    if (hasRedo())
    {
        loadState (storedStates[++currentStep]);
        return true;
    }

    return false;
}

void UndoManager::undoToLastTransaction()
{
    loadState (storedStates[currentStep]);
}

bool UndoManager::hasUndo() const
{
    return currentStep > 0 && ! storedStates.empty();
}

bool UndoManager::hasRedo() const
{
    return currentStep + 1 < storedStates.size();
}

void UndoManager::beginNewTransaction (const String& name, bool force)
{
    if (force) endTransaction();

    if (! changing && ! name.isEmpty())
    {
        transactionName = name;
    }

    changing = true;
}

void UndoManager::endTransaction()
{
    if (changing)
    {
        saveState();
        changing = false;
    }
}

void UndoManager::saveState()
{
    storedStates.emplace_back (state.serialize(), transactionName);
}

void UndoManager::clearUndoHistory()
{
    storedStates.clear();
    transactionName.clear();
    currentStep = 0;
    saveState();
}

String UndoManager::getNextUndoTransactionName() const
{
    if (hasUndo())
        return storedStates[currentStep - 1].transactionName;

    return {};
}

String UndoManager::getNextRedoTransactionName() const
{
    if (hasRedo())
        return storedStates[currentStep + 1].transactionName;

    return {};
}

juce::StringArray UndoManager::getUndoTransactionNames() const
{
    juce::StringArray names;

    for (auto i = (int) (currentStep - 1); i >= 0; --i)
        names.add (storedStates[(std::vector< SavedState >::size_type) i].transactionName);

    return names;
}

juce::StringArray UndoManager::getRedoTransactionNames() const
{
    juce::StringArray names;

    for (auto i = currentStep + 1; i < storedStates.size(); ++i)
        names.add (storedStates[i].transactionName);

    return names;
}

void UndoManager::loadState (const SavedState& stateToLoad)
{
    state.deserialize (stateToLoad.state);
    broadcaster.trigger();
}

UndoManager::SavedState::SavedState (const ValueTree& tree, const String& name)
    : state (tree), transactionName (name)
{
}

UndoManager::ScopedTransaction::ScopedTransaction (UndoManager* um, const String& name)
    : undo (um)
{
    if (undo != nullptr)
        undo->beginNewTransaction (name);
}

UndoManager::ScopedTransaction::~ScopedTransaction()
{
    if (undo != nullptr)
        undo->endTransaction();
}

}  // namespace bav
