#pragma once

#include <utils/geometry/shapes/chain.h>
#include <utils/geometry/shapes/cirlce.h>
#include <utils/geometry/shapes/polygon.h>
#include <utils/geometry/shapes/rectangle.h>

namespace puma
{
    enum class ShapeType
    {
        Chain,
        Circle,
        Rectangle,
        Polygon,
    };

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
        void setAsRectangle ( const Rectangle& _rectangle )    { m_shape.rectangle = _rectangle; m_shapeType = ShapeType::Rectangle; }

        Chain getAsChain()          const { assert( m_shapeType == ShapeType::Chain ); return m_shape.chain; }
        Circle getAsCircle()        const { assert( m_shapeType == ShapeType::Circle ); return m_shape.circle; }
        Polygon getAsPolygon()      const { assert( m_shapeType == ShapeType::Polygon ); return m_shape.polygon; }
        Rectangle getAsRectangle()  const { assert( m_shapeType == ShapeType::Rectangle ); return m_shape.rectangle; }

    private:

        void copyFromShape( const Shape& _other )
        {
            m_shapeType = _other.m_shapeType;
            switch ( m_shapeType )
            {
            case ShapeType::Chain: {m_shape.chain = _other.getAsChain(); break; }
            case ShapeType::Circle: {m_shape.circle = _other.getAsCircle(); break; }
            case ShapeType::Polygon: {m_shape.polygon = _other.getAsPolygon(); break; }
            case ShapeType::Rectangle: {m_shape.rectangle = _other.getAsRectangle(); break; }
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
            Rectangle rectangle;
        }m_shape;



        ShapeType m_shapeType = ShapeType::Circle;
    };
    
}