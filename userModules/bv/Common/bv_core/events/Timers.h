#pragma once

namespace bav::events
{
struct TimerCallback : private juce::Timer
{
    TimerCallback (std::function< void() > callbackToUse,
                   int                     rateInMs = 100);

    virtual ~TimerCallback();

private:
    void timerCallback() final;

    std::function< void() > callback;
};

}  // namespace bav::events
