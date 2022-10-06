
namespace bav::TreeReflectorHelpers
{
String propertyNameToContainerName (const String& propertyName)
{
    return propertyName + "s";
}

String makePropertyNameForElement (const String& propertyName, int& index)
{
    return propertyName + "_" + String (index++);
}

int getNumElementsOfType (const String& propertyName, const ValueTree& tree)
{
    int index = 0;

    do
    {
        const auto i = index;
        if (! tree.hasProperty (makePropertyNameForElement (propertyName, index)))
            return i;
    } while (index < tree.getNumProperties());

    return index;
}

}  // namespace bav::TreeReflectorHelpers
