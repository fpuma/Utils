#include "misccontainertest.h"

TEST(MappedRealizationContainer, Register)
{
    MappedRealizationContainer<int, ITestBase> container;
    const MappedRealizationContainer<int, ITestBase>& constContainer = container;
#ifdef _DEBUG
    EXPECT_DEATH( container.add<ITestInterface>(0), "itRegisteredClass != m_registeredClasses.end" );
    EXPECT_DEATH( container.remove<ITestInterface>(1), "itElement != m_elements.end" );
    EXPECT_DEATH( container.get<TestRealization0>(2), "itElement != m_elements.end" );
    EXPECT_DEATH( container.getSafely<TestRealization1>(3), "itElement != m_elements.end" );
    EXPECT_DEATH( constContainer.get<TestRealization0>( 2 ), "itElement != m_elements.end" );
    EXPECT_DEATH( constContainer.getSafely<TestRealization1>( 3 ), "itElement != m_elements.end" );
#else
    auto ptr0 = container.add<ITestInterface>(0);
    container.remove<ITestInterface>(1);
    auto ptr1 = container.get<TestRealization1>(2);
    auto ptr2 = constContainer.getSafely<TestRealization1>( 3 );

    EXPECT_EQ( ptr0, nullptr );
    EXPECT_EQ( ptr1, nullptr );
    EXPECT_EQ( ptr2, nullptr );
#endif

    EXPECT_FALSE( container.isRegistered<ITestInterface>() );
    EXPECT_FALSE( container.isRegistered<TestRealization0>() );
    EXPECT_FALSE( container.isRegistered<TestRealization1>() );

    EXPECT_FALSE( container.contains<ITestInterface>(0) );
    EXPECT_FALSE( container.contains<TestRealization0>(1) );
    EXPECT_FALSE( container.contains<TestRealization1>(2) );

    container.registerInterface<ITestInterface, TestRealization0>();
    container.registerClass<TestRealization1>();

    EXPECT_TRUE( container.isRegistered<ITestInterface>() );
    EXPECT_TRUE( container.isRegistered<TestRealization0>() );
    EXPECT_TRUE( container.isRegistered<TestRealization1>() );

    EXPECT_FALSE( container.contains<ITestInterface>(0) );
    EXPECT_FALSE( container.contains<TestRealization0>(1) );
    EXPECT_FALSE( container.contains<TestRealization1>(2) );
}

TEST( MappedRealizationContainer, Add )
{
    MappedRealizationContainer<int, ITestBase> container;

    container.registerInterface<ITestInterface, TestRealization0>();
    container.registerClass<TestRealization1>();

    EXPECT_FALSE( container.contains<ITestInterface>(0) );
    EXPECT_FALSE( container.contains<TestRealization0>(1) );
    EXPECT_FALSE( container.contains<TestRealization1>(2) );

    container.add<ITestInterface>(0);

    EXPECT_TRUE( container.contains<ITestInterface>( 0 ) );
    EXPECT_TRUE( container.contains<TestRealization0>( 0 ) );
    EXPECT_FALSE( container.contains<TestRealization1>( 1 ) );

    container.add<TestRealization1>(1);

    EXPECT_TRUE( container.contains<ITestInterface>( 0 ) );
    EXPECT_TRUE( container.contains<TestRealization0>( 0 ) );
    EXPECT_TRUE( container.contains<TestRealization1>( 1 ) );

#ifdef _DEBUG
    EXPECT_DEATH( container.add<ITestInterface>( 0 ), "contains" );
    EXPECT_DEATH( container.add<TestRealization0>( 0 ), "contains" );
    EXPECT_DEATH( container.add<TestRealization1>( 1 ), "contains" );
#else
    auto ptr = container.add<TestRealization0>(0);
    EXPECT_EQ( ptr, nullptr );
#endif
}

TEST( MappedRealizationContainer, Gets )
{
    MappedRealizationContainer<int, ITestBase> container;

    container.registerInterface<ITestInterface, TestRealization0>();
    container.registerClass<TestRealization1>();
#ifdef _DEBUG
    EXPECT_DEATH( container.get<TestRealization0>(0), "itElement != m_elements.end" );
    EXPECT_DEATH( container.getSafely<TestRealization0>(0), "itElement != m_elements.end" );
#else
    auto ptr = container.get<TestRealization0>(0);
    auto shrdPtr = container.getSafely<TestRealization0>(0);
    EXPECT_EQ( ptr, nullptr );
    EXPECT_EQ( shrdPtr, nullptr );
#endif
    
    container.add<TestRealization0>(1);
    container.add<ITestInterface>( 0 );

    TestRealization0* ptr0 = container.get<TestRealization0>(0);
    const ITestInterface* ptr1 = container.get<ITestInterface>(1);

    std::shared_ptr<TestRealization0> ptr2 = container.getSafely<TestRealization0>(1);
    std::shared_ptr<const ITestInterface> ptr3 = container.getSafely<ITestInterface>(0);

#ifdef _DEBUG
    EXPECT_DEATH( container.clear(), "2 == elementPtr.second.use_count" );
#endif

    EXPECT_NE( ptr0, ptr2.get() );
    EXPECT_EQ( ptr1, ptr2.get() );
    EXPECT_EQ( ptr0, ptr3.get() );
    EXPECT_NE( ptr1, ptr3.get() );
}

TEST( MappedRealizationContainer, Remove )
{
    MappedRealizationContainer<int, ITestBase> container;

    container.registerInterface<ITestInterface, TestRealization0>();
    container.registerClass<TestRealization1>();
#ifdef _DEBUG
    EXPECT_DEATH( container.remove<TestRealization1>( 0 ), "itElement != m_elements.end" );
#else
    container.remove<TestRealization1>( 0 );
#endif

    container.add<ITestInterface>( 0 );

    EXPECT_TRUE( container.contains<TestRealization0>( 0 ) );

#ifdef _DEBUG
    EXPECT_DEATH( container.remove<ITestInterface>(1), "itElement != m_elements.end" );
#else
    container.remove<TestRealization1>( 0 );
#endif

    container.remove<TestRealization0>( 0 );

    EXPECT_FALSE( container.contains<TestRealization0>( 0 ) );
    EXPECT_FALSE( container.contains<TestRealization1>( 0 ) );
}

TEST( MappedRealizationContainer, Visit )
{
    MappedRealizationContainer<int, ITestBase> container;

    container.registerInterface<ITestInterface, TestRealization0>();
    container.registerClass<TestRealization1>();

    int controlNumber = 0;

    container.add<TestRealization0>(0);

    container.visit( 0, [&controlNumber]( std::shared_ptr<ITestBase> _element )
        {
            controlNumber = _element->addOne( controlNumber );
        } );

    EXPECT_EQ( controlNumber, 1 );

    container.add<TestRealization0>( 1 );

    container.visit( 1, [&controlNumber]( std::shared_ptr<ITestBase> _element )
        {
            controlNumber = _element->addOne( controlNumber );
        } );

    EXPECT_EQ( controlNumber, 2 );

    container.add<TestRealization1>( 1 );

    container.visit( 1, [&controlNumber]( std::shared_ptr<ITestBase> _element )
        {
            controlNumber = _element->addOne( controlNumber );
        } );

    EXPECT_EQ( controlNumber, 4 );
}