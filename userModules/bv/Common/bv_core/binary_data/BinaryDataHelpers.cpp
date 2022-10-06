

namespace bav
{
#if BV_HAS_BINARY_DATA

RawData::RawData (const String& fileToFind)
    : RawData (fileToFind.toRawUTF8())
{
}

RawData::RawData (const char* fileToFind)
{
    using namespace BinaryData;

    for (int index = 0; index < namedResourceListSize; ++index)
    {
        const auto binaryName = namedResourceList[index];

        if (getNamedResourceOriginalFilename (binaryName) == fileToFind)
        {
            data = BinaryData::getNamedResource (binaryName, size);
            return;
        }
    }
}


bool RawData::isValid() const
{
    return data != nullptr && size > 0;
}


#else /* if BV_HAS_BINARY_DATA */

RawData::RawData (const String&)
{
}

RawData::RawData (const char*)
{
}

bool RawData::isValid() const { return false; }

#endif

}  // namespace bav
