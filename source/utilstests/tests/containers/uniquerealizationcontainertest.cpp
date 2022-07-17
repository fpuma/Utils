#include "uniquerealizationcontainertest.h"

TEST( Containers, UniqueRealizationContainer )
{
    TestUniqueRealizationContainer container;
    
    EXPECT_DEATH( container.add<ITestInterface>(), "itRegisteredClass != m_registeredClasses.end"); 
    EXPECT_DEATH( container.remove<ITestInterface>(), "itElement != m_elements.end" );
    EXPECT_DEATH( container.get<TestRealization0>(), "itRegisteredClass != m_registeredClasses.end" );

    //====================================================================================

    EXPECT_FALSE( container.isRegistered<ITestInterface>() );
    EXPECT_FALSE( container.isRegistered<TestRealization0>() );
    
    container.registerInterface<ITestInterface, TestRealization0>();

    auto dummyLambda = [&]() {container.registerInterface<ITestInterface, TestRealization1>(); };
    EXPECT_DEATH( dummyLambda(), "itRegisteredClass == m_registeredClasses.end" );

    EXPECT_TRUE( container.isRegistered<ITestInterface>() );
    EXPECT_TRUE( container.isRegistered<TestRealization0>() );
    
    EXPECT_FALSE( container.isRegistered<TestRealization1>() );
    container.registerClass<TestRealization1>();
    EXPECT_TRUE( container.isRegistered<TestRealization1>() );

    //====================================================================================

    EXPECT_CALL( container.MockTest, onAdded() ).Times(2);
    EXPECT_CALL( container.MockTest, onRemoved() ).Times(1);

    //====================================================================================

    EXPECT_FALSE( container.contains<TestRealization0>() );
    EXPECT_FALSE( container.contains<TestRealization1>() );

    TestRealization0* testRealization0ptr = container.add<TestRealization0>();
    EXPECT_DEATH( container.add<ITestInterface>(), "itElement == m_elements.end" );
    TestRealization1* testRealization1ptr = container.add<TestRealization1>();

    EXPECT_TRUE( container.contains<ITestInterface>() );
    EXPECT_TRUE( container.contains<TestRealization0>() );
    EXPECT_TRUE( container.contains<TestRealization1>() );

    EXPECT_EQ( container.size(), 2 );

    EXPECT_EQ( container.get<ITestInterface>(), testRealization0ptr );
    EXPECT_EQ( container.get<TestRealization0>(), testRealization0ptr );
    EXPECT_EQ( container.get<TestRealization1>(), testRealization1ptr );

    //====================================================================================

    container.remove<ITestInterface>();

    EXPECT_DEATH( container.get<ITestInterface>(), "itElement != m_elements.end" );
    EXPECT_DEATH( container.get<TestRealization0>(), "itElement != m_elements.end" );
    EXPECT_EQ( container.get<TestRealization1>(), testRealization1ptr );

    EXPECT_EQ( container.size(), 1 );
}