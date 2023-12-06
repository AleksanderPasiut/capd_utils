///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "map_base.hpp"
#include "map_compatibility.hpp"

namespace CapdUtils
{

template<typename MapT, typename MapU, typename... MapV>
class CompositeMap : public MapBase<MapT>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    CompositeMap(MapU map_1, MapV... map_2_args) : m_map_1(map_1), m_map_2(map_2_args...)
    {
        if (map_1.imageDimension() != m_map_2.dimension())
        {
            throw std::logic_error("Composite map dimensions mismatch!");
        }
    }

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, m_map_1.dimension(), "CompositeMap vec vector size mismatch (1)!");

        const VectorType v1 = m_map_1(vec);
        const VectorType v2 = m_map_2(v1);
        return v2;
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, m_map_1.dimension(), "CompositeMap vec vector size mismatch (2)!");

        MatrixType m1(m_map_1.imageDimension(), m_map_1.dimension());
        const VectorType v1 = m_map_1(vec, m1);
        this->assert_matrix_size(m1, m_map_1.imageDimension(), m_map_1.dimension(), "CompositeMap m1 matrix size mismatch!");

        MatrixType m2(m_map_2.imageDimension(), m_map_2.dimension());
        const VectorType v2 = m_map_2(v1, m2);
        this->assert_matrix_size(m2, m_map_2.imageDimension(), m_map_2.dimension(), "CompositeMap m2 matrix size mismatch!");

        mat = m2 * m1;
        return v2;
    }

    unsigned dimension() const noexcept override
    {
        return m_map_1.dimension();
    }

    unsigned imageDimension() const noexcept override
    {
        return m_map_2.imageDimension();
    }

private:
    MapU m_map_1;
    CompositeMap<MapT, MapV...> m_map_2;
};


template<typename MapT, typename MapU>
class CompositeMap<MapT, MapU> : public MapBase<MapT>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    CompositeMap(MapU map) : m_map(map)
    {}

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, m_map.dimension(), "CompositeMap vec vector size mismatch (1)!");
        return m_map(vec);
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->assert_vector_size(vec, m_map.dimension(), "CompositeMap vec vector size mismatch (2)!");

        const VectorType ret = m_map(vec, mat);
        this->assert_matrix_size(mat, m_map.imageDimension(), m_map.dimension(), "CompositeMap mat matrix size mismatch!");

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
