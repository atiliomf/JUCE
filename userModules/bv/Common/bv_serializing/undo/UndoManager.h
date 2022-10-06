#pragma once

namespace bav
{
class UndoManager
{
public:
    UndoManager (SerializableData& stateToManage);
    virtual ~UndoManager() = default;

    bool undo();
    bool redo();

    bool hasUndo() const;
    bool hasRedo() const;

    bool isInMiddleOfTransaction() const;

    void undoToLastTransaction();

    void beginNewTransaction (const String& name = {}, bool force = false);
    void endTransaction();

    String            getCurrentTransactionName() const;
    String            getNextUndoTransactionName() const;
    String            getNextRedoTransactionName() const;
    juce::StringArray getUndoTransactionNames() const;
    juce::StringArray getRedoTransactionNames() const;

    void clearUndoHistory();

    events::Broadcaster& getBroadcaster();

    struct ScopedTransaction
    {
        ScopedTransaction (UndoManager* um, const String& name = {});
        ~ScopedTransaction();

    private:
        UndoManager* undo;
    };

private:
    struct SavedState
    {
        SavedState (const ValueTree& tree, const String& name);

        ValueTree state;
        String    transactionName;
    };

    void saveState();
    void loadState (const SavedState& stateToLoad);

    SerializableData& state;

    String transactionName;
    bool   changing {false};

    std::vector< SavedState >            storedStates;
    std::vector< SavedState >::size_type currentStep {0};

    events::Broadcaster broadcaster;
};

}  // namespace bav
