///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "map_base.hpp"
#include "map_compatibility.hpp"

#include "concat.hpp"

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Image sum implementation
//!
//! @details F(x) = [ f_1(x), ..., f_n(x) ]
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU, typename... MapV>
class ImageSum : public MapBase<MapT>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ImageSum(MapU map_1, MapV... map_2_args) : m_map_1(map_1), m_map_2(map_2_args...)
    {
        if (m_map_1.dimension() != m_map_2.dimension())
        {
            throw std::logic_error("ImageSum dimension mismatch!");
        }
    }

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, this->dimension(), "ImageSum vec vector size mismatch (1)!");

        const VectorType v1 = m_map_1(vec);
        const VectorType v2 = m_map_2(vec);

        const VectorType ret = Concat<MapT>::concat_vectors({ v1, v2 });
        return ret;
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, this->dimension(), "ImageSum vec vector size mismatch (2)!");

        MatrixType m1(m_map_1.imageDimension(), m_map_1.dimension());
        const VectorType v1 = m_map_1(vec, m1);
        this->assert_matrix_size(m1, m_map_1.imageDimension(), m_map_1.dimension(), "ImageSum m1 matrix size mismatch!");

        MatrixType m2(m_map_2.imageDimension(), m_map_2.dimension());
        const VectorType v2 = m_map_2(vec, m2);
        this->assert_matrix_size(m2, m_map_2.imageDimension(), m_map_2.dimension(), "ImageSum m2 matrix size mismatch!");

        const VectorType ret = Concat<MapT>::concat_vectors({ v1, v2 });

        mat = MatrixType(this->imageDimension(), this->dimension());
        Concat<MapT>::copy_matrix_on_matrix(mat, m1, 0, 0);
        Concat<MapT>::copy_matrix_on_matrix(mat, m2, m_map_1.imageDimension(), 0);
        return ret;
    }

    unsigned dimension() const noexcept override
    {
        return m_map_1.dimension();
    }

    unsigned imageDimension() const noexcept override
    {
        return m_map_1.imageDimension() + m_map_2.imageDimension();
    }

private:
    MapU m_map_1;
    ImageSum<MapT, MapV...> m_map_2;
};


template<typename MapT, typename MapU>
class ImageSum<MapT, MapU> : public MapBase<MapT>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ImageSum(MapU map) : m_map(map)
    {}

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, m_map.dimension(), "ImageSum vec vector size mismatch (1)!");
        return m_map(vec);
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, m_map.dimension(), "ImageSum vec vector size mismatch (2)!");

        const VectorType ret = m_map(vec, mat);
        this->assert_matrix_size(mat, m_map.imageDimension(), m_map.dimension(), "ImageSum mat matrix size mismatch!");

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
