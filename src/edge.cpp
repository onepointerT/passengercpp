
#include "edge.hpp"

#include "action.hpp"
#include "passenger.hpp"

namespace passenger {

Action<>;
EdgeSimple<>;
Edge< GraphPassenger, Action<> >;
EdgeToNode< GraphPassenger, Action<>, NodeNumbered<> >;
EdgeToNode< GraphPassengerNumber<>, Action<>, NodeNumbered<> >;

} // namespace passenger
