
#pragma once

#include <vector>

#include "passenger.hpp"


template< class NodeT >
class Graph
    :   public std::vector< NodeT* >
{
protected:
    NodeT* m_start;
    NodeT* m_end;

public:
    Graph( NodeT* start = nullptr, NodeT* end = nullptr )
        :   std::vector< NodeT* >()
        ,   m_start( start )
        ,   m_end( end )
    {}

    void reset_start( NodeT* node ) {
        m_start = node;
    }

    void reset_end( NodeT* node ) {
        m_end = node;
    }

    NodeT* get_start() const {
        return m_start;
    }

    NodeT* get_end() const {
        return m_end;
    }

    bool send_to_start( typename NodeT::node_passenger_t& passenger ) {
        this->get_start()->arrive( passenger );
    }
};
