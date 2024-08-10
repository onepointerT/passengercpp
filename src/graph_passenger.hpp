
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "config.hpp"



typedef typename std::pair< std::string, std::string > parameter_pair_t;
typedef typename std::vector< parameter_pair_t > parameter_list_t;


class GraphPassenger
    :   public std::string
{
public:

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
