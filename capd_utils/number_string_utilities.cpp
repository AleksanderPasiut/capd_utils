///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <regex>

#include "number_string_utilities.hpp"
#include "number_string_info.hpp"

namespace Carina
{
    
bool is_string_integer(const std::string& str)
{
    return std::regex_match(str, std::regex( "^-?[0-9]+$" ));
}

bool is_string_float(const std::string& str)
{
    return std::regex_match(str, std::regex( "^-?[0-9]+\\.[0-9]*$" ));
}

bool compare_values(const std::string& lhs, const std::string& rhs)
{
    const NumberStringInfo lhs_info(lhs);
    const NumberStringInfo rhs_info(rhs);

    if (lhs_info.minus && !rhs_info.minus)
    {
        return true; // (-) <= (+)
    }

    if (!lhs_info.minus && rhs_info.minus)
    {
        return false; // (+) <= (-)
    }

    const NumberStringInfo res_info(lhs_info, rhs_info);

    const std::string lhs_tmp = res_info.print(lhs, lhs_info);
    const std::string rhs_tmp = res_info.print(rhs, rhs_info);

    if (lhs_info.minus)
    {
        return lhs_tmp >= rhs_tmp;
    }
    else
    {
        return lhs_tmp <= rhs_tmp;
    }
}

}
