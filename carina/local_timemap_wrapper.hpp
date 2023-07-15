///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "capd/ode_solver.hpp"
#include "capd/timemap.hpp"
#include "capd/c0rect2set.hpp"
#include "capd/c1rect2set.hpp"

#include "map_base.hpp"
#include "local_coordinate_system.hpp"
#include "gauss.hpp"

namespace Carina
{

template<typename MapT, bool is_interval>
class LocalTimemapWrapperInternal
{};

template<typename MapT>
class LocalTimemapWrapperInternal<MapT, false>
{
public:
	using ScalarType = typename MapT::ScalarType;
	using VectorType = typename MapT::VectorType;
	using MatrixType = typename MapT::MatrixType;

	LocalTimemapWrapperInternal(
        Timemap<MapT>& timemap,
        const ScalarType& time,
        const LocalCoordinateSystem<MapT>& origin, 
        const LocalCoordinateSystem<MapT>& image)
            : m_timemap(timemap)
            , m_time(time)
            , m_origin(origin)
            , m_image(image)
	{}

	VectorType operator() (const VectorType& vec)
	{
        VectorType vec_global = m_origin.get_origin() + m_origin.get_directions_matrix() * vec;
        const MatrixType inv = gaussInverseMatrix<MapT>( m_image.get_directions_matrix() );

        const VectorType img = m_timemap(m_time, vec_global);
        const VectorType ret = inv * (img - m_image.get_origin());
        return ret;
	}

    VectorType operator() (const VectorType& vec, MatrixType& der)
	{
        VectorType vec_global = m_origin.get_origin() + m_origin.get_directions_matrix() * vec;
        const MatrixType inv = gaussInverseMatrix<MapT>( m_image.get_directions_matrix() );

        MatrixType global_der( m_origin.get_origin().dimension(), m_origin.get_origin().dimension() );
        const VectorType img = m_timemap(m_time, vec_global, global_der);
        const VectorType ret = inv * (img - m_image.get_origin());

        der = inv * global_der * m_origin.get_directions_matrix();
        return ret;
	}

private:
	Timemap<MapT>& m_timemap;
	const ScalarType& m_time;

    const LocalCoordinateSystem<MapT>& m_origin;
    const LocalCoordinateSystem<MapT>& m_image;
};

template<typename MapT>
class LocalTimemapWrapperInternal<MapT, true>
{
public:
	using ScalarType = typename MapT::ScalarType;
	using VectorType = typename MapT::VectorType;
	using MatrixType = typename MapT::MatrixType;

	LocalTimemapWrapperInternal(
        Timemap<MapT>& timemap,
        const ScalarType& time,
        const LocalCoordinateSystem<MapT>& origin, 
        const LocalCoordinateSystem<MapT>& image)
            : m_timemap(timemap)
            , m_time(time)
            , m_origin(origin)
            , m_image(image)
	{}

	VectorType operator() (const VectorType& vec)
	{
		C0Rect2Set<MapT> set(m_origin.get_origin(), m_origin.get_directions_matrix(), vec);

        const MatrixType inv = gaussInverseMatrix<MapT>( m_image.get_directions_matrix() );

        m_timemap(m_time, set);

        const VectorType ret = (inv * set.get_C()) * set.get_r0() + (inv * set.get_B()) * set.get_r() + inv * (set.get_x() - m_image.get_origin());
        return ret;
	}

    VectorType operator() (const VectorType& vec, MatrixType& der)
	{
        C1Rect2Set<MapT> set(m_origin.get_origin(), m_origin.get_directions_matrix(), vec);

        const MatrixType inv = gaussInverseMatrix<MapT>( m_image.get_directions_matrix() );

        MatrixType global_der( m_origin.get_origin().dimension(), m_origin.get_origin().dimension() );
        m_timemap(m_time, set, global_der);

        const VectorType ret = (inv * set.get_C()) * set.get_r0() + (inv * set.get_B()) * set.get_r() + inv * (set.get_x() - m_image.get_origin());

        der = inv * global_der * m_origin.get_directions_matrix();
        return ret;
	}

private:
	Timemap<MapT>& m_timemap;
	const ScalarType& m_time;

    const LocalCoordinateSystem<MapT>& m_origin;
    const LocalCoordinateSystem<MapT>& m_image;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Time map implementation (specialized for computation in local coordinate systems)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class LocalTimemapWrapper : Carina::MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Constructor
    //!
    //! @param vector_field vector field to be integrated
    //! @param solver_order order of the solver used for integration
    //! @param origin       local coordinate system in which the arugments are expressed
    //! @param image        local coordinate system in which the images are expressed
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LocalTimemapWrapper(
        MapT& vector_field,
        ScalarType time,
        unsigned solver_order,
        const LocalCoordinateSystem<MapT>& origin, 
        const LocalCoordinateSystem<MapT>& image)
            : m_solver(vector_field, solver_order)
            , m_timemap(m_solver)
            , m_time(time)
            , m_origin(origin)
            , m_image(image)
            , m_timemap_internal(m_timemap, m_time, m_origin, m_image)
    {}

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, dimension(), "LocalTimemapWrapper vec vector size mismatch!");
        return m_timemap_internal(vec);
    }

    VectorType operator() (const VectorType& vec, MatrixType& der) override
    {
        this->assert_vector_size(vec, dimension(), "LocalTimemapWrapper vec vector size mismatch!");
        return m_timemap_internal(vec, der);
    }

    unsigned dimension() const override
    {
        return m_origin.get_origin().dimension();
    }

    unsigned imageDimension() const override
    {
        return m_origin.get_origin().dimension();
    }

    void set_time(ScalarType time)
	{
		m_time = time;
	}

	ScalarType get_time() const noexcept
	{
		return m_time;
	}

private:
	OdeSolver<MapT> m_solver;
    Timemap<MapT> m_timemap;

    ScalarType m_time;

    const LocalCoordinateSystem<MapT> m_origin;
    const LocalCoordinateSystem<MapT> m_image;

    static constexpr bool is_interval = capd::TypeTraits<ScalarType>::isInterval;
	LocalTimemapWrapperInternal<MapT, is_interval> m_timemap_internal;
};

}
