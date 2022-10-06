#pragma once

namespace bav
{
struct TreeReflector final
{
    explicit TreeReflector (const ValueTree& treeToUse, bool loading);

    bool isLoading() const;
    bool isSaving() const;

    ValueTree& getRawDataTree();

    template < typename Type >
    void add (const String& propertyName, Type& object);

private:
    template < typename Type >
    void load (const String& propertyName, Type& object);

    template < typename Type >
    void save (const String& propertyName, Type& object);

    template < typename Type >
    void loadObject (const String& propertyName, Type& object) const;

    template < typename Type >
    void saveObject (const String& propertyName, Type& object);

    template < class ContainerType >
    void loadContainer (const String& propertyName, ContainerType& container);

    template < class ContainerType >
    void saveContainer (const String& propertyName, ContainerType& container);

    void loadDataChild (const String& propertyName, SerializableData& data) const;
    void saveDataChild (const String& propertyName, SerializableData& data);

    ValueTree  tree;
    const bool loadingData;
};

}  // namespace bav

#include "detail/detail.h"
