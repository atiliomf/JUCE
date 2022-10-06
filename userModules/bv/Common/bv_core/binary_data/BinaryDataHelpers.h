
#pragma once

#if BV_HAS_BINARY_DATA
#    include "BinaryData.h"
#endif


namespace bav
{
struct RawData
{
    RawData() = default;
    explicit RawData (const String& fileToFind);
    explicit RawData (const char* fileToFind);

    bool isValid() const;

    const char* data = nullptr;
    int         size {0};
};

/** To easily access some binary data:
 @code
 RawData binary {"File.wav"};
 doSomethingWithData (binary.data, binary.size);
 @end-code
 */

}  // namespace bav
