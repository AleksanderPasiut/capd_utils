///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "capd/ode_solver.hpp"
#include "capd/poincare_map.hpp"
#include "capd/section.hpp"

#include "capd/c0rect2set.hpp"
#include "capd/c1rect2set.hpp"

#include "map_base.hpp"

namespace Carina
{

template<typename MapT, typename SectionT, bool flow_der, bool is_interval>
class PoincareWrapperInternal
{};

template<typename MapT, typename SectionT, bool flow_der>
class PoincareWrapperInternal<MapT, SectionT, flow_der, false>
{
public:
	using ScalarType = typename MapT::ScalarType;
	using VectorType = typename MapT::VectorType;
	using MatrixType = typename MapT::MatrixType;

	PoincareWrapperInternal(PoincareMap<MapT, SectionT>& poincare_map, unsigned dimension) : m_poincare_map(poincare_map), m_dimension(dimension)
	{}

	VectorType operator() (const VectorType& vec)
    {
        m_return_time = 0.0;
        return m_poincare_map(vec, m_return_time);
    }

    VectorType operator() (const VectorType& vec, MatrixType& der)
    {
        m_return_time = 0.0;
        der = MatrixType(m_dimension, m_dimension);
        if constexpr (flow_der)
        {
            return m_poincare_map(vec, der, m_return_time);
        }
        else
        {
            const VectorType ret = m_poincare_map(vec, der, m_return_time);
            der = m_poincare_map.computeDP(ret, der);
            return ret;
        }
    }

    ScalarType get_last_evaluation_return_time() const
    {
        return m_return_time;
    }

private:
    PoincareMap<MapT, SectionT>& m_poincare_map;
    unsigned m_dimension;

    ScalarType m_return_time {};
};

template<typename MapT, typename SectionT, bool flow_der>
class PoincareWrapperInternal<MapT, SectionT, flow_der, true>
{
public:
	using ScalarType = typename MapT::ScalarType;
	using VectorType = typename MapT::VectorType;
	using MatrixType = typename MapT::MatrixType;

	PoincareWrapperInternal(PoincareMap<MapT, SectionT>& poincare_map, unsigned dimension) : m_poincare_map(poincare_map), m_dimension(dimension)
	{}

	VectorType operator() (const VectorType& vec)
    {
        C0Rect2Set<MapT> set(vec);
        return m_poincare_map(set, m_return_time);
    }

    VectorType operator() (const VectorType& vec, MatrixType& der)
    {
        C1Rect2Set<MapT> set(vec);
        der = MatrixType(m_dimension, m_dimension);
        if constexpr (flow_der)
        {
            return m_poincare_map(set, der, m_return_time);
        }
        else
        {
            const VectorType ret = m_poincare_map(set, der, m_return_time);
            der = m_poincare_map.computeDP(ret, der);
            return ret;
        }
    }

    ScalarType get_last_evaluation_return_time() const
    {
        return m_return_time;
    }
	
private:
    PoincareMap<MapT, SectionT>& m_poincare_map;
    unsigned m_dimension;

    ScalarType m_return_time {};
};

template<typename MapT, typename SectionT, bool flow_der = false>
class PoincareWrapper : public MapBase<MapT>
{
public:
    using ScalarType = typename MapT::ScalarType;
    using VectorType = typename MapT::VectorType;
    using MatrixType = typename MapT::MatrixType;

    static_assert(std::is_same<SectionT, CoordinateSection<MapT>>::value || std::is_same<SectionT, AffineSection<MapT>>::value);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //! @brief Constructor
    //!
    //! @param vector_field vector field to be integrated
    //! @param solver_order order of the underlying ode solver
    //! @param section      poincare section
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PoincareWrapper(MapT& vector_field, unsigned solver_order, SectionT section)
        : m_vector_field(vector_field)
        , m_section(section)
		, m_solver(vector_field, solver_order)
        , m_poincare_map(m_solver, this->m_section)
		, m_poincare_internal(m_poincare_map, vector_field.dimension())
    {}

	VectorType operator() (const VectorType& vec) override
    {
        return m_poincare_internal(vec);
    }

    VectorType operator() (const VectorType& vec, MatrixType& der) override
    {
		return m_poincare_internal(vec, der);
    }

    unsigned dimension() const override
	{
		return m_vector_field.dimension();
	}

	unsigned imageDimension() const override
	{
		return m_vector_field.imageDimension();
	}

    ScalarType get_last_evaluation_return_time() const
    {
        return m_poincare_internal.get_last_evaluation_return_time();
    }

protected:
	MapT& m_vector_field;
    SectionT m_section;

	OdeSolver<MapT> m_solver;
    PoincareMap<MapT, SectionT> m_poincare_map;

	static constexpr bool is_interval = capd::TypeTraits<ScalarType>::isInterval;
	PoincareWrapperInternal<MapT, SectionT, flow_der, is_interval> m_poincare_internal;
};

}

