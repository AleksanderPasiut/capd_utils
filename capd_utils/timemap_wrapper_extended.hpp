///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <capd_utils/timemap_wrapper.hpp>
#include <capd_utils/extract.hpp>
#include <capd_utils/concat.hpp>

namespace CapdUtils
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Timemap implementation (extended)
//!
//! @details Calculation of a flow of a given vector field.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename MapT>
class TimemapWrapperExtended : public MapBase<MapT>
{
public:
	using ScalarType = typename MapT::ScalarType;
	using VectorType = typename MapT::VectorType;
	using MatrixType = typename MapT::MatrixType;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @brief Constructor
	//!
	//! @param vector_field vector field to be integrated
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TimemapWrapperExtended(MapT& vector_field, unsigned order = 20)
		: m_timemap_internal(vector_field, 0.0, order)
	{}

	VectorType operator() (const VectorType& vec) override
	{
		const VectorType arg = Extract<MapT>::get_vector(vec, 1, m_timemap_internal.dimension());
		const ScalarType time = vec[0];

		SolutionCurve<MapT> solution_curve( time.left() );
		m_timemap_internal.set_time( time.right() );
		m_timemap_internal(arg, solution_curve);

		return solution_curve(time);
	}

    VectorType operator() (const VectorType& vec, MatrixType& der) override
	{
		const VectorType arg = Extract<MapT>::get_vector(vec, 1, m_timemap_internal.dimension());
		const ScalarType time = vec[0];

		SolutionCurve<MapT> solution_curve( 0.0 );
		m_timemap_internal.set_time( time.right() );
		m_timemap_internal(arg, solution_curve);

		const VectorType time_derivative = solution_curve.timeDerivative(time);
		const MatrixType derivative = solution_curve.derivative(time);

		// std::cout << time_derivative << '\n';
		// std::cout << derivative << '\n';

		der = MatrixType(this->imageDimension(), this->dimension());
		Concat<MapT>::copy_matrix_on_matrix(der, derivative, 0, 1);

		for (unsigned i = 1; i <= time_derivative.dimension(); ++i)
		{
			der(i, 1) = time_derivative(i);
		}

		return solution_curve(time);
	}

	unsigned dimension() const override
	{
		return m_timemap_internal.dimension() + 1;
	}

	unsigned imageDimension() const override
	{
		return m_timemap_internal.imageDimension();
	}

private:
	TimemapWrapper<MapT> m_timemap_internal;
};

}
