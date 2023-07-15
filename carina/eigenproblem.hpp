///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "capd/map.hpp"
#include "concat.hpp"
#include "eigenpair.hpp"

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Implement map:
//!                f(x,lambda) = (A-lambda*I)*x, x*x
//! where A is given parameter.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class Eigenproblem
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    Eigenproblem(const MatrixType& arg) : m_n(get_dimension(arg)), m_arg(arg)
    {}

    VectorType operator() (const VectorType& vec)
    {
        const Eigenpair<MapT> pair = Eigenpair<MapT>::create(vec);
        const VectorType& x = pair.get_vector();
        const ScalarType& lambda = pair.get_lambda();

        const ScalarType vec_norm_sqr = capd::vectalg::scalarProduct(x, x);
        const VectorType eigen_part = m_arg * x - lambda * x;

        return Concat<MapT>::concat_vectors({ eigen_part, VectorType{ vec_norm_sqr - 1.0 } });
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat)
    {
        const Eigenpair<MapT> pair = Eigenpair<MapT>::create(vec);
        const VectorType& x = pair.get_vector();
        const ScalarType& lambda = pair.get_lambda();

        const ScalarType vec_norm_sqr = capd::vectalg::scalarProduct(x, x);

        mat = MatrixType(m_n+1, m_n+1);

        const MatrixType der1 = m_arg - MatrixType::Identity(m_n) * lambda;
        const VectorType eigen_part = der1 * x;

        for (unsigned i = 1; i <= m_n; ++i)
        {
            for (unsigned j = 1; j <= m_n; ++j)
            {
                mat(i, j) = der1(i, j);
            }

            mat(i, m_n+1) = -x(i);
            mat(m_n+1, i) = 2 * x(i);
        }

        return Concat<MapT>::concat_vectors({ eigen_part, VectorType{ vec_norm_sqr - 1.0 } });
    }

    unsigned dimension() const noexcept
    {
        return m_n + 1;
    }

    unsigned imageDimension() const noexcept
    {
        return m_n + 1;
    }

    static MatrixType create_eigenvalues_matrix(const std::list<Eigenpair<MapT>>& eigenpairs)
    {
        MatrixType ret( eigenpairs.size(), eigenpairs.size(), ScalarType(0.0) );

        auto it = eigenpairs.begin();   
        for (int i = 1; i <= eigenpairs.size(); ++i, ++it)
        {
            ret(i, i) = it->get_lambda();
        }

        return ret;
    }

    static MatrixType create_similarity_matrix(const std::list<Eigenpair<MapT>>& eigenpairs)
    {
        using EigenpairListIt = typename std::list<Eigenpair<MapT>>::const_iterator;

        struct It : public EigenpairListIt
        {
            It(const EigenpairListIt& it) : EigenpairListIt(it)
            {}

            const VectorType* operator-> ()
            {
                const EigenpairListIt& internal = *this;
                return &internal->get_vector();
            }

            const VectorType& operator* ()
            {
                const EigenpairListIt& internal = *this;
                return internal->get_vector();
            }
        };

        return Concat<MapT>::build_matrix_from_vvectors( It(eigenpairs.begin()), It(eigenpairs.end()) );
    }

private:
    static unsigned get_dimension(const MatrixType& arg)
    {
        if (arg.dimension().first != arg.dimension().second)
        {
            throw std::logic_error("Eigenproblem is implemented for square matrices only!");
        }

        return arg.dimension().first;
    }

    struct Arg
    {
        VectorType x;
        ScalarType lambda;
        ScalarType x_norm_squared;

        Arg(const VectorType& vec)
            : x( vec.dimension()-1 )
            , lambda()
            , x_norm_squared()
        {
            for (unsigned i = 0; i < vec.dimension()-1; ++i)
            {
                x[i] = vec[i];
                x_norm_squared += x[i] * x[i];
            }

            lambda = vec[vec.dimension()-1];
        }
    };

    const unsigned m_n;
    MatrixType m_arg;
};

}
