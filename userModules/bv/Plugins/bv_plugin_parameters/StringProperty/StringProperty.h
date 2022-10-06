#pragma once

namespace bav::plugin
{
class StringProperty : public SerializableData
{
public:
    StringProperty (const String& propertyName, const String& defaultValue = {});
    virtual ~StringProperty() = default;

    StringProperty* operator->() { return this; }

    void refreshDefault();
    void resetToDefault();

    String getDefault() const;
    void   setDefault (const String& newDefault);

    String get() const;
    void   set (const String& newValue);

    String getPropertyName() const;

    void setUndoManager (UndoManager& managerToUse);

    struct Listener
    {
        Listener (StringProperty& propertyToUse);
        virtual ~Listener();

        virtual void propertyValueChanged (const String& /* newValue */) { }
        virtual void propertyDefaultChanged (const String& /* newDefault */) { }

    private:
        StringProperty& property;
    };

private:
    void serialize (TreeReflector& ref) final;

    void setInternal (const String& newValue);
    void setDefaultInternal (const String& newDefault);

    String currentDefault;
    String currentValue;

    UndoManager* um = nullptr;

    juce::ListenerList< Listener > listeners;

    const String valueChangeTransactionName;
    const String defaultChangeTransactionName;
};

}  // namespace bav::plugin
