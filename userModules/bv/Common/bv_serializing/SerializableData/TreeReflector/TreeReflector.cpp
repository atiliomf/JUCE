
namespace bav
{
TreeReflector::TreeReflector (const ValueTree& treeToUse, bool loading)
    : tree (treeToUse), loadingData (loading)
{
    jassert (tree.isValid());
}

bool TreeReflector::isLoading() const { return loadingData; }

bool TreeReflector::isSaving() const { return ! isLoading(); }

void TreeReflector::loadDataChild (const String& propertyName, SerializableData& data) const
{
    data.restoreFromTree (tree.getChildWithName (propertyName));
}

void TreeReflector::saveDataChild (const String& propertyName, SerializableData& data)
{
    tree.appendChild (data.saveToTree (propertyName), nullptr);
}

ValueTree& TreeReflector::getRawDataTree() { return tree; }

}  // namespace bav
