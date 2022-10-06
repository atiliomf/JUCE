#pragma once

#include "helpers.h"

namespace bav
{
template < typename Type >
void TreeReflector::add (const String& propertyName, Type& object)
{
    if constexpr (std::is_pointer< Type >())
    {
        if constexpr (! std::is_null_pointer< Type >())
            if (object != nullptr)
                add (propertyName, *object);
    }
    else
    {
        if (loadingData)
            load (propertyName, object);
        else
            save (propertyName, object);
    }
}

template < typename Type >
void TreeReflector::load (const String& propertyName, Type& object)
{
    if constexpr (! std::is_const< Type >())
    {
        if constexpr (isSerializable< Type >())
            loadDataChild (propertyName, object);
        else if constexpr (is_container< Type >())
            loadContainer (propertyName, object);
        else
            loadObject (propertyName, object);
    }
}

template < typename Type >
void TreeReflector::save (const String& propertyName, Type& object)
{
    if constexpr (isSerializable< Type >())
        saveDataChild (propertyName, object);
    else if constexpr (is_container< Type >())
        saveContainer (propertyName, object);
    else
        saveObject (propertyName, object);
}

template < typename Type >
void TreeReflector::loadObject (const String& propertyName, Type& object) const
{
    if (tree.hasProperty (propertyName))
    {
        const juce::var& var = tree.getProperty (propertyName);

        if constexpr (std::is_enum< Type >())
            object = TreeReflectorHelpers::toEnum< Type > (var);
        else
            object = fromVar< Type > (var);
    }
}

template < typename Type >
void TreeReflector::saveObject (const String& propertyName, Type& object)
{
    juce::var var;

    if constexpr (std::is_enum< Type >())
        var = TreeReflectorHelpers::fromEnum (object);
    else
        var = toVar (object);

    tree.setProperty (propertyName, var, nullptr);
}

template < class ContainerType >
void TreeReflector::loadContainer (const String& propertyName, ContainerType& container)
{
    const auto child = tree.getChildWithName (TreeReflectorHelpers::propertyNameToContainerName (propertyName));
    if (! child.isValid()) return;

    TreeReflector ref {child, true};

    TreeReflectorHelpers::addContainer (ref, container, propertyName);
}

template < class ContainerType >
void TreeReflector::saveContainer (const String& propertyName, ContainerType& container)
{
    ValueTree     child {TreeReflectorHelpers::propertyNameToContainerName (propertyName)};
    TreeReflector ref {child, false};

    TreeReflectorHelpers::addContainer (ref, container, propertyName);

    tree.appendChild (ref.getRawDataTree(), nullptr);
}

}  // namespace bav
