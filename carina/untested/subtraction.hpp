///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <carina/map_base.hpp>
#include <carina/map_compatibility.hpp>
#include <stdexcept>

namespace Carina
{

template<typename MapT, typename MapU, typename MapV>
class Subtraction : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    static_assert(MapCompatibility<MapT, MapU>::value);
    static_assert(MapCompatibility<MapT, MapV>::value);

    Subtraction(MapU map1, MapV map2) : m_map1(map1), m_map2(map2)
    {
        if (map1.dimension() != map2.dimension())
        {
            throw std::logic_error("Map dimensions mismatch!");
        }

        if (map1.imageDimension() != map2.imageDimension())
        {
            throw std::logic_error("Map image dimensions mismatch!");
        }
    }

    VectorType operator() (const VectorType& vec) override
    {
        return m_map1(vec) - m_map2(vec);
    }

    VectorType operator() (const VectorType& vec, MatrixType& der) override
    {
        MatrixType m1(m_map1.imageDimension(), m_map1.dimension());
        const VectorType v1 = m_map1(vec, m1);

        MatrixType m2(m_map2.imageDimension(), m_map2.dimension());
        const VectorType v2 = m_map2(vec, m2);

        der = m1 - m2;
        return v1 - v2;
    }

    unsigned dimension() const override
    {
        return m_map1.dimension();
    }

    unsigned imageDimension() const override
    {
        return m_map1.imageDimension();
    }

private:
    MapU m_map1;
    MapV m_map2;
};

}
