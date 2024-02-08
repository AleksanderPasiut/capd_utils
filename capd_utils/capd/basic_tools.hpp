///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "basic_tools.internal.hpp"

namespace CapdUtils
{

template<typename ScalarType>
inline ScalarType middle(const ScalarType& arg)
{
    BasicTools::MiddleInternal<ScalarType, capd::TypeTraits<ScalarType>::isInterval> internal {};
    return internal(arg);
}

template<typename VectorType>
inline VectorType mid_vector(const VectorType& arg)
{
    using ScalarType = typename VectorType::ScalarType;

    BasicTools::MiddleInternal<ScalarType, capd::TypeTraits<ScalarType>::isInterval> internal {};

    VectorType ret(arg.dimension());
    for (unsigned i = 0; i < arg.dimension(); ++i)
    {
        ret[i] = internal( arg[i] );
    }

    return ret;
}

template<typename MatrixType>
inline MatrixType mid_matrix(const MatrixType& arg)
{
    using ScalarType = typename MatrixType::ScalarType;

    BasicTools::MiddleInternal<ScalarType, capd::TypeTraits<ScalarType>::isInterval> internal {};

    MatrixType ret(arg.numberOfRows(), arg.numberOfColumns());
    for (unsigned i = 1; i <= arg.numberOfRows(); ++i)
    {
        for (unsigned j = 1; j <= arg.numberOfColumns(); ++j)
        {
            ret(i,j) = internal( arg(i,j) );
        }
    }

    return ret;
}

template<typename ScalarType>
inline auto span(const ScalarType& arg)
{
    BasicTools::SpanInternal<ScalarType, capd::TypeTraits<ScalarType>::isInterval> internal {};
    return internal(arg);
}

template<typename VectorType>
inline auto span_vector(const VectorType& arg)
{
    using ScalarType = typename VectorType::ScalarType;
    using ReturnType = typename BasicTools::BoundType<ScalarType>::VectorType;

    BasicTools::SpanInternal<ScalarType, capd::TypeTraits<ScalarType>::isInterval> internal {};

    ReturnType ret(arg.dimension());
    for (unsigned i = 0; i < arg.dimension(); ++i)
    {
        ret[i] = internal( arg[i] );
    }

    return ret;
}

template<typename MatrixType>
inline auto span_matrix(const MatrixType& arg)
{
    using ScalarType = typename MatrixType::ScalarType;
    using ReturnType = typename BasicTools::BoundType<ScalarType>::MatrixType;

    BasicTools::SpanInternal<ScalarType, capd::TypeTraits<ScalarType>::isInterval> internal {};

    ReturnType ret(arg.numberOfRows(), arg.numberOfColumns());
    for (unsigned i = 1; i <= arg.numberOfRows(); ++i)
    {
        for (unsigned j = 1; j <= arg.numberOfColumns(); ++j)
        {
            ret(i,j) = internal( arg(i,j) );
        }
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Expand given interval by given number of steps
//!
//! Each step moves right bound into smallest greater representable real number
//! and left bound into largest lesser representable real number.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ScalarType>
inline ScalarType expand(const ScalarType& arg, unsigned steps = 1)
{
    static_assert(capd::TypeTraits<ScalarType>::isInterval, "Expanding is implemented for intervals only!");

    using BoundType = typename ScalarType::BoundType;

    BoundType right = arg.rightBound();
    BoundType left = arg.leftBound();

    for (unsigned i = 0; i < steps; ++i)
    {
        right = std::nextafter(right, INFINITY);
        left = std::nextafter(left, -INFINITY);
    }

    return ScalarType(left, right);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Expand given VectorType by given number of steps
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename VectorType>
inline VectorType expand_vector(const VectorType& arg, unsigned steps = 1)
{
    using ScalarType = typename VectorType::ScalarType;
    
    VectorType ret( arg.dimension() );
    for (unsigned i = 0; i < arg.dimension(); ++i)
    {
        ret[i] = expand<ScalarType>( arg[i], steps );
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Expand given IMatrix by given number of steps
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MatrixType>
inline MatrixType expand_matrix(const MatrixType& arg, unsigned steps = 1)
{
    using ScalarType = typename MatrixType::ScalarType;

    MatrixType ret(arg.numberOfRows(), arg.numberOfColumns());
    for (unsigned i = 1; i <= arg.numberOfRows(); ++i)
    {
        for (unsigned j = 1; j <= arg.numberOfColumns(); ++j)
        {
            ret(i,j) = expand<ScalarType>( arg(i,j), steps );
        }
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Outer product of vectors
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MatrixType, typename VectorType>
inline MatrixType outer_product(const VectorType& a, const VectorType& b)
{
    MatrixType ret( a.dimension(), b.dimension() );

    for (unsigned i = 1; i <= a.dimension(); ++i)
    {
        for (unsigned j = 1; j <= b.dimension(); ++j)
        {
            ret(i,j) = a(i) * b(j);
        }
    }

    return ret;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Absolute maximum inclusion factor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ScalarType>
inline typename ScalarType::BoundType amif(const ScalarType& internal, const ScalarType external)
{
    static_assert(capd::TypeTraits<ScalarType>::isInterval, "AMIF is implemented for intervals only!");

    using BoundType = typename ScalarType::BoundType;

    if (internal.getBaseInterval().disjoint(external.getBaseInterval()) == false)
    {
        const BoundType right_aif = external.rightBound() - internal.rightBound();
        const BoundType left_aif = internal.leftBound() - external.leftBound();
        return std::max<BoundType>(right_aif, left_aif);
    }
    else
    {
        return BoundType(INFINITY);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Absolute maximum inclusion factor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename VectorType>
inline typename VectorType::ScalarType::BoundType amif_vector(const VectorType& internal, const VectorType& external)
{
    using ScalarType = typename VectorType::ScalarType;
    using BoundType = typename ScalarType::BoundType;

    BoundType ret = BoundType(0.0);
    for (unsigned i = 0; i < internal.dimension(); ++i)
    {
        const BoundType b = amif(internal[i], external[i]);
        ret = std::max<BoundType>( ret, b );
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Absolute maximum inclusion factor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MatrixType>
inline typename MatrixType::ScalarType::BoundType amif_matrix(const MatrixType& internal, const MatrixType& external)
{
    using ScalarType = typename MatrixType::ScalarType;
    using BoundType = typename ScalarType::BoundType;

    BoundType ret = BoundType(0.0);
    for (unsigned i = 1; i <= internal.numberOfRows(); ++i)
    {
        for (unsigned j = 1; j <= internal.numberOfColumns(); ++j)
        {
            const BoundType b = amif(internal(i,j), external(i,j));
            ret = std::max<BoundType>( ret, b );
        }
    }

    return ret;
}


}
