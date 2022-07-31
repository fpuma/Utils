#include "containedvectortest.h"

using namespace puma;

TEST( Containers, ContainedVector )
{
    ContainedVector<int, 5> container;

    EXPECT_EQ( container.size(), 0 );

    container.push_back( 0 );
    container.push_back( 1 );
    container.push_back( 2 );

    EXPECT_EQ( container.size(), 3 );

    container.push_back( 3 );
    container.push_back( 4 );

    EXPECT_EQ( container.size(), 5 );
#ifdef _DEBUG
    EXPECT_DEATH( container.push_back( 0 ), "m_currentSize < ContainerSize" );
#endif

    container.erase( container.begin() + 2 );

    EXPECT_EQ( container.size(), 4 );

    EXPECT_EQ( container[0], 0 );
    EXPECT_EQ( container[1], 1 );
    EXPECT_EQ( container[2], 3 );
    EXPECT_EQ( container[3], 4 );

    container.clear();

    EXPECT_EQ( container.size(), 0 );
}