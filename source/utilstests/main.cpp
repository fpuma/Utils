#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main( int argc, char* argv[] )
{
    ::testing::InitGoogleTest( &argc, argv );
    ::RUN_ALL_TESTS();
    return 0;
}

//Adding all headers to check for build issues
#include <utils/containers/containedvector.h>
#include <utils/containers/mappedrealizationcontainer.h>
#include <utils/containers/uniquerealizationcontainer.h>
#include <utils/events/eventmanager.h>
#include <utils/events/ievent.h>
#include <utils/events/ieventlistener.h>
#include <utils/geometry/shapes/chain.h>
#include <utils/geometry/shapes/cirlce.h>
#include <utils/geometry/shapes/polygon.h>
#include <utils/geometry/shapes/shape.h>
#include <utils/geometry/shapes/shapedefs.h>
#include <utils/geometry/geometryhelpers.h>
#include <utils/geometry/vec2.h>
#include <utils/geometry/vec3.h>
#include <utils/graphics/color.h>
#include <utils/graphics/dimensions.h>
#include <utils/statemachine/istate.h>
#include <utils/statemachine/statemachine.h>
#include <utils/defaultinstance.h>
#include <utils/formatstring.h>
#include <utils/genericid.h>
#include <utils/hash.h>
#include <utils/multithreadbuffer.h>
#include <utils/noncopyable.h>
#include <utils/numerictypes.h>
#include <utils/random.h>