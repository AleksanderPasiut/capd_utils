///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iomanip>
#include <ostream>

namespace Carina
{

class ReadableMemory
{
public:
	static std::ostream& print_hex(std::ostream& ostr, const void* ptr, std::size_t len)
	{
		while (len > 0)
		{
			const uint16_t value = static_cast<const uint8_t*>(ptr)[--len];
			ostr << to_char((value >> 4) & 0xf);
			ostr << to_char(value & 0xf);
		}

		return ostr;
	}

	static std::istream& parse_hex(std::istream& istr, void* ptr, std::size_t len)
	{
		while (len > 0)
		{
			uint8_t value {};
			char ch {};

			istr >> ch;
			value = (from_char(ch) << 4);

			istr >> ch;
			value |= from_char(ch);

			static_cast<uint8_t*>(ptr)[--len] = value;
		}

		return istr;
	}

    template<typename T>
    static std::string print_str(const T& arg)
    {
        std::stringstream ss {};
        print_hex(ss, &arg, sizeof(T));
        return ss.str();
    }

    template<typename T>
    static T parse_str(const std::string& arg)
    {
        std::stringstream ss {};
        ss.str(arg);

        T ret {};
        parse_hex(ss, &ret, sizeof(T));
        return ret;
    }

private:
    static char to_char(uint8_t u)
    {
        static const std::array<char, 16> lookup = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
        };

        return lookup[u];
    }

    static uint8_t from_char(char ch)
    {
        switch (ch)
        {
            case '0': return 0;
            case '1': return 1;
            case '2': return 2;
            case '3': return 3;
            case '4': return 4;
            case '5': return 5;
            case '6': return 6;
            case '7': return 7;
            case '8': return 8;
            case '9': return 9;
            case 'a': return 10;
            case 'b': return 11;
            case 'c': return 12;
            case 'd': return 13;
            case 'e': return 14;
            case 'f': return 15;
            case 'A': return 10;
            case 'B': return 11;
            case 'C': return 12;
            case 'D': return 13;
            case 'E': return 14;
            case 'F': return 15;
            default: throw std::invalid_argument("Unexpected character!");
        }
    }
};

}
