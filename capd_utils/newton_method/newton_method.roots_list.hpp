///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include <algorithm>
#include <stdexcept>

#include "newton_method.root.hpp"

namespace CapdUtils
{

template<typename MapT>
struct RootsList : std::list<Root<MapT>>
{
    using VectorType = typename MapT::VectorType;

    bool is_present(const Root<MapT>& root) const
    {
        return std::find(this->begin(), this->end(), root) != this->end();
    }

    VectorType best_argument() const
    {
        if (this->size() > 0)
        {
            auto it = this->begin();

            Root<MapT> best_root = *it;

            for (++it; it != this->end(); ++it)
            {
                if (it->value_norm < best_root.value_norm)
                {
                    best_root = *it;
                }
            }

            return best_root.argument;
        }
        else
        {
            throw std::logic_error("Root list is empty!");
        }
    }
};

}
