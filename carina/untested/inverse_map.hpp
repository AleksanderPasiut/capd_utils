///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <carina/newton_method/newton_method.hpp>

#include "shift_map.hpp"

namespace Carina
{

template<typename MapT, typename MapU>
class InverseMap : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    InverseMap(MapU map) : m_shift(map, VectorType(map.imageDimension()))
    {
        if (map.dimension() != map.imageDimension())
        {
            throw std::logic_error("Map dimension is not equal image dimension!");
        }
    }

    VectorType operator() (const VectorType&) override
    {
        Carina::NewtonMethod<typename MapT>
        return m_value;
    }

    VectorType operator() (const VectorType&, MatrixType& der) override
    {
        der = MatrixType{};
        return m_value;
    }

    unsigned dimension() const override
    {
        return m_map.imageDimension();
    }

    unsigned imageDimension() const override
    {
        return m_map.dimension();
    }

private:
    ShiftMap<MapT, MapU> m_shift {};
};

}
