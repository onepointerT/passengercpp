# passengercpp
### A library that is havening a passenger arriving and departing node graph with and without actions on edges
#### v0.1.0

This small library is implemented to handle concurrent consent arbitrary passengers of nodes and solves the following questions to source code and graph marker member departure behaviour:

* Which members need to be arrived to start at this node
* Which members are accepted at the node
* Which members are already arrived at the node
* How many members can arrive at a node
* Does an edge have an action to implement, and if any, it will be absolved

## How does it work

An edge can define an action function to absolve per edge. An Action function has the signature `parameter_list_t (*action_function)(GraphPassenger, std::string)`, where `GraphPassenger` is the member marker of the absolving passenger and `std::string` is the name of one of the functions that are available to `Action` and can be defined with `ActionFunction` from `action.hpp`. Once a function is absolved by the `GraphPassenger`, a parameter list of the calculated values will be handed out to `GraphPassenger::absolvedActionFunction`, which then calls the virtual function `GraphPassenger::absolvedFunction`, that can be defined in inheritors of `GraphPassenger`, so that every update of the named (key, value)-pairs will call an update to the inheriting class of `GraphPassenger`, e.g. when passing an edge or combining passenger markers at a `PassengerNode`.

Start your first passenger node with the class `PassengerNode`. Each passenger node has a stack of members (inherited class `GraphPassengerNumber`) that are allowed to arrive and take place at the `PassengerNode`.

The following example shows how a 3-node graph with two edges can be created, use simple actions at the edges and fullfil.

```c++
#include "action.hpp"
#include "edge.hpp"
#include "graph.hpp"
#include "graph_passenger.hpp"
#include "passenger.hpp"

#include <iostream>
#include <string>

typedef typename GraphPassengerNumber< unsigned int > GraphPassengerT;
typedef typename Action< GraphPassengerT > ActionT;
typedef typename EdgeToNode< GraphPassengerT, ActionT > EdgeT;
typedef typename PassengerNode< GraphPassengerT, EdgeT > PassengerNodeT;
typedef typename Graph< PassengerNodeT > GraphT;

GraphT graph();

struct functions {
    parameter_list_t nextEdge( GraphPassengerT& passenger, std::string funcname ) {
        std::out << "Passing edge and arriving at next node" << std::endl;
        return parameter_list_t{ did: "next_edge" };
    }

    parameter_list_t leaveStart( GraphPassengerT& passenger, std::string funcname ) {
        std::out << "Leaving start node to first node" << std::endl;
        return parameter_list_t{ nodes_passed: "1", nodes_left: std::to_string(graph.size()-1) };
    }

    parameter_list_t arriveEnd( GraphPassengerT& passenger, std::string funcname ) {
        std::out << "Leaving pre-last node to end node" << std::endl;
        return parameter_list_t{ nodes_passed: std::to_string(graph.size()-1), nodes_left: "1" };
    }
};

// Now a few actions
ActionFunction actionfunction_startEdge( "leaveStart", &functions::leaveStart );
ActionFunction actionfunction_endEdge( "arriveEnd", &functions::arriveEnd );
ActionFunction actionfunction_nextEdge( "nextEdge", &functions::nextEdge );

ActionT actions();
action.register_function( actionfunction_startEdge );
action.register_function( actionfunction_endEdge );
action.register_function( actionfunction_nextEdge );

// Now our nodes
PassengerNodeT* node_start = new PassengerNodeT( PassengerNumber<>() );
PassengerNodeT* node_end = new PassengerNodeT( PassengerNumber<>() );
PassengerNodeT* node_middle = new PassengerNodeT( PassengerNumber<>() );

// Register the nodes
graph.reset_start( node_start );
graph.reset_end( node_end );

// Create edges
EdgeT edge_middle( actions, node_middle );
node_start->add_edge( edge_middle );
EdgeT edge_end( actions, node_end );
node_middle->add_edge( edge_end );

// Add passengers
GraphPassengerT p1( "me" );
GraphPassengerT p2( "you" );
graph.send_to_start( p1 );
graph.send_to_start( p2 );
```
