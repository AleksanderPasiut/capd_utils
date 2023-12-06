///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "local_map.base.hpp"
#include "gauss.hpp"

namespace Carina
{

template<typename MapT, bool is_interval>
class IntervalHull_LocalMapInternal
{};

template<typename MapT>
class IntervalHull_LocalMapInternal<MapT, true>
{
public:
    using VectorType = typename MapT::VectorType;

    static VectorType get(const VectorType& A, const VectorType& B)
    {
        return capd::vectalg::intervalHull(A, B);
    }
};

template<typename MapT>
class IntervalHull_LocalMapInternal<MapT, false>
{
public:
    using VectorType = typename MapT::VectorType;

    static VectorType get(const VectorType& A, const VectorType& B)
    {
        return B;
    }
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Local map implementation
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class LocalMap : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    static_assert(MapCompatibility<MapT, MapU>::value);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Constructor
    //!
    //! @param map         underlying global map
    //! @param source      local coordinate system around source point
    //! @param destination local coordinate system around destination point
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LocalMap(
        MapU& map,
        const LocalCoordinateSystem<MapT>& source,
        const LocalCoordinateSystem<MapT>& destination)
            : m_map(map)
            , m_source(source)
            , m_destination(destination)
            , m_dest_directions_inverse( gaussInverseMatrix<MapT>(m_destination.get_directions_matrix()) )
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

private:
    MatrixType get_multiplier(const VectorType& vec)
    {
        const MatrixType& a1 = m_source.get_directions_matrix();
        const VectorType vec_in_origin = a1 * vec + m_source.get_origin();
        const VectorType vec_extended = IntervalHull::get(m_source.get_origin(), vec_in_origin);

        MatrixType der(this->m_map.imageDimension(), this->m_map.dimension());
        m_map(vec_extended, der);

        const MatrixType ret = m_dest_directions_inverse * (der * a1);
        return ret;
    }

    MatrixType get_derivative(const VectorType& vec)
    {
        const MatrixType& a1 = m_source.get_directions_matrix();
        const VectorType vec_in_origin = a1 * vec + m_source.get_origin();

        MatrixType der(this->m_map.imageDimension(), this->m_map.dimension());
        m_map(vec_in_origin, der);

        const MatrixType ret = m_dest_directions_inverse * (der * a1);
        return ret;
    }

    VectorType compute_shift()
    {
        const VectorType fbb = m_map(m_source.get_origin()) - m_destination.get_origin();
        return m_dest_directions_inverse * fbb;
    }

    MapU m_map;

    const LocalCoordinateSystem<MapT> m_source;
    const LocalCoordinateSystem<MapT> m_destination;

    const MatrixType m_dest_directions_inverse;
    const VectorType m_shift;

	static constexpr bool is_interval = capd::TypeTraits<ScalarType>::isInterval;
    using IntervalHull = IntervalHull_LocalMapInternal<MapT, is_interval>;
};




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Local map implementation
//! @details This implementation uses regular matrix inverse.
//!          This implementation supports interval computation only!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class LocalMapRmi : public LocalMapBase<MapT, MapU>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    static_assert(capd::TypeTraits<ScalarType>::isInterval);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Constructor
    //!
    //! @param map         underlying global map
    //! @param source      local coordinate system around source point
    //! @param destination local coordinate system around destination point
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LocalMapRmi(
        MapU& map,
        const LocalCoordinateSystem<MapT>& source,
        const LocalCoordinateSystem<MapT>& destination) : LocalMapBase<MapT, MapU>(map, source, destination)
    {}

private:
    virtual VectorType intervalHull(const VectorType& A, const VectorType& B) override final
    {
        return capd::vectalg::intervalHull(A, B);
    }

