///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdexcept>

#include "number_string_info.hpp"
#include "number_string_utilities.hpp"

namespace Leo
{

NumberStringInfo::NumberStringInfo(const std::string& str)
{
    if (is_string_integer(str))
    {
        fractional_digits = 0;
        dot = false;
        if (str[0] == '-')
        {
            minus = true;
            integer_digits = str.size() - 1;
        }
        else
        {
            minus = false;
            integer_digits = str.size();
        }
    }
    else if (is_string_float(str))
    {
        dot = true;
        size_t const dot_index = str.find_first_of('.');
        fractional_digits = str.size() - dot_index - 1;

        if (str[0] == '-')
        {
            minus = true;
            integer_digits = dot_index - 1;
        }
        else
        {
            minus = false;
            integer_digits = dot_index;
        }
    }
    else
    {
        throw std::invalid_argument("String `" + str + "` is not number representation!");
    }
}

NumberStringInfo::NumberStringInfo(const NumberStringInfo& lhs, const NumberStringInfo& rhs)
{
    integer_digits = std::max(lhs.integer_digits, rhs.integer_digits);
    fractional_digits = std::max(lhs.fractional_digits, rhs.fractional_digits);
    dot = true;
    minus = lhs.minus || rhs.minus;
}

size_t NumberStringInfo::get_length() const
{
    return integer_digits + fractional_digits + (dot ? 1 : 0);
}

std::string NumberStringInfo::print(const std::string& str, const NumberStringInfo& info) const
{
    std::string ret;
    ret.reserve( get_length() );

    for (size_t i = integer_digits - info.integer_digits; i > 0; --i)
    {
        ret.push_back('0');
    }

    if (info.minus)
    {
        ret.append(str.begin() + 1, str.end());
    }
    else
    {
        ret.append(str.begin(), str.end());
    }

    for (size_t i = fractional_digits - info.fractional_digits; i > 0; --i)
    {
        ret.push_back('0');
    }

    return ret;
}

}
