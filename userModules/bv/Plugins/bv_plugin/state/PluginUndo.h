#pragma once

namespace bav::plugin
{
struct Undo : UndoManager
{
    Undo (ParameterList& listToManage)
        : UndoManager (listToManage)
    {
        listToManage.setUndoManager (*this);
    }

    Undo (State& stateToManage)
        : UndoManager (stateToManage)
    {
        stateToManage.getParameters().setUndoManager (*this);
    }
};

}  // namespace bav::plugin
