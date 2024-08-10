
#pragma once

#include "graph_passenger.hpp"
#include "passenger.hpp"


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
    :   public EdgeSimple< GraphPassengerT >
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
        , class NodeT = NodeNumbered< num_default > >
class EdgeToNode
    :   public Edge< GraphPassengerT, ActionT >
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


template< class GraphPassengerT
        , class ActionT
        , class NodeT = NodeNumbered< num_default >
        , typename num = num_default >
class EdgeControlled
    :   public EdgeToNode< GraphPassengerT, ActionT, NodeT >
    ,   public PassengerNumber< num >
{
protected:
    NodeT* m_target_node;

public:
    EdgeControlled( const ActionT& action, NodeT* node = nullptr )
        :   Edge< GraphPassengerT, ActionT >( action )
        ,   m_target_node( node )
    {}

    bool arriveAtNode( const GraphPassengerT& passenger ) {
        if ( this->m_target_node->can_arrive( passenger ) ) {
            return m_target_node->arrive( passenger );
        }
        return false;
    }

    bool departure_possible_here( const GraphPassengerT& passenger ) {
        return this->m_members.is_member( passenger );
    }

    bool departure_along_this( const GraphPassengerT& passenger ) {
        if ( this->departure_possible_here( passenger ) ) {
            return this->absolve( passenger );
        }
        return false;
    }

    bool absolve( const GraphPassengerT& passenger ) {
        if ( Edge< GraphPassengerT, ActionT >::absolve( passenger ) ) {
            return this->arriveAtNode( passenger );
        }
        return false;
    }
};
