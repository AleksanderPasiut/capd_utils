///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author: Aleksander M. Pasiut
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <string>

namespace CapdUtils
{

class ProgressLogger
{
public:
    class Updater
    {
    public:
        Updater(ProgressLogger& logger) : m_logger(logger)
        {
            m_logger.tick_begin();
        }

        ~Updater() noexcept
        {
            m_logger.tick_end();
        }

    private:
        ProgressLogger& m_logger;
    };

    ProgressLogger(std::ostream& ostr, const std::string& prefix, size_t total, size_t index = 0)
        : m_ostr(ostr)
        , m_prefix(prefix)
        , m_index(index)
        , m_total(total)
    {}

    ProgressLogger(const ProgressLogger&) = delete;
    ProgressLogger& operator= (const ProgressLogger&) = delete;

    ~ProgressLogger()
    {
        print();
        m_ostr << " completed.\n";
    }

private:
    void tick_begin() const
    {
        print();
        m_ostr << '\n';
    }

    void tick_end()
    {
        ++m_index;
        rewind_line();
    }

    void print() const
    {
        m_ostr << m_prefix << " computation... " << m_index << " / " << m_total;
    }

    void rewind_line() const
    {
        m_ostr << "\033[F";
    }

    std::ostream& m_ostr;
    const std::string m_prefix;
    size_t m_index;
    const size_t m_total;
};

}
