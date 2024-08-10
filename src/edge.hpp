
#pragma once

#include "graph_passenger.hpp"


template< class GraphPassengerT = GraphPassenger >
class EdgeSimple {
public:
    EdgeSimple();

    virtual bool absolve( const GraphPassengerT& passenger ) { return false; }
    virtual bool arrive( const GraphPassengerT& passenger ) {
        return true;
    }
};


template< class GraphPassengerT, class ActionT >
class Edge
    :   EdgeSimple< GraphPassengerT >
{
protected:
    ActionT& m_action;
    std::string m_action_funcname;

public:
    Edge( const ActionT& action, const std::string action_funcname )
        :   EdgeSimple< GraphPassengerT >()
        ,   m_action( action )
        ,   m_action_funcname( action_funcname )
    {}

    bool absolve( const GraphPassengerT& passenger ) {
        return this->m_action.run( passenger, m_action_funcname );
    }
};


template< class GraphPassengerT
        , class ActionT
        , class NodeT >
class EdgeToNode
    :   Edge< GraphPassengerT, ActionT >
{
protected:
    NodeT* m_target_node;

public:
    EdgeToNode( const ActionT& action, NodeT* node = nullptr )
        :   Edge< GraphPassengerT, ActionT >( action )
        ,   m_target_node( node )
    {}

    bool arriveAtNode( const GraphPassengerT& passenger ) {
        return m_target_node->arrive( passenger );
    }

    bool absolve( const GraphPassengerT& passenger ) {
        if ( Edge< GraphPassengerT, ActionT >::absolve( passenger ) ) {
            return this->arriveAtNode( passenger );
        }
        return false;
    }
};
