///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "eigenproblem.hpp"
#include <capd_utils/newton_method/newton_method.multisearcher.hpp>

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Heuristic solver for eigenvalue problem
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class EigenproblemSearcher
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    EigenproblemSearcher(const MatrixType& arg) : m_problem(arg)
    {}

    template<typename RngT>
    std::list<Eigenpair<MapT>> find_eigenpairs(
        size_t max_attempts,
        size_t steps_per_attempt,
        ScalarType threshold,
        RngT rng)
    {
        const size_t dimension = m_problem.dimension()-1;

        auto check_eigenvector_similarity = [threshold](const VectorType& lhs, const VectorType& rhs) -> bool
        {
            if (lhs.dimension() == rhs.dimension())
            {
                if (lhs.dimension() > 1)
                {
                    ScalarType prod {};

                    for (int i = 0; i < lhs.dimension()-1; ++i)
                    {
                        prod += lhs[i] * rhs[i];
                    }

                    return std::abs(prod) > 1.0 - threshold;
                }
                else
                {
                    throw std::logic_error("Unexpected vector size!");
                }
            }
            else
            {
                throw std::logic_error("Mismatch of dimensions of compared vectors!");
            }
        };

        NewtonMethodMultisearcher m_internal_multisearcher(m_problem);
        const std::list<VectorType> roots = m_internal_multisearcher.find_roots(
            max_attempts,
            steps_per_attempt, 
            dimension,
            check_eigenvector_similarity,
            rng);

        std::list<Eigenpair<MapT>> ret {};
        
        for (const VectorType& v : roots)
        {
            ret.emplace_back( Eigenpair<MapT>::create(v) );
        }

        return ret;
    }

    Eigenpair<MapT> find_single_eigenpair(size_t steps, const Eigenpair<MapT>& initial_eigenpair)
    {
        const VectorType& x = initial_eigenpair.get_vector();
        const ScalarType& lambda = initial_eigenpair.get_lambda();

        VectorType initial_root(x.dimension() + 1);
        
        for (int i = 0; i < x.dimension(); ++i)
        {
            initial_root[i] = x[i];
        }
        initial_root[ x.dimension() ] = lambda;

        NewtonMethod<decltype(m_problem)> m_internal_searcher(m_problem, initial_root, steps);
        const VectorType ret = m_internal_searcher.get_root();
        return Eigenpair<MapT>::create( ret );
    }

private:
    Eigenproblem<MapT> m_problem;
};

}

