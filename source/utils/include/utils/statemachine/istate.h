#pragma once

#include <utils/genericid.h>
#include <utils/numerictypes.h>

namespace puma
{
    class StateId;
}

DECLARE_GENERIC_ID( puma::StateId, puma::s32, -1 );

namespace puma
{

    class StateMachineInfo
    {
    public:
        StateId currentStateId;
    };

    template <class SMInfo>
    class IState
    {
    public:
        
        static_assert(std::is_base_of<StateMachineInfo, SMInfo>::value);

        using Info = SMInfo;
        
        explicit IState( StateId _stateId)
            : m_stateId (_stateId){}
        virtual ~IState() = default;

        StateId getId() const { return m_stateId; };
        
        virtual void onEnter( SMInfo& _info ) = 0;
        virtual void update ( SMInfo& _info ) = 0;
        virtual void onExit ( SMInfo& _info ) = 0;

    protected:

        const StateId m_stateId;

    };
}