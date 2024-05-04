#pragma once
#include <utils/statemachine/istate.h>

#include <assert.h>

namespace puma
{
    template<class BaseState>
    class StatelessMachine
    {
    public:
        StatelessMachine() {}
        ~StatelessMachine() = default;

        static_assert(std::is_base_of<IState<typename BaseState::Info>, BaseState>::value);

        template<class StateClass>
        void addState()
        {
            static_assert(std::is_base_of<BaseState, StateClass>::value);
            std::unique_ptr<StateClass> uPtr = std::make_unique<StateClass>();
            m_states.emplace( std::make_pair( uPtr->getId(), std::move( uPtr ) ) );
        }

        void start( typename BaseState::Info& _info )
        {
            BaseState* currentState = getState( _info.currentStateId );
            currentState->onEnter( _info );
        }

        void update( typename BaseState::Info& _info )
        {
            BaseState* currentState = getState( _info.currentStateId );
            currentState->update( _info );
            StateId newStateId = _info.currentStateId;

            if ( currentState->getId() != newStateId )
            {
                currentState->onExit( _info );
                currentState = getState( newStateId );
                currentState->onEnter( _info );
            }
        }

        void uninit( typename BaseState::Info& _info )
        {
            BaseState* currentState = getState( _info.currentStateId );
            currentState->onExit( _info );
            m_states.clear();
        }

    protected:

        BaseState* getState( StateId _stateId )
        {
            assert( _stateId.isValid() );
#ifdef _DEBUG
            auto itState = m_states.find( _stateId );
            assert( itState != m_states.end() );
#endif
            BaseState* result = m_states[_stateId].get();
            assert( nullptr != result );
            return result;
        }

        std::map<StateId,std::unique_ptr<BaseState>> m_states;
    };
}