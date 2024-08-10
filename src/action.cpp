
#include "action.hpp"

#include "graph_passenger.hpp"


ActionFunction::ActionFunction( const std::string nameof_func, action_function_t func )
    :   m_func( func )
    ,   funcname( nameof_func )
{}


Action< GraphPassenger >;
Action< GraphPassengerNumber<> >;
Action< GraphPassengerNumber< num_non_default > >;
ActionPassengerGroup< GraphPassengerNumber<> >;
ActionPassengerGroup< GraphPassengerNumber< num_non_default > >;
