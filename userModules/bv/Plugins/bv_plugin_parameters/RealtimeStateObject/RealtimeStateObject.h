#pragma once

namespace bav::plugin
{
template < typename OwnedObjectType >
class RealtimeStateObject
{
public:
    struct RealtimeScopedWrite;
    struct RealtimeScopedRead;
    struct NonrealtimeScopedWrite;
    struct NonrealtimeScopedRead;

    RealtimeScopedWrite    realtimeWrite();
    RealtimeScopedRead     realtimeRead();
    NonrealtimeScopedWrite nonrealtimeWrite();
    NonrealtimeScopedRead  nonrealtimeRead();

private:
    friend struct RealtimeScopedWrite;
    friend struct RealtimeScopedRead;
    friend struct NonrealtimeScopedWrite;
    friend struct NonrealtimeScopedRead;

    OwnedObjectType& realtime_beginWrite();
    void             realtime_endWrite();

    const OwnedObjectType& realtime_beginRead();
    void                   realtime_endRead();

    OwnedObjectType& nonRealtime_beginWrite();
    void             nonRealtime_endWrite();

    const OwnedObjectType& nonrealtime_beginRead();
    void                   nonrealtime_endRead();


    OwnedObjectType object;
};

}  // namespace bav::plugin

#include "detail.h"
