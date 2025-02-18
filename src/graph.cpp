
#include "graph.hpp"

#include "action.hpp"
#include "edge.hpp"
#include "graph_passenger.hpp"
#include "passenger.hpp"


namespace passenger {

Graph< Node< GraphPassenger > >;
Graph< Node< GraphPassengerNumber<> > >;
Graph< PassengerNode< GraphPassenger, Edge< GraphPassenger, Action< GraphPassenger > > > >;
Graph< PassengerNode< GraphPassengerNumber<>, Edge< GraphPassengerNumber<>, Action< GraphPassengerNumber<> > > > >;

} // namespace passenger
