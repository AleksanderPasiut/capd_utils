///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "identity_map.hpp"
#include "map_compatibility.hpp"

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief F(x) = f(x) - x = 0
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class FixedPointCondition
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    template<typename... MapUArgs>
    FixedPointCondition(MapUArgs... map_u_args)
        : m_map_u(map_u_args...)
        , m_id(m_map_u.dimension())
    {
        assert(m_map_u.dimension() == m_map_u.imageDimension());
    }

    VectorType operator() (const VectorType& vec)
    {
        return m_map_u(vec) - m_id(vec);
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat)
    {
        const size_t N = m_map_u.dimension();

        MatrixType m2(N, N);
        const VectorType v2 = m_id(vec, m2);

        MatrixType m1(N, N);
        const VectorType v1 = m_map_u(vec, m1);

        mat = m1 - m2;
        return v1 - v2;
    }

    int dimension() const noexcept
	{
		return m_map_u.dimension();
	}

	int imageDimension() const noexcept
	{
		return m_map_u.dimension();
	}

private:
    MapU m_map_u;
    IdentityMap<MapT> m_id;
};

}
