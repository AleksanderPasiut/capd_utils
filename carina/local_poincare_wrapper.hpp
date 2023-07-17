///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "capd/ode_solver.hpp"
#include "capd/section.hpp"
#include "capd/poincare_map.hpp"
#include "capd/c0rect2set.hpp"
#include "capd/c1rect2set.hpp"

#include "map_base.hpp"
#include "local_coordinate_system.hpp"
#include "gauss.hpp"

namespace Carina
{

template<typename MapT, typename SectionT, bool flow_der, bool is_interval>
class LocalPoincareWrapperInternal
{};

template<typename MapT, typename SectionT, bool flow_der>
class LocalPoincareWrapperInternal<MapT, SectionT, flow_der, false>
{
public:
	using ScalarType = typename MapT::ScalarType;
	using VectorType = typename MapT::VectorType;
	using MatrixType = typename MapT::MatrixType;

	LocalPoincareWrapperInternal(
        PoincareMap<MapT, SectionT>& poincare_map,
        const LocalCoordinateSystem<MapT>& origin, 
        const LocalCoordinateSystem<MapT>& image)
            : m_poincare_map(poincare_map)
            , m_origin(origin)
            , m_image(image)
            , m_image_inv( gaussInverseMatrix<MapT>( m_image.get_directions_matrix() ) )
	{}

	VectorType operator() (const VectorType& vec)
	{
        m_return_time = 0.0;

        const VectorType vec_global = m_origin.get_origin() + m_origin.get_directions_matrix() * vec;
        const VectorType img = m_poincare_map(vec_global, m_return_time);
        const VectorType ret = m_image_inv * (img - m_image.get_origin());
        return ret;
	}

    VectorType operator() (const VectorType& vec, MatrixType& der)
	{
        const unsigned dimension = m_origin.get_origin().dimension();
        der = MatrixType(dimension, dimension);

        const VectorType vec_global = m_origin.get_origin() + m_origin.get_directions_matrix() * vec;

        m_return_time = 0.0;
        const VectorType img = m_poincare_map(vec_global, der, m_return_time);
        const VectorType ret = m_image_inv * (img - m_image.get_origin());

        if constexpr (!flow_der)
        {
            der = m_poincare_map.computeDP(img, der);
        }

        der = m_image_inv * der * m_origin.get_directions_matrix();
        return ret;
	}

    ScalarType get_last_evaluation_return_time() const
    {
        return m_return_time;
    }

private:
	PoincareMap<MapT, SectionT>& m_poincare_map;

    const LocalCoordinateSystem<MapT>& m_origin;
    const LocalCoordinateSystem<MapT>& m_image;
    const MatrixType m_image_inv;

    ScalarType m_return_time {};
};

template<typename MapT, typename SectionT, bool flow_der>
class LocalPoincareWrapperInternal<MapT, SectionT, flow_der, true>
{
public:
	using ScalarType = typename MapT::ScalarType;
	using VectorType = typename MapT::VectorType;
	using MatrixType = typename MapT::MatrixType;

	LocalPoincareWrapperInternal(
        PoincareMap<MapT, SectionT>& poincare_map,
        const LocalCoordinateSystem<MapT>& origin, 
        const LocalCoordinateSystem<MapT>& image)
            : m_poincare_map(poincare_map)
            , m_origin(origin)
            , m_image(image)
            , m_image_inv( gaussInverseMatrix<MapT>( m_image.get_directions_matrix() ) )
	{}

	VectorType operator() (const VectorType& vec)
	{
		C0Rect2Set<MapT> set(m_origin.get_origin(), m_origin.get_directions_matrix(), vec);
        return m_poincare_map(set, m_image.get_origin(), m_image_inv, m_return_time);
	}

    VectorType operator() (const VectorType& vec, MatrixType& der)
	{
        const unsigned dimension = m_origin.get_origin().dimension();
        der = MatrixType(dimension, dimension);

        C0Rect2Set<MapT> set(m_origin.get_origin(), m_origin.get_directions_matrix(), vec);
        const VectorType ret = m_poincare_map(set, m_image.get_origin(), m_image_inv, m_return_time);

        C1Rect2Set<MapT> set1(m_origin.get_origin(), m_origin.get_directions_matrix(), vec);
        const VectorType img = m_poincare_map(set1, der);

        if constexpr (!flow_der)
        {
            der = m_poincare_map.computeDP(img, der);
        }

        der = m_image_inv * der * m_origin.get_directions_matrix();
        return ret;
	}

    ScalarType get_last_evaluation_return_time() const
    {
        return m_return_time;
    }

private:
	PoincareMap<MapT, SectionT>& m_poincare_map;

    const LocalCoordinateSystem<MapT>& m_origin;
    const LocalCoordinateSystem<MapT>& m_image;
    const MatrixType m_image_inv;

    ScalarType m_return_time {};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Poincare map implementation (specialized for computation in local coordinate systems)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT, typename SectionT, bool flow_der = false>
class LocalPoincareWrapper : MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Constructor
    //!
    //! @param vector_field vector field to be integrated
    //! @param solver_order order of the underlying ode solver
    //! @param section      poincare section
    //! @param origin       local coordinate system in which the arugments are expressed
    //! @param image        local coordinate system in which the images are expressed
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    LocalPoincareWrapper(
        MapT& vector_field,
        unsigned solver_order,
        SectionT section,
        const LocalCoordinateSystem<MapT>& origin, 
        const LocalCoordinateSystem<MapT>& image)
            : m_solver(vector_field, solver_order)
            , m_section(section)
            , m_poincare_map(m_solver, m_section)
            , m_origin(origin)
            , m_image(image)
            , m_poincare_internal(m_poincare_map, m_origin, m_image)
    {}

    VectorType operator() (const VectorType& vec) override
    {
        this->assert_vector_size(vec, dimension(), "LocalPoincareWrapper vec vector size mismatch!");

        return m_poincare_internal(vec);
    }

    VectorType operator() (const VectorType& vec, MatrixType& der) override
    {
        this->assert_vector_size(vec, dimension(), "LocalPoincareWrapper vec vector size mismatch!");

        return m_poincare_internal(vec, der);
    }

    unsigned dimension() const override
    {
        return m_origin.get_origin().dimension();
    }

    unsigned imageDimension() const override
    {
        return m_origin.get_origin().dimension();
    }

    ScalarType get_last_evaluation_return_time() const
    {
        return m_poincare_internal.get_last_evaluation_return_time();
    }

private:
	OdeSolver<MapT> m_solver;
    SectionT m_section;
    PoincareMap<MapT, SectionT> m_poincare_map;

    const LocalCoordinateSystem<MapT> m_origin;
    const LocalCoordinateSystem<MapT> m_image;

    static constexpr bool is_interval = capd::TypeTraits<ScalarType>::isInterval;
	LocalPoincareWrapperInternal<MapT, SectionT, flow_der, is_interval> m_poincare_internal;
};

}
