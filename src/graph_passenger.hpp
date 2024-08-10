
#pragma once

#include <string>

#include "action.hpp"
#include "config.hpp"


class GraphPassenger
    :   public std::string
{
protected:
    parameter_list_t m_parameter_pairs;

public:
    GraphPassenger( const std::string name );

    virtual bool absolvedFunction();
    virtual bool absolvedActionFunction( const parameter_list_t parameters );
};


template< typename num = num_default >
class GraphPassengerNumber {
protected:
    const num m_num;

public:
    static num current_member_num = 0;

    GraphPassengerNumber( const num member_num = ++GraphPassengerNumber::current_member_num )
        :   m_num( member_num )
    {}

    const num number() const {
        return m_num;
    }
};
