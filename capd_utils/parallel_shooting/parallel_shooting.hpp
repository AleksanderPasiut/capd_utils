///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "single_shooting.hpp"

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief General implementation of parallel shooting map
//!
//! @details F(x_1, ..., x_n) = f_1 (x_1) - x_2
//!                             f_2 (x_2) - x_2
//!                             f_3 (x_3) - x_4
//!                              ...
//!                             f_n (x_n)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU, typename... MapV>
class ParallelShooting : public MapBase<MapT>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ParallelShooting(MapU map_1, MapV... map_2_args) : m_map_1(map_1), m_map_2(map_2_args...)
    {}

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, this->dimension(), "ParallelShooting vec vector size mismatch (1)!");

        const VectorType arg1 = Extract<MapT>::get_vector(vec, 0, m_map_1.dimension());
        const VectorType arg2 = Extract<MapT>::get_vector(vec, m_map_1.internalMapDimension(), m_map_2.dimension());

        const VectorType v1 = m_map_1(arg1);
        const VectorType v2 = m_map_2(arg2);

        const VectorType ret = Concat<MapT>::concat_vectors({ v1, v2 });
        return ret;
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, this->dimension(), "ParallelShooting vec vector size mismatch (2)!");

        const VectorType arg1 = Extract<MapT>::get_vector(vec, 0, m_map_1.dimension());
        const VectorType arg2 = Extract<MapT>::get_vector(vec, m_map_1.internalMapDimension(), m_map_2.dimension());

        MatrixType m1(m_map_1.imageDimension(), m_map_1.dimension());
        const VectorType v1 = m_map_1(arg1, m1);
        this->assert_matrix_size(m1, m_map_1.imageDimension(), m_map_1.dimension(), "ParallelShooting m1 matrix size mismatch!");

        MatrixType m2(m_map_2.imageDimension(), m_map_2.dimension());
        const VectorType v2 = m_map_2(arg2, m2);
        this->assert_matrix_size(m2, m_map_2.imageDimension(), m_map_2.dimension(), "ParallelShooting m2 matrix size mismatch!");

        const VectorType ret = Concat<MapT>::concat_vectors({ v1, v2 });

        mat = MatrixType(this->imageDimension(), this->dimension());
        Concat<MapT>::copy_matrix_on_matrix(mat, m1, 0, 0);
        Concat<MapT>::copy_matrix_on_matrix(mat, m2, m_map_1.imageDimension(), m_map_1.internalMapDimension());
        return ret;
    }

    unsigned dimension() const noexcept override
    {
        return m_map_1.internalMapDimension() + m_map_2.dimension();
    }

    unsigned imageDimension() const noexcept override
    {
        return m_map_1.imageDimension() + m_map_2.imageDimension();
    }

private:
    SingleShooting<MapT, MapU> m_map_1;
    ParallelShooting<MapT, MapV...> m_map_2;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief General implementation of parallel shooting map (1 map specialization)
//!
//! @details F(x_1) = f_1 (x_1)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class ParallelShooting<MapT, MapU> : public MapBase<MapT>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ParallelShooting(MapU map) : m_map(map)
    {}

    VectorType operator() (const VectorType& vec) override
    {
        return m_map(vec);
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        return m_map(vec, mat);
    }

    unsigned dimension() const noexcept override
    {
        return m_map.dimension();
    }

    unsigned imageDimension() const noexcept override
    {
        return m_map.imageDimension();
    }

private:
    MapU m_map;
};

}
