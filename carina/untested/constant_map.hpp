///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <carina/map_base.hpp>
#include <stdexcept>

namespace Carina
{

template<typename MapT>
class Constant : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    Constant(VectorType value) : m_value(value)
    {}

    VectorType operator() (const VectorType&) override
    {
        return m_value;
    }

    VectorType operator() (const VectorType&, MatrixType& der) override
    {
        der = MatrixType{};
        return m_value;
    }

    unsigned dimension() const override
    {
        return 0;
    }

    unsigned imageDimension() const override
    {
        return m_value.dimension();
    }

private:
    VectorType m_value;
};

}
