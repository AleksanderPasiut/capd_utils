///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cmath>

#include "basic_types.hpp"

namespace CapdUtils
{
namespace BasicTools
{

template<typename T, bool is_interval>
struct BoundTypeInternal
{};

template<typename T>
struct BoundTypeInternal<T, true>
{
    using Type = typename T::BoundType;
};

template<typename T>
struct BoundTypeInternal<T, false>
{
    using Type = T;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Get bound type of given type T (or T if T isn't of interval type)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct BoundType
{
    using ScalarType = typename BoundTypeInternal<T, capd::TypeTraits<T>::isInterval>::Type;
    using VectorType = capd::vectalg::Vector<ScalarType, 0>;
    using MatrixType = capd::vectalg::Matrix<ScalarType, 0, 0>;
};

template<typename T, bool is_interval>
struct MiddleInternal
{};

template<typename T>
struct MiddleInternal<T, true>
{
    T operator() (const T& value) const
    {
        return value.mid().leftBound();
    }
};

template<typename T>
struct MiddleInternal<T, false>
{
    T operator() (const T& value) const
    {
        return value;
    }
};

template<typename T, bool is_interval>
struct SpanInternal
{};

template<typename T>
struct SpanInternal<T, true>
{
    using BoundType = typename BoundType<T>::ScalarType;

    BoundType operator() (const T& value) const
    {
        return value.rightBound() - value.leftBound();
    }
};

template<typename T>
struct SpanInternal<T, false>
{
    using BoundType = typename BoundType<T>::ScalarType;

    BoundType operator() (const T& value) const
    {
        return BoundType(0.0);
    }
};

}
}
