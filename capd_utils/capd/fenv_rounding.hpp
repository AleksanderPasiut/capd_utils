///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <capd/rounding/DoubleRounding.h>

#include <fenv.h>
#pragma STDC FENV_ACCESS ON

namespace capd
{
namespace rounding
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Implementation of capd::rounding::FenvRounding class which is equivalent
//! of capd::rounding::DoubleRounding class, but which enables configuration
//! of rounding policy for long double type (capd::rounding::DoubleRounding
//! does not work for long double calculations in AMD64 architecture).
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct FenvRounding
{
    static void roundNearest()
    {
        fesetround(FE_TONEAREST);
    }

    static void roundUp()
    {
        fesetround(FE_UPWARD);
    }

    static void roundDown()
    {
        fesetround(FE_DOWNWARD);
    }

    static void roundCut()
    {
        fesetround(FE_TOWARDZERO);
    }

    static RoundingMode test()
    {
        switch (fegetround())
        {
            case FE_TONEAREST: return RoundingMode::RoundNearest;
            case FE_UPWARD: return RoundingMode::RoundUp;
            case FE_DOWNWARD: return RoundingMode::RoundDown;
            case FE_TOWARDZERO: return RoundingMode::RoundCut;
            default: return RoundingMode::RoundUnknown;
        }
    }

    static bool isWorking()
    {
        roundUp();
        if (test() != RoundingMode::RoundUp)
        {
            return false;
        }

        roundDown();
        if (test() != RoundingMode::RoundDown)
        {
            return false;
        }

        roundNearest();
        if (test() != RoundingMode::RoundNearest)
        {
            return false;
        }

        roundCut();
        if (test() != RoundingMode::RoundCut)
        {
            return false;
        }

        roundUp();
        if (test() != RoundingMode::RoundUp)
        {
            return false;
        }

        return true;
    }
};

}
}
