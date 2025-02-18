
#pragma once

#include "graph_passenger.hpp"
#include "passenger.hpp"

namespace passenger {

/// @brief A simple edge with all basic elements so that it can be inherited from
/// @tparam GraphPassengerT The graph traveler, a subclass of `GraphPassenger`
template< class GraphPassengerT = GraphPassenger >
class EdgeSimple {
public:
    /// @brief The constructor of this class
    EdgeSimple();

    /// @brief Absolvate an edge, e.g. pass the edge to the next node. Virtual function, needs to be specialized.
    /// @param passenger The absolvent of the edge with the type `GraphPassengerT`.
    /// @return Wether passing this edge succeeded or not.
    virtual bool absolve( const GraphPassengerT& passenger ) { return false; }
    /// @brief Absolvate an edge, e.g. arrive at the edge with the next action or pass to the next node.
    ///     Virtual function, needs to be specialized.
    /// @param passenger The absolvent of the edge with the type `GraphPassengerT`.
    /// @return Wether passing this edge succeeded or not.
    virtual bool arrive( const GraphPassengerT& passenger ) {
        return true;
    }
};


/// @brief A normal edge with an action. A specialization of `EdgeSimple< GraphPassengerT >`
/// @note The last class that has no node as template parameter.
/// @tparam GraphPassengerT The graph traveler, a subclass of `GraphPassenger`
/// @tparam ActionT The `Action` type, where actional functions can be called of, a specialization of at least `Action`.
template< class GraphPassengerT, class ActionT >
class Edge
    :   public EdgeSimple< GraphPassengerT >
{
protected:
    /// @brief The `ActionT` where actional function can be called at.
    ActionT& m_action;
    /// @brief An default action function name to be called when absolvating actions.
    std::string m_action_funcname;

public:
    /// @brief The constructor.
    /// @param action The `ActionT` where actional function can be called at.
    /// @param action_funcname An default action function name to be called when absolvating actions.
    Edge( const ActionT& action, const std::string action_funcname )
        :   EdgeSimple< GraphPassengerT >()
        ,   m_action( action )
        ,   m_action_funcname( action_funcname )
    {}

    /// @brief Absolvate this edge with one passenger
    /// @param passenger The graph travelers that travel alongside this edge and can absolve an action.
    /// @return If the action was successfully absolved (the return value of `Action::run()`).
    virtual bool absolve( const GraphPassengerT& passenger ) {
        return this->m_action.run( passenger, m_action_funcname );
    }
};


/// @brief A inheriting class of of `Edge` that also contains a `NodeT` where it is directing to. A direct subclass of
///     `Edge< GraphPassengerT, ActionT >`.
/// @note The first class that has a node as template parameter. Can be combined with subclasses of and with `NodeNumbered` or `Node`
///     that don't use edges for template parameters (because of cyclic template parameters).
///     NOTE: Base classes also work as a template parameter.
/// @tparam GraphPassengerT The graph traveler, a subclass of `GraphPassenger`
/// @tparam ActionT The `Action` type, where actional functions can be called of, a specialization of at least `Action`.
/// @tparam NodeT The node class that is used as a first base class with this edges, can't contain an edge as template parameters because
///     of cyclic template parameters.
template< class GraphPassengerT
        , class ActionT
        , class NodeT = NodeNumbered< num_default > >
class EdgeToNode
    :   public Edge< GraphPassengerT, ActionT >
{
protected:
    /// @brief A pointer to the node which the edge is directed to and that archieved as goaling node.
    NodeT* m_target_node;

public:
    /// @brief The constructor of this class.
    /// @param action The action that is used for calling of actional functions. May be a singleton.
    /// @param node The node where this edge ends at.
    EdgeToNode( const ActionT& action, NodeT* node = nullptr )
        :   Edge< GraphPassengerT, ActionT >( action )
        ,   m_target_node( node )
    {}

    /// @brief Don't stay on the edge, arrive at the goaling node.
    /// @param passenger The traveller of this edge.
    /// @return Returns `NodeT::arrive` or false, if this edge does not store a target node.
    virtual bool arriveAtNode( const GraphPassengerT& passenger ) {
        if ( m_target_node != nullptr )
            return m_target_node->arrive( passenger );
        return false;
    }

    /// @brief Absolve the action at this edge and travel to the node.
    /// @param passenger The traveller and absolvent at this edge.
    /// @return False, if the action could not be absolved and `arriveAtNode()` else.
    virtual bool absolve( const GraphPassengerT& passenger ) {
        if ( Edge< GraphPassengerT, ActionT >::absolve( passenger ) ) {
            return this->arriveAtNode( passenger );
        }
        return false;
    }
};


/// @brief A controlled edge that can have a few functions to controll, if arrivement, action and departure is possible here.
///     A direct subclass of `EdgeToNode< GraphPassengerT, ActionT, NodeT >` and `PassengerNumber< num >`.
/// @note This class that has a node as template parameter. Can be combined with subclasses of and with `NodeNumbered` or `Node`
///     that don't use edges for template parameters (because of cyclic template parameters).
///     NOTE: Base classes also work as a template parameter.
/// @tparam GraphPassengerT The graph traveler, a subclass of `GraphPassenger`
/// @tparam ActionT The `Action` type, where actional functions can be called of, a specialization of at least `Action`.
/// @tparam NodeT The node class that is used as a first base class with this edges, can't contain an edge as template parameters because
///     of cyclic template parameters.
/// @tparam num The numeric type for and of `PassengerNumber`.
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
    /// @brief The constructor of this class.
    /// @param action The action that is used for calling of actional functions. May be a singleton.
    /// @param node The node where this edge ends at.
    EdgeControlled( const ActionT& action, NodeT* node = nullptr )
        :   Edge< GraphPassengerT, ActionT >( action )
        ,   PassengerNumber< num >()
        ,   m_target_node( node )
    {}

    /// @brief Don't stay on the edge, arrive at the goaling node.
    /// @param passenger The traveller of this edge.
    /// @return Returns `NodeT::arrive` or false, if this edge does not store a target node.
    virtual bool arriveAtNode( const GraphPassengerT& passenger ) {
        if ( this->m_target_node->can_arrive( passenger ) ) {
            return m_target_node->arrive( passenger );
        }
        return false;
    }

    /// @brief Check, if a passenger is an edge member here and can arrive or travel along this edge.
    /// @param passenger A traveller of the graph passengers.
    /// @return True, if `passenger` is a member at this edge.
    virtual bool departure_possible_here( const GraphPassengerT& passenger ) {
        return this->m_members.is_member( passenger );
    }

    /// @brief Checks `departure_possible_here()` and does `absolve()`
    /// @param passenger A traveller of the graph passengers.
    /// @return The return value of `this->absolve() or false, if the `passenger` is not member of this edge.
    virtual bool departure_along_this( const GraphPassengerT& passenger ) {
        if ( this->departure_possible_here( passenger ) ) {
            return this->absolve( passenger );
        }
        return false;
    }

    /// @brief Absolve the action at this edge and travel to the node.
    /// @param passenger The traveller and absolvent at this edge.
    /// @return False, if the action could not be absolved and `arriveAtNode()` else.
    virtual bool absolve( const GraphPassengerT& passenger ) {
        if ( Edge< GraphPassengerT, ActionT >::absolve( passenger ) ) {
            return this->arriveAtNode( passenger );
        }
        return false;
    }
};

} // namespace passenger
