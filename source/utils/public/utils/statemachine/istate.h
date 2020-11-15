#pragma once

#include <utils/numerictypes.h>

namespace puma
{
    using StateID = s32;
    constexpr s32 kInvalidStateID = -1;

    class StateMachineInfo
    {
    public:
        StateID currentStateId = kInvalidStateID;
    };

    template <class SMInfo>
    class IState
    {
    public:
        
        static_assert(std::is_base_of<StateMachineInfo, SMInfo>::value);

        using Info = SMInfo;
        
        explicit IState(StateID _stateId)
            : m_stateId (_stateId){}
        virtual ~IState() = default;

        StateID getID() const { return m_stateId; };
        
        virtual void onEnter( SMInfo& _info ) = 0;
        virtual void update ( SMInfo& _info ) = 0;
        virtual void onExit ( SMInfo& _info ) = 0;

    protected:

        const StateID m_stateId = kInvalidStateID;

    };
}