    virtual MatrixType solveMatrix(const MatrixType& A, const MatrixType& B) override final
    {
        return gaussInverseMatrix<MapT>(A) * B;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Local map implementation
//! @details This implementation uses matrix solver with intersect.
//!          This implementation supports interval computation only!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class LocalMapMii : public LocalMapBase<MapT, MapU>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    static_assert(capd::TypeTraits<ScalarType>::isInterval);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Constructor
    //!
    //! @param map         underlying global map
    //! @param source      local coordinate system around source point
    //! @param destination local coordinate system around destination point
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LocalMapMii(
        MapU& map,
        const LocalCoordinateSystem<MapT>& source,
        const LocalCoordinateSystem<MapT>& destination) : LocalMapBase<MapT, MapU>(map, source, destination)
    {}

private:
    virtual VectorType intervalHull(const VectorType& A, const VectorType& B) override final
    {
        return capd::vectalg::intervalHull(A, B);
    }

    virtual MatrixType solveMatrix(const MatrixType& A, const MatrixType& B) override final
    {
        return gaussMatrixSolverWithIntersect<MapT>(A, B);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Local map implementation
//! @details This implementation uses regular matrix inverse.
//!          This implementation is not correct for interval computation!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename MapU>
class LocalMapReal : public LocalMapBase<MapT, MapU>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    static_assert(capd::TypeTraits<ScalarType>::isInterval == false);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Constructor
    //!
    //! @param map         underlying global map
    //! @param source      local coordinate system around source point
    //! @param destination local coordinate system around destination point
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LocalMapReal(
        MapU& map,
        const LocalCoordinateSystem<MapT>& source,
        const LocalCoordinateSystem<MapT>& destination) : LocalMapBase<MapT, MapU>(map, source, destination)
    {}

private:
    virtual VectorType intervalHull(const VectorType&, const VectorType& B) override final
    {
        return B;
    }

    virtual MatrixType solveMatrix(const MatrixType& A, const MatrixType& B) override final
    {
        return gaussInverseMatrix<MapT>(A) * B;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Local map implementation
//! @details This implementation is obsolete!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class LocalMapObsolete : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Constructor
    //!
    //! @param map           underlying global map
    //! @param origin        origin point in global coordinates
    //! @param origin_image  origin point image in global coordinates
    //! @param basis         matrix with basis vectors at origin point
    //! @param basis_image   matrix with basis vectors at origin point image
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LocalMapObsolete(
        MapT& map,
        const VectorType& origin,
        const VectorType& origin_image,
        const MatrixType& basis,
        const MatrixType& basis_image)
            : m_map(map)
            , m_origin(origin)
            , m_basis(basis)
            , m_origin_image(origin_image)
            , m_basis_image(basis_image)
    {
        this->assert_vector_size(origin, m_map.dimension(), "LocalMapObsolete origin vector size mismatch!");
        this->assert_matrix_size(basis, m_map.dimension(), m_map.dimension(), "LocalMapObsolete basis matrix size mismatch!");

        this->assert_vector_size(origin_image, m_map.imageDimension(), "LocalMapObsolete origin_image size mismatch!");
        this->assert_matrix_size(basis_image, m_map.imageDimension(), m_map.imageDimension(), "LocalMapObsolete basis_image matrix size mismatch!");

        m_b_inv = gaussInverseMatrix<MapT>(m_basis_image);
    }

    VectorType operator() (const VectorType& vec, MatrixType& mat) override
    {
        this->calc_internal(vec, mat);

        // std::cout << "bu: " << ( this->m_map(m_origin) - m_origin_image ) << '\n';

        // const VectorType err = this->m_b_inv * ( this->m_map(m_origin) - m_origin_image );
        return mat * vec;// + err;
    }

    VectorType operator() (const VectorType& vec) override
    {
        MatrixType dummy( m_map.imageDimension(), m_map.dimension() );
        return (*this)(vec, dummy);
    }

    unsigned dimension() const noexcept override
    {
        return m_map.dimension();
    }

    unsigned imageDimension() const noexcept override
    {
        return m_map.imageDimension();
    }

private:
    VectorType calc_internal(const VectorType& vec, MatrixType& mat)
    {
        this->assert_vector_size(vec, this->m_map.dimension(), "LocalMapObsolete vec vector size mismatch!");

        const VectorType v = this->m_basis * vec + this->m_origin;

        MatrixType der(this->m_map.imageDimension(), this->m_map.dimension());
        const VectorType ret = this->m_map(v, der);
        this->assert_matrix_size(der, this->m_map.imageDimension(), this->m_map.dimension(), "LocalMapObsolete der matrix size mismatch!");

        mat = this->m_b_inv * der * this->m_basis;
        return ret;
    }

    MapT& m_map;

    VectorType m_origin;
    MatrixType m_basis;

    VectorType m_origin_image;
    MatrixType m_basis_image;

    MatrixType m_b_inv;
};

}
