#pragma once

namespace bav::events
{
class Broadcaster
{
public:
    struct Reciever
    {
        Reciever (Broadcaster& b);
        virtual ~Reciever();
        virtual void callback() = 0;

    private:
        Broadcaster& broadcaster;
    };

    void trigger();

private:
    friend Reciever;

    juce::ListenerList< Reciever > listeners;
};

}  // namespace bav::events
