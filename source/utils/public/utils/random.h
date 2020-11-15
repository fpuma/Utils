#pragma once

#include <cstdlib>
#include <ctime>


namespace puma
{
    class Random
    {
    public:
        
        static void refreshSeed()
        {
            std::srand( (unsigned int) std::time( nullptr ) );
        }

        static int generateRandom( unsigned int _min, unsigned int _max )
        {
            return _min + std::rand() / ((RAND_MAX + _min) / _max);
        }

    };
}