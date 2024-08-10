
#pragma once

#include "config.hpp"
#include "graph_passenger.hpp"

#include <functional>
#include <string>
#include <vector>
#include <utility>


extern "C" {
typedef typename std::vector< std::pair< std::string, std::string > >(*action_function_t)(GraphPassenger&, std::string);
}


class ActionFunction {
protected:
    action_function_t m_func;

public:
    const std::string funcname;

    ActionFunction( const std::string nameof_func, action_function_t func );

    template< class absolverT >
    bool run( absolverT absolver, const std::string nameof_func ) {
        if ( nameof_func != funcname ) {
            return false;
        }

        parameter_list_t returned_parameters = m_func( absolver );
        return absolver->absolvedActionFunction( returned_parameters );
    };
};


typedef typename std::pair< std::string, ActionFunction > named_action_function_t;

template< class GraphPassengerT = GraphPassenger >
class Action {
protected:
    static std::vector< ActionFunction > m_functions;

public:
    static bool register_function( const ActionFunction& function ) {
        for ( typename std::vector< ActionFunction >::iterator it = m_functions.begin()
            ; it != m_functions.end(); ++it
        ) {
            if ( it->funcname != function.funcname ) {
                continue;
            } else {
                return false;
            }
        }

        m_functions.push_back( function );
        return true;
    }

    static unsigned int register_functions( const std::vector< ActionFunction > functions ) {
        unsigned int mistaken = 0;

        for ( typename std::vector< ActionFunction >::iterator it = functions.begin()
            ; it != functions.end(); ++it
        ) {
            if ( ! Action< GraphPassengerT >::register_function( *it ) ) {
                ++mistaken;
            }
        }

        return mistaken;
    }

    virtual bool run( const std::string funcname, const GraphPassengerT& passenger ) {
        for ( typename std::vector< ActionFunction >::iterator it = m_functions.begin()
            ; it != m_functions.end(); ++it
        ) {
            if ( it->funcname == funcname ) {
                return it->run< GraphPassengerT >( passenger, funcname );
            }
        }

        return false;
    }

    Action() {}
};


template<class GraphPassengerT> std::vector<ActionFunction> Action<GraphPassengerT>::m_functions
    = std::vector<ActionFunction>();



template< class GraphPassengerT = GraphPassengerNumber<> >
class ActionPassengerGroup
    :   public Action< GraphPassengerT >
{
protected:
    static std::vector< ActionFunction > m_functions;

public:
    typedef typename std::vector< GraphPassengerT& > passenger_group_t;

    ActionPassengerGroup()
        :   Action< GraphPassengerT >()
    {}

    virtual bool run( const std::string funcname, const passenger_group_t& passenger_group ) {
        for ( typename std::vector< ActionFunction >::iterator it = m_functions.begin()
            ; it != m_functions.end(); ++it
        ) {
            if ( it->funcname == funcname ) {
                return it->run< const passenger_group_t& >( passenger_group, funcname );
            }
        }

        return false;
    }
};



template<class GraphPassengerT> std::vector<ActionFunction> ActionPassengerGroup<GraphPassengerT>::m_functions
    = std::vector<ActionFunction>();
