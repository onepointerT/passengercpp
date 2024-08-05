
#pragma once

#include "graph_passenger.hpp"


template< class GraphPassengerT = GraphPassenger >
class Action {
public:
    virtual bool run( const GraphPassengerT& passenger ) { return false; }

    Action() {}
};


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

public:
    Edge( const ActionT& action )
        :   EdgeSimple< GraphPassengerT >()
        ,   m_action( action )
    {}

    bool absolve( const GraphPassengerT& passenger ) {
        return this->m_action.run( passenger );
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
        if ( Edge< GraphPassengerT, ActionT< GraphPassengerT > >::absolve( passenger ) ) {
            return this->arriveAtNode( passenger );
        }
        return false;
    }
};
