///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "map_base.hpp"
#include "map_compatibility.hpp"

#include "extract.hpp"
#include "concat.hpp"

namespace CapdUtils
{

template<typename MapT, typename MapU, typename... MapV>
class DirectSum : public MapBase<MapT>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    DirectSum(MapU map_1, MapV... map_2_args) : m_map_1(map_1), m_map_2(map_2_args...)
    {}

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, this->dimension(), "DirectSum vec vector size mismatch (1)!");

        const VectorType arg1 = Extract<MapT>::get_vector(vec, 0, m_map_1.dimension());
        const VectorType arg2 = Extract<MapT>::get_vector(vec, m_map_1.dimension(), m_map_2.dimension());

        const VectorType v1 = m_map_1(arg1);
        const VectorType v2 = m_map_2(arg2);

        const VectorType ret = Concat<MapT>::concat_vectors({ v1, v2 });
        return ret;
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, this->dimension(), "DirectSum vec vector size mismatch (2)!");

        const VectorType arg1 = Extract<MapT>::get_vector(vec, 0, m_map_1.dimension());
        const VectorType arg2 = Extract<MapT>::get_vector(vec, m_map_1.dimension(), m_map_2.dimension());

        MatrixType m1(m_map_1.imageDimension(), m_map_1.dimension());
        const VectorType v1 = m_map_1(arg1, m1);
        this->assert_matrix_size(m1, m_map_1.imageDimension(), m_map_1.dimension(), "DirectSum m1 matrix size mismatch!");

        MatrixType m2(m_map_2.imageDimension(), m_map_2.dimension());
        const VectorType v2 = m_map_2(arg2, m2);
        this->assert_matrix_size(m2, m_map_2.imageDimension(), m_map_2.dimension(), "DirectSum m2 matrix size mismatch!");

        const VectorType ret = Concat<MapT>::concat_vectors({ v1, v2 });

        mat = MatrixType(this->imageDimension(), this->dimension());
        Concat<MapT>::copy_matrix_on_matrix(mat, m1, 0, 0);
        Concat<MapT>::copy_matrix_on_matrix(mat, m2, m_map_1.imageDimension(), m_map_1.dimension());
        return ret;
    }

    unsigned dimension() const noexcept override
    {
        return m_map_1.dimension() + m_map_2.dimension();
    }

    unsigned imageDimension() const noexcept override
    {
        return m_map_1.imageDimension() + m_map_2.imageDimension();
    }

private:
    MapU m_map_1;
    DirectSum<MapT, MapV...> m_map_2;
};


template<typename MapT, typename MapU>
class DirectSum<MapT, MapU> : public MapBase<MapT>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    DirectSum(MapU map) : m_map(map)
    {}

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, m_map.dimension(), "DirectSum vec vector size mismatch (1)!");
        return m_map(vec);
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, m_map.dimension(), "DirectSum vec vector size mismatch (2)!");

        const VectorType ret = m_map(vec, mat);
        this->assert_matrix_size(mat, m_map.imageDimension(), m_map.dimension(), "DirectSum mat matrix size mismatch!");

        return ret;
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
