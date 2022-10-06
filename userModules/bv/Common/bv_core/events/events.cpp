
namespace bav::events
{
void Broadcaster::trigger()
{
    listeners.call ([] (Reciever& l)
                    { l.callback(); });
}

Broadcaster::Reciever::Reciever (Broadcaster& b) : broadcaster (b)
{
    broadcaster.listeners.add (this);
}

Broadcaster::Reciever::~Reciever()
{
    broadcaster.listeners.remove (this);
}

Listener::Listener (Broadcaster& b, std::function< void() > func)
    : Reciever (b),
      callbackFunc (std::move (func))
{
}

void Listener::callback()
{
    callbackFunc();
}

TimerCallback::TimerCallback (std::function< void() > callbackToUse,
                              int                     rateInMs)
    : callback (std::move (callbackToUse))
{
    startTimer (rateInMs);
}

TimerCallback::~TimerCallback()
{
    stopTimer();
}

void TimerCallback::timerCallback()
{
    callback();
}

}  // namespace bav::events
