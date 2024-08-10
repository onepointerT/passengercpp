
#include "graph_passenger.hpp"


GraphPassenger::GraphPassenger( const std::string name )
    :   std::string( name )
{}


bool GraphPassenger::absolvedFunction() {
    /* DO SOMETHING WITH STACK VALUES */
    return true;
}

bool GraphPassenger::absolvedActionFunction( const parameter_list_t parameters ) {
        for ( typename parameter_list_t::const_iterator it = parameters.begin()
            ; it != parameters.end(); ++it
        ) {
            bool is_new = true;
            for ( typename parameter_list_t::iterator it2 = m_parameter_pairs.begin()
                ; it2 != m_parameter_pairs.end(); ++it2
            ) {
                if ( it2->first == it->first ) {
                    it2->second = it->second;
                    is_new = false;
                    break;
                }
            }
            if ( is_new ) {
                m_parameter_pairs.push_back( *it );
            }
        }

        return this->absolvedFunction();
    }


GraphPassengerNumber< num_non_default >;
GraphPassengerNumber< num_default >;
