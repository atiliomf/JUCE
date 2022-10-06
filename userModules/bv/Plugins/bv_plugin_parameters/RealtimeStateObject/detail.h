#pragma once

namespace bav::plugin
{
/*--- realtime write ---*/

template < typename OwnedObjectType >
OwnedObjectType& RealtimeStateObject< OwnedObjectType >::realtime_beginWrite()
{
}

template < typename OwnedObjectType >
void RealtimeStateObject< OwnedObjectType >::realtime_endWrite()
{
}

template < typename OwnedObjectType >
struct RealtimeStateObject< OwnedObjectType >::RealtimeScopedWrite
{
    RealtimeScopedWrite (RealtimeStateObject& state)
        : owner (state), object (owner.realtime_beginWrite())
    {
    }

    ~RealtimeScopedWrite()
    {
        owner.realtime_endWrite();
    }

    OwnedObjectType& get() { return object; }
    OwnedObjectType& operator()() { return object; }
    OwnedObjectType& operator*() { return object; }
    OwnedObjectType* operator->() { return &object; }

private:
    RealtimeStateObject& owner;
    OwnedObjectType&     object;
};

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::RealtimeScopedWrite RealtimeStateObject< OwnedObjectType >::realtimeWrite()
{
    return {*this};
}

/*--- realtime read ---*/

template < typename OwnedObjectType >
const OwnedObjectType& RealtimeStateObject< OwnedObjectType >::realtime_beginRead()
{
}

template < typename OwnedObjectType >
void RealtimeStateObject< OwnedObjectType >::realtime_endRead()
{
}

template < typename OwnedObjectType >
struct RealtimeStateObject< OwnedObjectType >::RealtimeScopedRead
{
    RealtimeScopedRead (RealtimeStateObject& state)
        : owner (state), object (owner.realtime_beginRead())
    {
    }

    ~RealtimeScopedRead()
    {
        owner.realtime_endRead();
    }

    const OwnedObjectType& get() const { return object; }
    const OwnedObjectType& operator()() const { return object; }
    const OwnedObjectType& operator*() const { return object; }
    const OwnedObjectType* operator->() const { return &object; }

private:
    RealtimeStateObject&   owner;
    const OwnedObjectType& object;
};

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::RealtimeScopedRead RealtimeStateObject< OwnedObjectType >::realtimeRead()
{
    return {*this};
}


/*--- non realtime write ---*/

template < typename OwnedObjectType >
OwnedObjectType& RealtimeStateObject< OwnedObjectType >::nonRealtime_beginWrite()
{
}

template < typename OwnedObjectType >
void RealtimeStateObject< OwnedObjectType >::nonRealtime_endWrite()
{
}

template < typename OwnedObjectType >
struct RealtimeStateObject< OwnedObjectType >::NonrealtimeScopedWrite
{
    NonrealtimeScopedWrite (RealtimeStateObject& state)
        : owner (state), object (owner.nonRealtime_beginWrite())
    {
    }

    ~NonrealtimeScopedWrite()
    {
        owner.nonRealtime_endWrite();
    }

    OwnedObjectType& get() { return object; }
    OwnedObjectType& operator()() { return object; }
    OwnedObjectType& operator*() { return object; }
    OwnedObjectType* operator->() { return &object; }

private:
    RealtimeStateObject& owner;
    OwnedObjectType&     object;
};

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::NonrealtimeScopedWrite RealtimeStateObject< OwnedObjectType >::nonrealtimeWrite()
{
    return {*this};
}


/*--- non realtime read ---*/

template < typename OwnedObjectType >
const OwnedObjectType& RealtimeStateObject< OwnedObjectType >::nonrealtime_beginRead()
{
}

template < typename OwnedObjectType >
void RealtimeStateObject< OwnedObjectType >::nonrealtime_endRead()
{
}

template < typename OwnedObjectType >
struct RealtimeStateObject< OwnedObjectType >::NonrealtimeScopedRead
{
    NonrealtimeScopedRead (RealtimeStateObject& state)
        : owner (state), object (owner.nonrealtime_beginRead())
    {
    }

    ~NonrealtimeScopedRead()
    {
        owner.nonrealtime_endRead();
    }

    const OwnedObjectType& get() const { return object; }
    const OwnedObjectType& operator()() const { return object; }
    const OwnedObjectType& operator*() const { return object; }
    const OwnedObjectType* operator->() const { return &object; }

private:
    RealtimeStateObject&   owner;
    const OwnedObjectType& object;
};

template < typename OwnedObjectType >
typename RealtimeStateObject< OwnedObjectType >::NonrealtimeScopedRead RealtimeStateObject< OwnedObjectType >::nonrealtimeRead()
{
    return {*this};
}

}  // namespace bav::plugin
