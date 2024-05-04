#pragma once

#include <utils/events/ievent.h>

namespace puma
{
    template <class BaseEvent = IEvent>
    class IEventListener
    {
    public:
        virtual ~IEventListener() = default;

        virtual void onEvent(const BaseEvent& _event) = 0;
    };
}