#pragma once

#include <random>

namespace puma
{
    class Random
    {
    public:
        
        void refreshSeed( unsigned int _seed)
        {
            m_gen.seed( _seed );
        }

        int generateRandom( int _min, int _max )
        {
            std::uniform_int_distribution<int> distribution( _min, _max );
            return distribution( m_gen );
        }

    private:

        std::mt19937 m_gen;
    };
}