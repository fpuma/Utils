#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <utils/events/eventmanager.h>

using namespace puma;

enum class EventType
{
    A,
    B
};

class TestEvent
{
public:
    TestEvent( EventType _type ) : m_type( _type ) {}
    virtual ~TestEvent() = default;

    EventType getType() const { return m_type; }

private:

    const EventType m_type;
};

class TestEventA : public TestEvent
{
public:
    TestEventA()
        : TestEvent( EventType::A ) {}

    int number = 0;
};

class TestEventB : public TestEvent
{
public:
    TestEventB()
        : TestEvent( EventType::B ) {}

    float number = 0;
};

class TestCbs
{
public:
    virtual void test() {}
};

class TestCbsMock : public TestCbs
{
public:

    MOCK_METHOD( void, test, () );
};

class TestListener : public IEventListener<TestEvent>
{
public:

    void onEvent( const TestEvent& _event )
    {
        TestCbs.test();
        switch (_event.getType())
        {
        case EventType::A:
        {
            const TestEventA& testEvent = static_cast<const TestEventA&>(_event);
            numberi = testEvent.number;
            break;
        }
        case EventType::B:
        {
            const TestEventB& testEvent = static_cast<const TestEventB&>(_event);
            numberf = testEvent.number;
            break;
        }
        }

    }

    int getNumberi() const { return numberi; }
    float getNumberf() const { return numberf; }

    TestCbsMock TestCbs;

private:

    int numberi = 0;
    float numberf = 0.0f;
};

TEST( Events, Events )
{
    TestListener tl;
    EventManager<TestEvent> em;

    TestEventA eventA0;
    eventA0.number = 8;

    TestEventB eventB0;
    eventB0.number = 10.0f;

    em.registerListener<TestEventA>( &tl );
    em.registerListener<TestEventA>( &tl );
    EXPECT_CALL( tl.TestCbs, test() ).Times(4);
    
    em.queueEvent( eventA0 );
    EXPECT_EQ( 0, tl.getNumberi() );
    em.queueEvent( eventB0 );
    em.processEvents();

    EXPECT_EQ( eventA0.number, tl.getNumberi() );
    EXPECT_FLOAT_EQ( 0.0f, tl.getNumberf() );

    TestEventA eventA1;
    eventA1.number = 88;
    em.executeEvent( eventA1 );
    EXPECT_EQ( eventA1.number, tl.getNumberi() );

    em.registerListener<TestEventB>( &tl );
    em.queueEvent( eventB0 );
    em.processEvents();
    EXPECT_FLOAT_EQ( eventB0.number, tl.getNumberf() );

    eventB0.number = 20.0f;

    em.executeEvent( eventB0 );
    EXPECT_FLOAT_EQ( eventB0.number, tl.getNumberf() );

    em.unregisterListener<TestEventA>( &tl );
    em.unregisterListener<TestEventA>( &tl );
    em.executeEvent( eventA0 );

    EXPECT_EQ( eventA1.number, tl.getNumberi() );
}