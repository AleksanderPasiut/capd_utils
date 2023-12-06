///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "readable_memory.hpp"

namespace CapdUtils
{

template<typename ScalarType>
class ReadableScalar
{
public:
    ReadableScalar(const char * str) : m_buffer(str)
    {}

    ReadableScalar(ScalarType x) : m_buffer( ReadableMemory::print_str(x) )
    {}

    operator std::string() const
    {
        return m_buffer;        
    }

    operator ScalarType() const
    {
        return ReadableMemory::parse_str<ScalarType>(m_buffer);
    }

private:
    std::string m_buffer;
};

}
