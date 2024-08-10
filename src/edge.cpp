
#include "edge.hpp"

#include "action.hpp"
#include "passenger.hpp"


Action<>;
EdgeSimple<>;
Edge< GraphPassenger, Action<> >;
EdgeToNode< GraphPassenger, Action<>, NodeNumbered<> >;
EdgeToNode< GraphPassengerNumber<>, Action<>, NodeNumbered<> >;
