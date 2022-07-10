#include "statemachinetest.h"

const StateId TestStates::StateA = StateId( 0 );
const StateId TestStates::StateB = StateId( 1 );

TEST( StateMachine, StateMachine )
{
    TestStateMachine sm;
    sm.addState<MockTestStateA>();
    sm.addState<MockTestStateB>();

    TestStateMachineInfo tsmi;
    TestStateMachineInfo tsmiAux;
    tsmi.currentStateId = TestStates::StateA;

    EXPECT_CALL( sm.getMockStateA(), mockEnter( tsmi ) ).Times( 1 );
    EXPECT_EQ( tsmi.getIntValue(), 0 );

    sm.start( tsmi );

    EXPECT_EQ( tsmi.getIntValue(), 1 );

    EXPECT_CALL( sm.getMockStateA(), mockUpdate( tsmi ) ).Times( 1 );

    tsmiAux.currentStateId = TestStates::StateB;
    tsmiAux.setIntValue( 2 );
    EXPECT_CALL( sm.getMockStateA(), mockExit( tsmiAux ) ).Times( 1 );

    tsmiAux.currentStateId = TestStates::StateB;
    tsmiAux.setIntValue( 3 );
    EXPECT_CALL( sm.getMockStateB(), mockEnter( tsmiAux ) ).Times( 1 );

    sm.update( tsmi );

    tsmiAux.currentStateId = TestStates::StateB;
    tsmiAux.setIntValue( 4 );
    EXPECT_CALL( sm.getMockStateB(), mockUpdate( tsmiAux ) ).Times( 1 );
    
    sm.update( tsmi );
}
