///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>

namespace Leo
{

struct NumberStringInfo
{
    size_t integer_digits;
    size_t fractional_digits;
    bool dot;
    bool minus;

    NumberStringInfo(const std::string& str);
    NumberStringInfo(const NumberStringInfo& lhs, const NumberStringInfo& rhs);

    std::string print(const std::string& str, const NumberStringInfo& info) const;

private:
    size_t get_length() const;
};

}
