///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "type_cast.hpp"

namespace CapdUtils
{

template<typename MapT, typename MapU>
class TypeAdapterMap
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    using ScalarTypeU = typename MapU::ScalarType;
    using VectorTypeU = typename MapU::VectorType;
    using MatrixTypeU = typename MapU::MatrixType;

    TypeAdapterMap(MapU& map_u) : m_map_u(map_u)
    {}

    VectorType operator() (const VectorType& arg)
    {
        const VectorTypeU vec = vector_cast<VectorTypeU>(arg);
        const VectorTypeU img = m_map_u(vec);
        return vector_cast<VectorTypeU>(img);
    }

    VectorType operator() (const VectorType& arg, MatrixType& der)
    {
        const VectorTypeU vec = vector_cast<VectorTypeU>(arg);

        MatrixTypeU mat( dimension(), imageDimension() );
        const VectorTypeU img = m_map_u(vec, mat);

        der = matrix_cast<MatrixType>(mat);
        return vector_cast<VectorType>(img);
    }

    unsigned dimension() const
    {
        return m_map_u.dimension();
    }

    unsigned imageDimension() const
    {
        return m_map_u.imageDimension();
    }

private:
    MapU& m_map_u;
};

}
