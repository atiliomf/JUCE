
#pragma once

namespace bav::serializing
{
String valueTreeToJSON (const ValueTree& v);

ValueTree valueTreeFromJSON (const String& jsonText);

}  // namespace bav::serializing
