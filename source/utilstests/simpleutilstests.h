#include <utils/containers/mappedrealizationcontainer.h>
#include <utils/containers/uniquerealizationcontainer.h>
#include <utils/statemachine/istate.h>
#include <utils/statemachine/statemachine.h>
#include <utils/defaultinstance.h>
#include <utils/genericid.h>
#include <utils/noncopyable.h>
#include <utils/numerictypes.h>
#include <utils/random.h>
#include <utils/stringformat.h>

#include <iostream>

namespace puma::utils
{
    class IBase
    {
    public:
        virtual ~IBase() {}
    };

    class ITest : public IBase
    {
    public:
        virtual ~ITest() {}
        virtual int get() const = 0;
        virtual void set( int _num ) = 0;
    };

    class Test : public ITest
    {
    public:

        int get() const override { return m_num; }
        void set( int _num ) override { m_num = _num; }

    private:
        int m_num = 0;
    };

    class Test1 : public ITest
    {
    public:

        int get() const override { return m_num; }
        void set( int _num ) override { m_num = _num; }

    private:
        int m_num = 0;
    };

    class TestSMInfo : public StateMachineInfo
    {
    public:
        int test = 0;
    };

    StateID StateZero = 0;
    StateID StateOne = 1;

    class IStateA : public IState<TestSMInfo>
    {
    public:
        IStateA( StateID _id )
            : IState( _id ) {}
        virtual ~IStateA() {}
    };

    class State0 : public IStateA
    {
    public:
        State0()
            : IStateA( StateZero )
        {}

        void onEnter( TestSMInfo& _int ) override {}
        void update( TestSMInfo& _int ) override {}
        void onExit( TestSMInfo& _int ) override {}

    };

    class State1 : public IStateA
    {
    public:
        State1()
            : IStateA( StateOne )
        {}

        void onEnter( TestSMInfo& _int ) override {}
        void update( TestSMInfo& _int ) override {}
        void onExit( TestSMInfo& _int ) override {}

    };

    class TestID : public GenericID<u32, kMaxU32> {};

    void TestUtils()
    {
        {
            UniqueRealizationContainer<IBase> container0;

            //container0.add<ITest>(); //This is for testing, it triggers an assert
            //container0.remove<Test>(); //This is for testing, it triggers an assert

            container0.registerInterface<ITest, Test>();

            ITest* iTestCont0 = container0.add<ITest>();

            iTestCont0->set( 8 );

            Test* testCont0 = container0.get<Test>();

            assert( testCont0 == iTestCont0 );

            //container0.get<Test1>(); //This is for testing, it triggers an assert

            const char* testExists = container0.exists<Test>() ? "Exists" : "Does not exist";
            const char* test1Exists = container0.exists<Test1>() ? "Exists" : "Does not exist";

            std::cout << "Test: " << testExists << std::endl;
            std::cout << "Test1: " << test1Exists << std::endl;



            UniqueRealizationContainer<IBase> container1 = container0.cloneRegistriesOnly();

            Test* testCont1 = container1.add<Test>();

            testCont1->set( 7 );

            std::cout << "Test in container0 is: " << testCont0->get() << std::endl;
            std::cout << "Test in container1 is: " << testCont1->get() << std::endl;

            std::cout << "Cont0 size is: " << container0.size() << std::endl;


            container0.remove<Test>();
        }
        //---------------------------------------------

        {
            std::cout << std::endl;
            MappedRealizationContainer<int, IBase> mappedContainer0;
            MappedRealizationContainer<int, IBase> mappedContainer1;

            //mappedContainer0.add<Test>( 0 ); //This is for testing, it triggers an assert
            //mappedContainer0.remove<ITest>( 0 ); //This is for testing, it triggers an assert
            //mappedContainer0.get<ITest>( 0 ); //This is for testing, it triggers an assert

            mappedContainer0.registerInterface<ITest, Test>();
            mappedContainer1.registerInterface<ITest, Test>();

            Test* testCont00 = mappedContainer0.add<Test>( 0 );
            ITest* iTestCont00 = mappedContainer0.get<ITest>( 0 );
            assert( testCont00 == iTestCont00 );

            Test* testCont01 = mappedContainer0.add<Test>( 1 );
            Test* testCont10 = mappedContainer1.add<Test>( 0 );
            Test* testCont11 = mappedContainer1.add<Test>( 1 );

            iTestCont00->set( 1 );
            testCont01->set( 2 );
            testCont10->set( 3 );
            testCont11->set( 4 );

            std::cout << "testCont00 is: " << testCont00->get() << std::endl;
            std::cout << "testCont01 is: " << testCont01->get() << std::endl;
            std::cout << "testCont10 is: " << testCont10->get() << std::endl;
            std::cout << "testCont11 is: " << testCont11->get() << std::endl;

            //mappedContainer0.remove<Test1>( 0 ); //This is for testing, it triggers an assert

            //-------------------------------------------------------------------------


            TestSMInfo testInfo;
            testInfo.currentStateId = StateZero;

            StatelessMachine<IStateA> stateMachine;

            stateMachine.addState<State0>();
            stateMachine.addState<State1>();

            stateMachine.start( testInfo );

            stateMachine.update( testInfo );

            stateMachine.uninit( testInfo );

            //-----------------------------------------------------------------------
            TestID testId;

            std::cout << testId.value()<< std::endl << std::endl;

            //--------------------------------------------------------------------

            std::cout << "Testing string format:" << std::endl;
            std::cout << formatString( "This is a number %d and this is a string \"%s\"", 88, "fpuma" );
        }
    }
}