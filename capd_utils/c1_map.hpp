///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <capd_utils/capd/basic_tools.hpp>
#include <capd_utils/map_base.hpp>

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief C1 Map implementation
//! @details Improve computation accuracy by application of the equation:
//!    f(X) = f(x0) + Df(X)(X-x0)
//!  where x0 belongs to X.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class C1_Map : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    C1_Map(MapU map) : m_map(map)
    {}

    VectorType operator() (const VectorType& vec) override
    {
        MatrixType dummy {};
        return (*this)(vec, dummy);
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        const VectorType x0 = mid_vector(vec);
        const VectorType f_x0 = m_map(x0);

        m_map(vec, mat);
        return f_x0 + mat * (vec - x0);
    }

    unsigned dimension() const override
    {
        return m_map.dimension();
    }

    unsigned imageDimension() const override
    {
        return m_map.imageDimension();
    }

private:
    MapU m_map;
};

}
