#pragma once

#include <utils/containers/uniquerealizationcontainer.h>
#include <utils/events/ieventlistener.h>
#include <vector>

namespace puma
{
    template <class BaseEvent = IEvent>
    class EventManager
    {
    public:

        using EventListener = IEventListener<BaseEvent>;

        virtual ~EventManager() = default;

        template<class EventType>
        void registerListener( EventListener* _listener )
        {
            if (!m_events.isRegistered<EventInfo<EventType>>())
            {
                m_events.registerClass<EventInfo<EventType>>();
                m_events.add<EventInfo<EventType>>();
            }

            EventInfo<EventType>* eventInfo = m_events.get<EventInfo<EventType>>();
            auto itListener = std::find( eventInfo->listeners.begin(), eventInfo->listeners.end(), _listener );
            if (itListener == eventInfo->listeners.end())
            {
                eventInfo->listeners.push_back( _listener );
            }
        }

        template<class EventType>
        void unregisterListener( EventListener* _listener )
        {
            if (!m_events.isRegistered<EventInfo<EventType>>()) return;
            
            EventInfo<EventType>* eventInfo = m_events.get<EventInfo<EventType>>();
            auto itListener = std::find( eventInfo->listeners.begin(), eventInfo->listeners.end(), _listener );
            if (itListener != eventInfo->listeners.end())
            {
                eventInfo->listeners.erase( itListener );
            }
        }

        template<class EventType>
        void queueEvent( const EventType& _event )
        {
            if (!m_events.isRegistered<EventInfo<EventType>>()) return;
            m_events.get<EventInfo<EventType>>()->events.emplace_back( std::move( _event ) );
        }

        template<class EventType>
        void executeEvent( const EventType& _event )
        {
            if (!m_events.isRegistered<EventInfo<EventType>>()) return;
            
            EventInfo<EventType>* eventInfo = m_events.get<EventInfo<EventType>>();
            if (nullptr != eventInfo)
            {
                for (EventListener* listener : eventInfo->listeners)
                {
                    listener->onEvent( _event );
                }
            }
        }

        void processEvents()
        {
            m_events.visit( []( std::shared_ptr<EventInfoBase> eventInfo )
                {
                    eventInfo->processEvents();
                } );
        }

    private:

        class EventInfoBase 
        {
        public:
            virtual ~EventInfoBase() = default;

            virtual void processEvents() = 0;
        };

        template <class EventType>
        class EventInfo : public EventInfoBase
        {
        public:

            void processEvents() override
            {
                for (const EventType& event : events)
                {
                    for (EventListener* listener : listeners)
                    {
                        listener->onEvent( event );
                    }
                }
                events.clear();
            }

            std::vector<EventType> events;
            std::vector<EventListener*> listeners;
        };

        UniqueRealizationContainer<EventInfoBase> m_events;

    };
}