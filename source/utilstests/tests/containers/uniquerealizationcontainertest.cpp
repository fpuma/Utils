#include "misccontainertest.h"

TEST( UniqueRealizationContainer, Register )
{
    UniqueRealizationContainer<ITestBase> container;
    const UniqueRealizationContainer<ITestBase>& constContainer = container;

#ifdef _DEBUG
    EXPECT_DEATH( container.add<ITestInterface>(), "itRegisteredClass != m_registeredClasses.end" );
    EXPECT_DEATH( container.remove<ITestInterface>(), "itRegisteredClass != m_registeredClasses.end" );
    EXPECT_DEATH( container.get<TestRealization0>(), "itRegisteredClass != m_registeredClasses.end" );
    EXPECT_DEATH( container.getSafely<TestRealization1>(), "itRegisteredClass != m_registeredClasses.end" );
    EXPECT_DEATH( constContainer.get<TestRealization0>(), "itRegisteredClass != m_registeredClasses.end" );
    EXPECT_DEATH( constContainer.getSafely<TestRealization1>(), "itRegisteredClass != m_registeredClasses.end" );

#else
   auto ptr0 = container.add<ITestInterface>();
   container.remove<ITestInterface>();
   auto ptr1 = container.get<TestRealization0>();
   auto ptr2 = container.getSafely<TestRealization1>();
   auto ptr3 = constContainer.get<ITestInterface>();
   auto ptr4 = constContainer.getSafely<ITestInterface>();

   EXPECT_EQ( ptr0, nullptr );
   EXPECT_EQ( ptr1, nullptr );
   EXPECT_EQ( ptr2, nullptr );
   EXPECT_EQ( ptr3, nullptr );
   EXPECT_EQ( ptr4, nullptr );
#endif

    EXPECT_FALSE( container.isRegistered<ITestInterface>() );
    EXPECT_FALSE( container.isRegistered<TestRealization0>() );
    EXPECT_FALSE( container.isRegistered<TestRealization1>() );

    EXPECT_FALSE( container.contains<ITestInterface>() );
    EXPECT_FALSE( container.contains<TestRealization0>() );
    EXPECT_FALSE( container.contains<TestRealization1>() );

    container.registerInterface<ITestInterface, TestRealization0>();
    container.registerClass<TestRealization1>();

    EXPECT_TRUE( container.isRegistered<ITestInterface>() );
    EXPECT_TRUE( container.isRegistered<TestRealization0>() );
    EXPECT_TRUE( container.isRegistered<TestRealization1>() );

    EXPECT_FALSE( container.contains<ITestInterface>() );
    EXPECT_FALSE( container.contains<TestRealization0>() );
    EXPECT_FALSE( container.contains<TestRealization1>() );
}

TEST( UniqueRealizationContainer, Add )
{
    TestUniqueRealizationContainer container;

    container.registerInterface<ITestInterface, TestRealization0>();
    container.registerClass<TestRealization1>();

    EXPECT_FALSE( container.contains<ITestInterface>() );
    EXPECT_FALSE( container.contains<TestRealization0>() );
    EXPECT_FALSE( container.contains<TestRealization1>() );

    EXPECT_CALL( container.MockTest, onAdded() ).Times( 2 );

    EXPECT_EQ( container.size(), 0 );
    container.add<ITestInterface>();
    EXPECT_EQ( container.size(), 1 );
    container.add<TestRealization1>();
    EXPECT_EQ( container.size(), 2 );
#ifdef _DEBUG
    EXPECT_DEATH( container.add<TestRealization0>(), "contains" );
#else
    auto ptr = container.add<TestRealization0>();
    EXPECT_EQ( ptr, nullptr );
#endif
    EXPECT_TRUE( container.contains<ITestInterface>() );
    EXPECT_TRUE( container.contains<TestRealization0>() );
    EXPECT_TRUE( container.contains<TestRealization1>() );
}

TEST( UniqueRealizationContainer, Gets )
{
    TestUniqueRealizationContainer container;

    container.registerInterface<ITestInterface, TestRealization0>();
    container.registerClass<TestRealization1>();
#ifdef _DEBUG
    EXPECT_DEATH( container.get<TestRealization0>(), "itElement != m_elements.end" );
    EXPECT_DEATH( container.getSafely<TestRealization0>(), "itElement != m_elements.end" );
#else
    auto ptr = container.get<TestRealization0>();
    auto shrdPtr = container.getSafely<TestRealization0>();
    EXPECT_EQ( ptr, nullptr );
    EXPECT_EQ( shrdPtr, nullptr );
#endif
    EXPECT_CALL( container.MockTest, onAdded() ).Times( 1 );
    container.add<TestRealization0>();

    TestRealization0* ptr0 = container.get<TestRealization0>();
    const ITestInterface* ptr1 = container.get<ITestInterface>();
    
    std::shared_ptr<TestRealization0> ptr2 = container.getSafely<TestRealization0>();
    std::shared_ptr<const ITestInterface> ptr3 = container.getSafely<ITestInterface>();

#ifdef _DEBUG
    EXPECT_DEATH( container.clear(), "2 == elementPtr.use_count" );
#endif

    EXPECT_EQ( ptr0, ptr1 );
    EXPECT_EQ( ptr2, ptr3 );
    EXPECT_EQ( ptr1, ptr2.get() );
}

TEST( UniqueRealizationContainer, Remove )
{
    TestUniqueRealizationContainer container;

    container.registerInterface<ITestInterface, TestRealization0>();
    container.registerClass<TestRealization1>();
#ifdef _DEBUG
    EXPECT_DEATH( container.remove<TestRealization1>(), "itElement != m_elements.end");
#else
    container.remove<TestRealization1>();
#endif

    EXPECT_CALL( container.MockTest, onAdded() ).Times( 1 );
    EXPECT_EQ( container.size(), 0 );
    container.add<ITestInterface>();
    EXPECT_EQ( container.size(), 1 );
#ifdef _DEBUG
    EXPECT_DEATH( container.remove<TestRealization1>(), "itElement != m_elements.end" );
#else
    container.remove<TestRealization1>();
#endif
    EXPECT_CALL( container.MockTest, onRemoved() ).Times( 1 );
    container.remove<TestRealization0>();
    EXPECT_EQ( container.size(), 0 );
    EXPECT_FALSE( container.contains<TestRealization1>() );
}