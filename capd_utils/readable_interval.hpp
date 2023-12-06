///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "capd/basic_types.hpp"

#include "readable_scalar.hpp"

namespace CapdUtils
{

template<typename ScalarType>
class ReadableInterval
{
public:
    using BoundType = typename ScalarType::BoundType;

    ReadableInterval(const char* left_str, const char* right_str)
        : m_left(left_str)
        , m_right(right_str)
    {}

    ReadableInterval(const ScalarType& iv)
        : m_left(iv.leftBound())
        , m_right(iv.rightBound())
    {}

    std::string left_string() const
    {
        return std::string(m_left);
    }

    std::string right_string() const
    {
        return std::string(m_right);
    }

    operator ScalarType() const
    {
        return ScalarType( BoundType(m_left), BoundType(m_right) );
    }

private:
    ReadableScalar<BoundType> m_left;
    ReadableScalar<BoundType> m_right;
};

}
