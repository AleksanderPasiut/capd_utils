///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "map_base.hpp"
#include "map_compatibility.hpp"
#include "local_coordinate_system.hpp"
#include "gauss.hpp"

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Local map implementation (base)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class LocalMapBase : public MapBase<MapT>
{
public:
    static_assert(MapCompatibility<MapT, MapU>::value);

    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        mat = get_derivative(vec);

        const MatrixType multiplier = get_multiplier(vec);
        return multiplier * vec + m_shift;
    }

    VectorType operator() (const VectorType& vec) override
    {
        const MatrixType multiplier = get_multiplier(vec);
        return multiplier * vec + m_shift;
    }

    unsigned dimension() const noexcept override
    {
        return m_map.dimension();
    }

    unsigned imageDimension() const noexcept override
    {
        return m_map.imageDimension();
    }

protected:
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Constructor
    //!
    //! @param map         underlying global map
    //! @param source      local coordinate system around source point
    //! @param destination local coordinate system around destination point
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LocalMapBase(
        MapU& map,
        const LocalCoordinateSystem<MapT>& source,
        const LocalCoordinateSystem<MapT>& destination)
            : m_map(map)
            , m_source(source)
            , m_destination(destination)
            , m_shift( this->compute_shift() )
    {
        if (map.dimension() != source.get_origin().dimension())
        {
            throw std::logic_error("Source coordinate dimension mismatch!");
        }

        if (map.imageDimension() != destination.get_origin().dimension())
        {
            throw std::logic_error("Destination coordinate dimension mismatch!");
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Return solution of A*X = B with respect to X
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual MatrixType solveMatrix(const MatrixType& A, const MatrixType& B) = 0;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Return interval hull of A and B
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    virtual VectorType intervalHull(const VectorType& A, const VectorType& B) = 0;

private:
    MatrixType get_multiplier(const VectorType& vec)
    {
        const MatrixType& a1 = m_source.get_directions_matrix();
        const VectorType vec_in_origin = a1 * vec + m_source.get_origin();
        const VectorType vec_extended = intervalHull(m_source.get_origin(), vec_in_origin);

        MatrixType der(this->m_map.imageDimension(), this->m_map.dimension());
        m_map(vec_extended, der);

        const MatrixType a2 = m_destination.get_directions_matrix();
        const MatrixType ret = solveMatrix(a2, der * a1);
        return ret;
    }

    MatrixType get_derivative(const VectorType& vec)
    {
        const MatrixType& a1 = m_source.get_directions_matrix();
        const VectorType vec_in_origin = a1 * vec + m_source.get_origin();

        MatrixType der(this->m_map.imageDimension(), this->m_map.dimension());
        m_map(vec_in_origin, der);

        const MatrixType a2 = m_destination.get_directions_matrix();
        const MatrixType ret = solveMatrix(a2, der * a1);
        return ret;
    }

    VectorType compute_shift()
    {
        const VectorType fbb = m_map(m_source.get_origin()) - m_destination.get_origin();

        return gauss<MapT>(m_destination.get_directions_matrix(), fbb);
    }

    MapU& m_map;

    const LocalCoordinateSystem<MapT> m_source;
    const LocalCoordinateSystem<MapT> m_destination;

    const VectorType m_shift;
};

}
