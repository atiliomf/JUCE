#pragma once

namespace bav::events
{
struct Listener : Broadcaster::Reciever
{
    Listener (Broadcaster& b, std::function< void() > func);

private:
    void callback() final;

    std::function< void() > callbackFunc;
};

}  // namespace bav::events
