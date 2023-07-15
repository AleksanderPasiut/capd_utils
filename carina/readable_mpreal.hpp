///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <leo/readable_memory.hpp>

#include "capd/basic_types.hpp"

#ifdef __HAVE_MPFR__

namespace Carina
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Auxiliary class to access multiprecision representation inside MpReal
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MpRealOpen : public MpReal
{
public:
	MpRealOpen(int x, MpReal::PrecisionType precision) : MpReal(0, MpReal::RoundNearest, precision)
	{}

	MpRealOpen(const MpReal& arg) : MpReal(arg)
	{}

	mpfr_t & get_rep()
	{
		return mpfr_rep;
	}

	const mpfr_t & get_rep() const
	{
		return mpfr_rep;
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! @brief Utility for parsing and printing of multiprecision real variable
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ReadableMpReal
{
public:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @brief Parse multiprecision real from hex string
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static MpReal parse_string(const std::string& arg)
	{
		std::stringstream ss(arg);

		mpfr_prec_t precision {};
		Leo::ReadableMemory::parse_hex(ss, &precision, sizeof(precision));

		MpRealOpen ret(0, precision);
		mpfr_t & mpfr_rep = ret.get_rep();

		Leo::ReadableMemory::parse_hex(ss, &(mpfr_rep->_mpfr_sign), sizeof(mpfr_rep->_mpfr_sign));
		Leo::ReadableMemory::parse_hex(ss, &(mpfr_rep->_mpfr_exp), sizeof(mpfr_rep->_mpfr_exp));

		const size_t length = prec_to_limb_count(mpfr_rep->_mpfr_prec);
		Leo::ReadableMemory::parse_hex(ss, mpfr_rep->_mpfr_d, sizeof(mp_limb_t) * length);

		return ret;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @brief Print multiprecision real into hex string
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static std::string get_string(const MpReal& arg)
	{
		MpRealOpen open(arg);
		const mpfr_t & mpfr_rep = open.get_rep();

		std::stringstream ss {};

		Leo::ReadableMemory::print_hex(ss, &(mpfr_rep->_mpfr_prec), sizeof(mpfr_rep->_mpfr_prec));
		Leo::ReadableMemory::print_hex(ss, &(mpfr_rep->_mpfr_sign), sizeof(mpfr_rep->_mpfr_sign));
		Leo::ReadableMemory::print_hex(ss, &(mpfr_rep->_mpfr_exp), sizeof(mpfr_rep->_mpfr_exp));

		const size_t length = prec_to_limb_count(mpfr_rep->_mpfr_prec);
		Leo::ReadableMemory::print_hex(ss, mpfr_rep->_mpfr_d, sizeof(mp_limb_t) * length);

		return ss.str();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//! @brief Print multiprecision real into latex string
	//! @todo  Currently, only multiprecision real variables with precision being multiple of limb size are supported!
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static std::string get_latex_string(const MpReal& arg)
	{
		MpRealOpen open(arg);
		const mpfr_t & mpfr_rep = open.get_rep();

		mpz_t mantissa {};
		mpz_init2(mantissa, mpfr_rep->_mpfr_prec);

		assert(mpfr_rep->_mpfr_prec % (8*sizeof(mp_limb_t)) == 0); // precision size check

		mantissa->_mp_size = prec_to_limb_count(mpfr_rep->_mpfr_prec);
		assert(mantissa->_mp_size <= mantissa->_mp_alloc);

		std::memcpy(mantissa->_mp_d, mpfr_rep->_mpfr_d, mantissa->_mp_alloc * sizeof(mp_limb_t));

		char * str = mpz_get_str(NULL, 10, mantissa);

		std::string ret {};
		ret += mpfr_rep->_mpfr_sign > 0 ? "+" : "-";
		ret += str;
		ret += "\\cdot 2^{";
		ret += std::to_string(mpfr_rep->_mpfr_exp - mpfr_rep->_mpfr_prec);
		ret += "}";
		
		free(str);

		mpz_clear(mantissa);

		return ret;
	}

private:
	static size_t prec_to_limb_count(MpReal::PrecisionType prec)
	{
		size_t bits_per_limb = 8 * sizeof(mp_limb_t);
		size_t length = prec / bits_per_limb + (prec % bits_per_limb != 0);
		return length;
	}
};

}

#endif
