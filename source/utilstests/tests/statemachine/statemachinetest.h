#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <utils/statemachine/statemachine.h>

using namespace puma;

const struct TestStates
{
    static const StateId StateA;
    static const StateId StateB;
};

class TestStateMachineInfo : public StateMachineInfo
{
public:

    bool operator == ( const TestStateMachineInfo& _other ) const
    {
        return (currentStateId == _other.currentStateId) && (m_intValue == _other.m_intValue);
    }

    int getIntValue() const { return m_intValue; }
    void setIntValue( int _value ) { m_intValue = _value; }

private:

    int m_intValue = 0;
};

class ITestStateBase : public IState<TestStateMachineInfo>
{
public:

    ITestStateBase( StateId _stateId )
        : IState( _stateId ) {}

    virtual void mockEnter( TestStateMachineInfo& _info ) {}
    virtual void mockUpdate( TestStateMachineInfo& _info ) {}
    virtual void mockExit( TestStateMachineInfo& _info ) {}
};

class TestStateA : public ITestStateBase
{
public:

    TestStateA()
        : ITestStateBase( TestStates::StateA ) {}

    void onEnter( TestStateMachineInfo& _info ) override 
    { 
        mockEnter( _info ); _info.setIntValue( 1 ); 
    }

    void update( TestStateMachineInfo& _info )  override 
    { 
        mockUpdate( _info ); 
        _info.currentStateId = TestStates::StateB; 
        _info.setIntValue( 2 );
    }

    void onExit( TestStateMachineInfo& _info )  override 
    { 
        mockExit( _info ); 
        _info.setIntValue( 3 ); 
    }
};

class TestStateB : public ITestStateBase
{
public:

    TestStateB()
        : ITestStateBase( TestStates::StateB ) {}

    void onEnter( TestStateMachineInfo& _info ) override 
    { 
        mockEnter( _info ); _info.setIntValue( 4 ); 
    }
    
    void update( TestStateMachineInfo& _info )  override 
    { 
        mockUpdate( _info ); 
        _info.setIntValue( 5 ); 
    }
    
    void onExit( TestStateMachineInfo& _info )  override 
    { 
        mockExit( _info ); 
        _info.setIntValue( 6 ); 
    }
};

class MockTestStateA : public TestStateA
{
public:
    MOCK_METHOD( void, mockEnter, (TestStateMachineInfo& ) );
    MOCK_METHOD( void, mockUpdate, (TestStateMachineInfo& ) ); 
    MOCK_METHOD( void, mockExit, (TestStateMachineInfo& ) ); 
};

class MockTestStateB : public TestStateB
{
public:
    MOCK_METHOD( void, mockEnter, (TestStateMachineInfo&) );
    MOCK_METHOD( void, mockUpdate, (TestStateMachineInfo&) );
    MOCK_METHOD( void, mockExit, (TestStateMachineInfo&) );
};

class TestStateMachine : public StatelessMachine<ITestStateBase>
{
public:

    MockTestStateA& getMockStateA() { return static_cast<MockTestStateA&>(*getState( TestStates::StateA )); }
    MockTestStateB& getMockStateB() { return static_cast<MockTestStateB&>(*getState( TestStates::StateB )); }

};