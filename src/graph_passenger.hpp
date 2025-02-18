
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "config.hpp"



namespace passenger {

/// @brief A parameter named tuple with (param_name, param_value).
typedef typename std::pair< std::string, std::string > parameter_pair_t;
/// @brief A list of parameter named tuples of the type `parameter_pair_t`
typedef typename std::vector< parameter_pair_t > parameter_list_t;


/// @brief A traveller through a graph, its edges and nodes. Inherits `std::string` for better naming convenience.
class GraphPassenger
    :   public std::string
{
public:

protected:
    /// @brief A `parameter_list_t` with parameter pair values of this travelling graph marker, also called value flags.
    parameter_list_t m_parameter_pairs;

public:
    /// @brief The constructor.
    GraphPassenger();
    /// @brief The constructor.
    /// @param name The name or an number as `std::string` that identifies and difers this `GraphPassenger` from others.
    GraphPassenger( const std::string name );

    /// @brief Get the value flag value of `m_parameter_pairs` for the key.
    /// @param key The key to lookup the parametristic value of.
    /// @return The `std::string value` found for `key` or an empty literal.
    const std::string getValue( const std::string key ) const;

    /// @brief Can be called as informer, when a function was absolvated.
    /// @return If this function succeeded.
    virtual bool absolvedFunction();
    /// @brief Can be called from an informing class, when a function actional like the ones from `Action` or a additionally contributing function.
    /// @param parameters Pass the changing parameters here.
    /// @return Return the If the function succeeded.
    virtual bool absolvedActionFunction( const parameter_list_t parameters );
};


/// @brief A specialization of `GraphPassenger`. Can be used to numerata the graph passengers.
/// @tparam num The numeric type, that the graph works with for enumering the traveller markers.
template< typename num = num_default >
class GraphPassengerNumber
    :   public GraphPassenger
{
protected:
    /// @brief The number that is saved for enumering graph passengers and its traveller marker
    const num m_num;

public:
    /// @brief A static counter for automatic indices of the next current number
    static num current_member_num = 0;

    /// @brief The constructor. Please specialize for another counting of next current numbers, e.ex. for `float`.
    /// @param member_num The member enumerata for this `GraphPassenger`
    GraphPassengerNumber( const num member_num = ++GraphPassengerNumber::current_member_num )
        :   GraphPassenger()
        ,   m_num( member_num )
    {}

    /// @brief The constructor. Please specialize for another counting of next current numbers, e.ex. for `float`.
    /// @param name The name or an number as `std::string` that identifies and difers this `GraphPassenger` from others.
    /// @param member_num The member enumerata for this `GraphPassenger`.
    GraphPassengerNumber( const std::string name, const num member_num = ++GraphPassengerNumber::current_member_num )
        :   GraphPassenger( name )
        ,   m_num( member_num )
    {}

    /// @brief Get the number of this `GraphPassenger`.
    /// @return The enumerata of this passenger graph marker of the type `num`.
    const num number() const {
        return m_num;
    }
};

} // namespace passenger
