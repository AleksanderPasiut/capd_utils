///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <carina/map_base.hpp>
#include <carina/map_compatibility.hpp>
#include <stdexcept>

namespace Carina
{

template<typename MapT, typename MapU>
class ShiftMap : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    static_assert(MapCompatibility<MapT, MapU>::value);

    ShiftMap(MapU map, VectorType shift) : m_map(map), m_shift(shift)
    {
        if (map.imageDimension() != shift.dimension())
        {
            throw std::logic_error("Map image dimension and shift dimension mismatch!");
        }
    }

    VectorType operator() (const VectorType& vec) override
    {
        return m_map(vec) - m_shift;
    }

    VectorType operator() (const VectorType& vec, MatrixType& der) override
    {
        return m_map(vec, der) - m_shift;
    }

    unsigned dimension() const override
    {
        return m_map.dimension();
    }

    unsigned imageDimension() const override
    {
        return m_map.imageDimension();
    }

    void set_shift(const VectorType& shift)
    {
        m_shift = shift;
    }

    const VectorType& get_shift() const
    {
        return m_shift;
    }

private:
    MapU m_map;
    VectorType m_shift;
};

}
