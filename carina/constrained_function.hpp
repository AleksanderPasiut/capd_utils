///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <carina/map_base.hpp>
#include <carina/map_compatibility.hpp>
#include <carina/concat.hpp>
#include <carina/extract.hpp>
#include <carina/composite_map.hpp>
#include <carina/extension_map.hpp>
#include <carina/projection_map.hpp>
#include <carina/newton_method/newton_method.hpp>

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Constrained function evaluation
//!
//! @details Implementation of function `h` such that:
//!          h( x_1, ..., x_n ) = f( x_1, ..., x_{n+k} ), where g( x_1, ..., x_{n+k} ) = 0
//!          where f : R^{n+1} \rightarrow R^m and g : R^{n+1} \rightarrow R^k.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU, typename MapV>
class ConstrainedFunction : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    static_assert(MapCompatibility<MapT, MapU>::value);
    static_assert(MapCompatibility<MapT, MapV>::value);

    ConstrainedFunction(MapU f, MapV g, VectorType initial_value, size_t newton_steps = 20)
        : m_f(f)
        , m_g(g)
        , m_initial_value(initial_value)
        , m_newton_steps(newton_steps)
    {
        if (m_f.dimension() < 2)
        {
            throw std::logic_error("Dimension of function f must be greater or equal 2!");
        }

        if (m_g.dimension() != m_f.dimension())
        {
            throw std::logic_error("Dimension of function f must be equal dimension of function g!");
        }

        if (m_g.imageDimension() >= m_g.dimension())
        {
            throw std::logic_error("Dimension of image of function g must be less than dimension of function g!");
        }

        if (m_initial_value.dimension() != m_g.imageDimension())
        {
            throw std::logic_error("Dimension of image of function g must be equal dimension of initial_value vector!");
        }
    }

    VectorType operator() (const VectorType& vec) override
    {
        const VectorType arg_vector = get_arg_vector(vec);

        return m_f(arg_vector);
    }

    VectorType operator() (const VectorType& vec, MatrixType& der) override
    {
        const VectorType arg_vector = get_arg_vector(vec);

        der = MatrixType( this->imageDimension(), this->dimension() );

        MatrixType f_der( m_f.imageDimension(), m_f.dimension() );
        const VectorType ret = m_f(arg_vector, f_der);

        MatrixType g_der( m_g.imageDimension(), m_g.dimension() );
        m_g(arg_vector, g_der);

        const unsigned n = m_f.dimension() - m_g.imageDimension();
        const unsigned k = m_g.imageDimension();
        const unsigned m = m_f.imageDimension();

        const MatrixType df_dx = Extract<MapT>::get_matrix(f_der, 0, 0, m, n );
        const MatrixType df_du = Extract<MapT>::get_matrix(f_der, 0, n, m, k );
        const MatrixType dg_dx = Extract<MapT>::get_matrix(g_der, 0, 0, k, n );
        const MatrixType dg_du = Extract<MapT>::get_matrix(g_der, 0, n, k, k );

        const MatrixType dg_du_inv = gaussInverseMatrix<MapT>(dg_du);

        der = df_dx - df_du * dg_du_inv * dg_dx;
        return ret;
    }

    unsigned dimension() const override
    {
        return m_f.dimension() - m_g.imageDimension();
    }

    unsigned imageDimension() const override
    {
        return m_f.imageDimension();
    }

private:
    VectorType get_arg_vector(const VectorType& vec)
    {
        this->assert_vector_size(vec, this->dimension(), "Constrained function vec vector size mismatch!");

        const VectorType extension_vector { Concat<MapT>::concat_vectors({ vec, m_initial_value }) };
        MapT extend { ExtensionMap<MapT>::create(extension_vector, m_extension_idx_list) };

        CompositeMap<MapT, MapT&, MapV&> objective
        {
            std::ref(extend),
            std::ref(m_g)
        };

        NewtonMethod find_and_bound( objective, m_initial_value, m_newton_steps );
        if (find_and_bound.is_successful())
        {
            return Concat<MapT>::concat_vectors({ vec, find_and_bound.get_root() });
        }
        else
        {
            throw std::logic_error("Failed to bound the solution!");
        }
    }

    MapU m_f;
    MapV m_g;

    const IdxList<int> m_extension_idx_list
    {
        IdxList<int>::create(m_f.dimension(), m_f.dimension()-m_g.imageDimension(), m_g.imageDimension())
    };

    const VectorType m_initial_value;
    size_t m_newton_steps;
};

}
