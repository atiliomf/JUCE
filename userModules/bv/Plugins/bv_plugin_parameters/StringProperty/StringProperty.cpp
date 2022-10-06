
namespace bav::plugin
{
StringProperty::StringProperty (const String& propertyName, const String& defaultValue)
    : SerializableData (propertyName),
      valueChangeTransactionName (TRANS ("Changed") + " " + propertyName),
      defaultChangeTransactionName (TRANS ("Changed default value of") + " " + propertyName)
{
    currentDefault = defaultValue;
    currentValue   = defaultValue;
}

String StringProperty::getPropertyName() const
{
    return "StringProperty";
}

void StringProperty::refreshDefault()
{
    setDefault (get());
}

void StringProperty::resetToDefault()
{
    set (getDefault());
}

String StringProperty::getDefault() const
{
    return currentDefault;
}

void StringProperty::setDefault (const String& newDefault)
{
    if (newDefault == getDefault()) return;

    UndoManager::ScopedTransaction s {um, defaultChangeTransactionName};
    setDefaultInternal (newDefault);
}

String StringProperty::get() const
{
    return currentValue;
}

void StringProperty::set (const String& newValue)
{
    if (newValue == get()) return;

    UndoManager::ScopedTransaction s {um, valueChangeTransactionName};
    setInternal (newValue);
}

void StringProperty::setUndoManager (UndoManager& managerToUse)
{
    um = &managerToUse;
}

void StringProperty::serialize (TreeReflector& ref)
{
    auto& tree = ref.getRawDataTree();

    if (ref.isLoading())
    {
        setInternal (tree.getProperty ("PropertyValue"));
        setDefaultInternal (tree.getProperty ("DefaultValue"));
    }
    else
    {
        tree.setProperty ("PropertyValue", get(), nullptr);
        tree.setProperty ("DefaultValue", getDefault(), nullptr);
    }
}

void StringProperty::setInternal (const String& newValue)
{
    currentValue = TRANS (newValue);
    listeners.call ([&] (Listener& l)
                    { l.propertyValueChanged (currentValue); });
}

void StringProperty::setDefaultInternal (const String& newDefault)
{
    currentDefault = TRANS (newDefault);
    listeners.call ([&] (Listener& l)
                    { l.propertyDefaultChanged (newDefault); });
}

StringProperty::Listener::Listener (StringProperty& propertyToUse)
    : property (propertyToUse)
{
    property.listeners.add (this);
}

StringProperty::Listener::~Listener()
{
    property.listeners.remove (this);
}

}  // namespace bav::plugin
