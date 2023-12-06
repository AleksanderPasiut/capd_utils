///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <utility>
#include <stdexcept>

#include "type_cast.hpp"

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Local coordinate system
//!
//! Represent local coordinate system by storing origin vector and directions matrix
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class LocalCoordinateSystem
{
public:
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Default constructor
    //!
    //! Construct 0-dim. (invalid) local coordinate system.
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LocalCoordinateSystem() : m_origin(), m_dirs()
    {}

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Construct local coordinate with origin vector and directions matrix
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LocalCoordinateSystem(const VectorType& origin, const MatrixType& dirs)
    {
        const unsigned dimension = origin.dimension();

        if (dirs.dimension() == std::make_pair(dimension, dimension))
        {
            m_origin = origin;
            m_dirs = dirs;
        }
        else
        {
            throw std::invalid_argument("Incorrect directions matrix dimension!");
        }
    }

    const VectorType& get_origin() const noexcept
    {
        return m_origin;
    }

    const MatrixType& get_directions_matrix() const noexcept
    {
        return m_dirs;
    }

    template<typename MapU>
    static LocalCoordinateSystem<MapT> convert_from(const LocalCoordinateSystem<MapU>& arg)
    {
        return LocalCoordinateSystem<MapT>(
            vector_cast<VectorType>(arg.get_origin()),
            matrix_cast<MatrixType>(arg.get_directions_matrix())
        );
    }

private:
    VectorType m_origin;
    MatrixType m_dirs;
};

}
