///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdexcept>

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Eigenpair (eigenvalue and eigenvector)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class Eigenpair
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    Eigenpair(const VectorType& vector, const ScalarType& lambda) : m_vector(vector), m_lambda(lambda)
    {}

    bool operator< (const Eigenpair<MapT>& arg) const noexcept
    {
        return m_lambda < arg.m_lambda;
    }

    const VectorType& get_vector() const noexcept
    {
        return m_vector;
    }
    
    const ScalarType& get_lambda() const noexcept
    {
        return m_lambda;
    }

    static Eigenpair<MapT> create(const VectorType& arg)
    {
        if (arg.dimension() > 2)
        {
            VectorType x(arg.dimension()-1);
            for (unsigned i = 0; i < arg.dimension()-1; ++i)
            {
                x[i] = arg[i];
            }

            ScalarType lambda = arg[arg.dimension()-1];
            return Eigenpair<MapT>(x, lambda);
        }
        else
        {
            throw std::logic_error("Eigenpair<MapT>::create unexpected arg dimension!");
        }
    }

private:
    VectorType m_vector;
    ScalarType m_lambda;
};

}
