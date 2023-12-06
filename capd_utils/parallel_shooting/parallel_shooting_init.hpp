///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <capd_utils/concat.hpp>
#include <capd_utils/map_compatibility.hpp>
#include <capd_utils/map_base.hpp>

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Build initial vector for parallel shooting evaluation
//!
//! @details Return concatenated vector ( x_1, x_2, ... x_n) where x_k = f_k ( x_{k-1} )
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU, typename... MapV>
class ParallelShootingInit
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ParallelShootingInit(MapU map_1, MapV... map_2_args) : m_map_1(map_1), m_map_2(map_2_args...)
    {}

    VectorType operator() (const VectorType& first_vec)
    {
        MapBase<MapT>::assert_vector_size(first_vec, m_map_1.dimension(), "ParallelShootingInit first_vec vector dimension mismatch!");
        
        const VectorType second_vec = mid_vector( m_map_1(first_vec) );
        const VectorType second_ret = mid_vector( m_map_2(second_vec) );
        return Concat<MapT>::concat_vectors({ first_vec, second_ret });
    }

private:
    MapU m_map_1;
    ParallelShootingInit<MapT, MapV...> m_map_2;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Build initial vector for parallel shooting evaluation (1 map specialization)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class ParallelShootingInit<MapT, MapU>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ParallelShootingInit(MapU)
    {}

    VectorType operator() (const VectorType& first_vec)
    {
        return first_vec;
    }
};


}
