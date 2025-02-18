
#include "passenger.hpp"

#include "edge.hpp"


namespace passenger {

Range<>;
Range< num_non_default >;
NumberEnumeration<>;
NumberEnumeration< num_non_default >;
PassengerNumber<>;
PassengerNumber< num_non_default >;
Node<>;
Node< GraphPassengerNumber<> >;
Node< GraphPassengerNumber< num_non_default > >;
NodeNumbered< GraphPassengerNumber<> >;
NodeNumbered< GraphPassengerNumber< num_non_default >, num_non_default >;
PassengerNode< GraphPassenger, EdgeSimple<> >;
PassengerNode< GraphPassenger, EdgeSimple<>, num_non_default >;
PassengerNodeActional< GraphPassenger, EdgeSimple<>, num_default >;
PassengerNodeActional< GraphPassengerNumber<>, EdgeSimple<>, num_default >;

} // namespace passenger
