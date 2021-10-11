#pragma once

#include <utils/geometry/shapes/chain.h>
#include <utils/geometry/shapes/cirlce.h>
#include <utils/geometry/shapes/polygon.h>
#include <utils/geometry/shapes/shapedefs.h>
#include <utils/graphics/dimensions.h>
#include <utils/numerictypes.h>

namespace puma
{
    class Shape
    {
    public:

        Shape() {}
        Shape( const Shape& _other )
        {
            copyFromShape( _other );
        }
        ~Shape() {}

        Shape& operator = ( const Shape& _other )
        {
            copyFromShape( _other );
            return *this;
        }

        ShapeType getShapeType() const { return m_shapeType; }

        void setAsChain     ( const Chain& _chain )            { m_shape.chain = _chain; m_shapeType = ShapeType::Chain; }
        void setAsCircle    ( const Circle& _circle )          { m_shape.circle = _circle; m_shapeType = ShapeType::Circle; }
        void setAsPolygon   ( const Polygon& _polygon )        { m_shape.polygon = _polygon; m_shapeType = ShapeType::Polygon; }
        void setAsPolygon   ( const Rectangle& _rectangle )
        {
            Polygon polygon;
            polygon.vertices.push_back( { _rectangle.lowerBoundary.x, _rectangle.lowerBoundary.y } );
            polygon.vertices.push_back( { _rectangle.upperBoundary.x, _rectangle.lowerBoundary.y } );
            polygon.vertices.push_back( { _rectangle.upperBoundary.x, _rectangle.upperBoundary.y } );
            polygon.vertices.push_back( { _rectangle.lowerBoundary.x, _rectangle.upperBoundary.y } );
            m_shape.polygon = polygon;
            m_shapeType = ShapeType::Polygon;
        }

        Chain getAsChain()          const { assert( m_shapeType == ShapeType::Chain ); return m_shape.chain; }
        Circle getAsCircle()        const { assert( m_shapeType == ShapeType::Circle ); return m_shape.circle; }
        Polygon getAsPolygon()      const { assert( m_shapeType == ShapeType::Polygon ); return m_shape.polygon; }

    private:

        void copyFromShape( const Shape& _other )
        {
            m_shapeType = _other.m_shapeType;
            switch ( m_shapeType )
            {
            case ShapeType::Chain: {m_shape.chain = _other.getAsChain(); break; }
            case ShapeType::Circle: {m_shape.circle = _other.getAsCircle(); break; }
            case ShapeType::Polygon: {m_shape.polygon = _other.getAsPolygon(); break; }
            default: assert( false ); //Shape not supported
            }
        }

        union InternalShape
        {
            InternalShape() {}
            ~InternalShape() {}
            Chain chain;
            Circle circle = {};
            Polygon polygon;
        }m_shape;



        ShapeType m_shapeType = ShapeType::Circle;
    };
    
}