
#pragma once

#include <queue>
#include <vector>

#include "action.hpp"
#include "graph_passenger.hpp"


namespace passenger {

/// @brief A numeric range with a templated numeric type.
/// @tparam num The numeric type of the range's number.
template< typename num = num_default >
class Range {
public:
    /// @brief The lower range's number
    const num from;
    /// @brief The upper range's number
    const num to;

    /// @brief The constructor.
    /// @param start The lower range's number
    /// @param end The upper range's number
    Range( const num start, const num end )
        :   from( start )
        ,   to( end )
    {}
};


/// @brief A simple number enumeration that keeps track of ranges of member marker's enumerata number so that they
///     can be member, allowed or unallowed on an attributed object. Inherits `std::vector< Range< num > >`.
/// @tparam num The numeric type of the NumberEnumeration
template< typename num = num_default >
class NumberEnumeration 
    :   protected std::vector< Range< num > > 
{
protected:
    /// @brief The vector type of `NumberEnumeration`.
    typedef std::vector< Range< num > > vector_t;

public:
    /// @brief The iterator type of `NumberEnumeration`.
    typedef typename std::vector< Range< num > >::const_iterator iterator;

    /// @brief The constructor.
    NumberEnumeration()
        :   std::vector< Range< num > >()
    {}

    /// @brief Add a `Range< num >` that reflects new members, allowances or disallowances with a class inheriting from `NumberEnumeration`.
    /// @param rng A range that interval includes all member's numbers.
    void add_range( const Range< num > rng ) {
        this->push_back( rng );
    }

    /// @brief Add a `Range< num >` (from, to), that reflects new members, allowances or disallowances with a class inheriting from `NumberEnumeration`.
    /// @param rng A range that interval includes all member's numbers.
    void add_range( const num from, const num to ) {
        Range< num > rng( from, to );
        this->push_back( rng );
    }

    /// @brief Test, if a number is member in one of the ranges or not.
    /// @param member_num The number of the type `num` to test for.
    /// @return If the number is member of the vector or not.
    bool is_member( const num member_num ) {
        for ( unsigned int i = 0; i < this->size(); i++ ) {
            Range< num > rng = this->at( i );
            if ( rng.from == 0 && rng.to == 0 ) {
                return true;
            } else if ( rng.from <= member_num && rng.to >= member_num ) {
                return true;
            }
            return false;
        }
    }

    /// @brief Returns the number of currently stored ranges.
    /// @return The size of this vector.
    size_t size() const {
        return this->size();
    }

    /// @brief Get the `Range< num >` at the position `pos`.
    /// @param pos The position of the range inside the vector.
    /// @return The `Range< num >`, if index in range or *(new Range< num >( 0, 0 )), if out of range.
    const Range< num >& at( const unsigned int pos ) const {
        if ( pos < this->size() ) {
            return *this->at( pos );
        }
        return *(new Range< num >( 0, 0 ));
    }

    /// @brief Get an iterator to the first position.
    /// @return An `iterator` to the first position.
    iterator begin() const {
        return vector_t::begin();
    }

    /// @brief Get an iterator to behind-the-last element's position.
    /// @return An `iterator` to behind-the-last element's position.
    iterator end() const {
        return vector_t::end();
    }
};


/// @brief A controller for `NumberEnumeration`.
/// @tparam num The numeric type for this class and `NumberEnumeration`.
template< typename num = num_default >
class PassengerNumber {
public:
    /// @brief A `NumberEnumeration< num >` with the numeric type `num`.
    typedef NumberEnumeration< num > numberenumeration_t;
    /// @brief A `Range< num >` with the numeric type `num`.
    typedef Range< num > member_t;

protected:
    /// @brief The members that are enumerated here
    NumberEnumeration< num > m_members;
    /// @brief The amount of members that are in the ranges of `m_members`.
    num m_members_count;
    /// @brief The amount of members that are here.
    num m_members_there;

    /// @brief The infimum (highest number) of a number enumeration.
    /// @param members A `numberenumeration_t` e.ex. `num hi = this->highest_member_number( this );`
    /// @return The highest number of all ranges in `m_members`.
    const num highest_member_number( const NumberEnumeration< num > members ) {
        num highest_num = 0;
        for ( typename numberenumeration_t::iterator it = members.start(); it < members.end(); ++it ) {
            if ( highest_num < it->to ) {
                highest_num = it->to;
            }
        }
        return highest_num;
    }

    /// @brief Find out, if another member range is disjunct to all other ranges, that means that it is not
    ///     within one of the `Range` of `m_members`.
    /// @param mem The other member range.
    /// @return True, if the other member range is not within one of the ranges of `m_members`.
    bool member_range_disjunct( const member_t mem ) const {
        for ( typename numberenumeration_t::iterator it = m_members.start(); it < m_members.end(); ++it ) {
            if (  (mem.from > it->from && mem.from < it->to )
              || ( mem.to < it->to && mem.to > it->from )
            ) {
                return false;
            }
        }
        return true;
    }

    /// @brief Find out, if all member ranges are disjunct to all other ranges, that means that it is not
    ///     within one of the `Range` per object of `m_members`.
    /// @return True, if the other member ranges are not within one of the ranges of `m_members`.
    bool member_ranges_disjunct() const {
        for ( typename numberenumeration_t::iterator it = m_members.start(); it < m_members.end(); ++it ) {
            for ( typename numberenumeration_t::iterator it_inner = it; it_inner < m_members.end(); ++it_inner ) {
                if ( ( it_inner->from > it->from && it_inner->from < it->to )
                  || ( it_inner->to < it->to && it_inner->to > it->from )
                ) {
                    return false;
                }
            }
        }
        return true;
    }

public:
    /// @brief The constructor.
    /// @param members A default number enumeration with ranges that is to be set for `m_members`.
    PassengerNumber( const NumberEnumeration< num > members = NumberEnumeration< num >() )
        :   m_members( members )
        ,   m_members_count( members.size() )
        ,   m_members_there( 0 )
    {}
    /// @brief The constructor.
    PassengerNumber()
        :   m_members( NumberEnumeration< num >() )
        ,   m_members_count( 0 )
        ,   m_members_there( 0 )
    {}

    /// @brief Add a new member range to `m_members`. Only fulfills, if disjunct to the others that are already stored.
    /// @param member_range The new member range for this class with the type `Range< num >`.
    /// @return True, if the new range is not within another already stored range, false otherwise.
    bool add_member( const member_t member_range ) {
        if( this->member_range_disjunct( member_range ) ) {
            m_members.add_range( member_range );
            ++m_members_count;
            return true;
        }
        return false;
    }
};


/// @brief A simple node.
/// @tparam GraphPassengerT A graph passenger's graph marker, an inheritor of at least `GraphPassenger`.
template< class GraphPassengerT = GraphPassenger >
class Node {
    static unsigned int next_number = 0;

protected:
    /// @brief A `std::queue` with all graph markers that are currently at this node.
    std::queue< GraphPassengerT& > m_arrivals;

public:
    /// @brief The type of the node traveller.
    typedef GraphPassengerT node_passenger_t;

    /// @brief The number of this node
    const unsigned int num_node;

    /// @brief The name of this node
    const std::string name;

    /// @brief The constructor.
    Node( const char* node_name = "" )
        :   m_arrivals()
        ,   num_node( ++next_number )
        ,   name( node_name )
    {}

    /// @brief Arrive with one traveller's marker at this node.
    /// @param passenger A graph marker that is already a specializing inheritor of `GraphPassenger` or `GraphPassengerNumber`.
    /// @return True on success.
    virtual bool arrive( const GraphPassengerT& passenger ) {
        for ( typename std::queue< GraphPassengerT& >::const_iterator it = m_arrivals.front()
            ; it != m_arrivals.back(); ++it
        ) {
            if ( (*it) == passenger ) {
                return true;
            }
        }
        m_arrivals.push( passenger );
        return true;
    }

    /// @brief Get the first element out of the queue and delete it inside of the queue
    /// @return A `GraphPassengerT&`, e.h. the first element of the used `std::queue< GraphPassengerT& >`.
    virtual GraphPassengerT& pop() {
        return m_arrivals.pop();
    }

    /// @brief Leave this node. Calls `this->pop()`, but needs to be specialiced in inheriting class, because no edge is known here.
    /// @return False. On inheritance true, if successfully on edge or at the next node.
    virtual bool departure() {
        if ( m_arrivals.size() == 0 ) {
            return false;
        }

        this->pop();
        return false;
    }
};


/// @brief A numbered and controlled node, inheriting `Node< GraphPassengerT >` and `PassengerNumber< num >`.
/// @note This is the last class without an edge as template parameter. Please have this in mind because of cyclic template parameters.
///     NOTE: Base classes also work as a template parameter.
/// @tparam GraphPassengerT A graph passenger's graph marker, an inheritor of at least `GraphPassenger`.
/// @tparam num The numeric type of member ranges and `PassengerNumber`.
template< class GraphPassengerT = GraphPassengerNumber<>, typename num = num_default >
class NodeNumbered
    :   public Node< GraphPassengerT >
    ,   protected PassengerNumber< num >
{
public:
    /// @brief Constructor.
    NodeNumbered()
        :   Node< GraphPassengerT >()
        ,   PassengerNumber< num >()
    {}

    /// @brief Check, if a graph traveller can arrive.
    /// @param passenger A passenger towards this node.
    /// @return True, if `passenger` is a member of `m_members`.
    virtual bool can_arrive( const GraphPassengerT& passenger ) {
        return this->m_members.is_member( passenger.number() );
    }

    /// @brief Arrive at this targeting node.
    /// @param passenger A graph passenger
    /// @return True, if `this->can_arrive(passenger) && Node< GraphPassengerT >::arrive(passenger)`.
    virtual bool arrive( const GraphPassengerT& passenger ) {
        if ( this->can_arrive(passenger) ) {
            return Node< GraphPassengerT >::arrive( passenger );
        }
        return false;
    }

    /// @brief Add a `Range< num >` (from, to), that reflects new members, allowances or disallowances with a class inheriting from `NumberEnumeration`.
    /// @param rng A range that interval includes all member's numbers.
    bool add_member_range( const num from, const num to ) {
        Range< num > rng( from, to );
        return PassengerNumber< num >::add_member( rng );
    }
};


/// @brief The passenger node that is specially for `GraphPassengerNumber`, inherits `NodeNumbered< GraphPassengerT, num >`.
/// @note The first class with an edge as template parameter. Please mind, that cyclic template parameters are not possible.
///     NOTE: Base classes can also be template parameters.
/// @tparam GraphPassengerT A graph passenger's graph marker, an inheritor of at least `GraphPassenger`.
/// @tparam EdgeT The type of the edges at and of this node.
/// @tparam num The numeric type of member ranges and `PassengerNumber`.
template< class GraphPassengerT, class EdgeT, typename num = num_default >
class PassengerNode
    :   public NodeNumbered< GraphPassengerT, num >
{
protected:
    /// @brief A vector with all outgoing edges, that should have a reference to the target node.
    std::vector< const EdgeT& > m_edges_outgoing;

public:
    /// @brief The type of the controller for `NumberEnumeration< num >`
    typedef PassengerNumber< num > passengernumber_t;

    /// @brief Constructor.
    PassengerNode()
        :   NodeNumbered< GraphPassengerT >()
        ,   m_edges_outgoing()
    {}
    /// @brief Another constructor.
    /// @param members Define a defaulting start member ranges list here.
    PassengerNode( const typename passengernumber_t::numberenumeration_t members )
        :   NodeNumbered< GraphPassengerT >()
        ,   m_edges_outgoing()
    {
        for ( unsigned int i = 0; i < members.size(); i++ ) {
            this->m_members.add_member( members.at(i) );
        }
    }

    /// @brief Add an outgoing edge.
    /// @param new_edge The new edge.
    /// @return True on success.
    bool add_edge( const EdgeT& new_edge ) {
        for ( typename std::vector< const EdgeT& >::const_iterator it = this->m_edges_outgoing.begin()
            ; it < this->m_edges_outgoing.end(); ++it
        ) {
            if ( (*it) == new_edge ) {
                return true;
            }
        }
        m_edges_outgoing.push_back( new_edge );
        return true;
    }

    /// @brief Leave this node with the edge at the index `edge_idx` at the known edge's list.
    /// @param edge_idx The edge index of the edge to use for departure.
    /// @return The return value of `this->departure()` or false on no success.
    virtual bool departureTo( const unsigned int edge_idx ) {
        if ( this->m_arrivals.size() == 0 || this->m_edges_outgoing.size() < edge_idx ) {
            return false;
        }

        GraphPassengerT& passenger = this->begin();
        EdgeT& edge = this->m_edges_outgoing.at( edge_idx );
        
        if ( ! edge.absolve( passenger ) ) {
            return false;
        }

        return this->departure();
    }
};


/// @brief The passenger node that is specially for `GraphPassengerNumber`, inherits `NodeNumbered< GraphPassengerT, num >`.
/// @note The class is with an edge as template parameter. Please mind, that cyclic template parameters are not possible.
///     NOTE: Base classes can also be template parameters.
/// @tparam GraphPassengerT A graph passenger's graph marker, an inheritor of at least `GraphPassenger`.
/// @tparam EdgeT The type of the edges at and of this node.
/// @tparam num The numeric type of member ranges and `PassengerNumber`.template< class GraphPassengerT, class EdgeT, typename num = num_default >
template< class GraphPassengerT, class EdgeT, typename num = num_default >
class PassengerNodeActional
    :   public PassengerNode< GraphPassengerT, EdgeT, num >
    ,   public ActionPassengerGroup< GraphPassengerT >
{
protected:
    /// @brief The function that is to be absolved pre arrivance.
    std::string m_func_pre_arrive;
    /// @brief The function that is to be absolved pre departure.
    std::string m_func_pre_departure;

public:
    /// @brief The `PassengerNumber` with template specification.
    typedef PassengerNumber< num > passengernumber_t;
    /// @brief A passenger group a vector.
    typedef typename std::vector< GraphPassengerT& > passenger_group_t;

    /// @brief Constructor.
    PassengerNodeActional()
        :   PassengerNode< GraphPassengerT, EdgeT, num >()
        ,   ActionPassengerGroup< GraphPassengerT >()
    {}
    /// @brief Another constructor.
    /// @param members Define a defaulting start member ranges list here.
    PassengerNodeActional( const typename passengernumber_t::numberenumeration_t members )
        :   PassengerNode< GraphPassengerT, EdgeT, num >( members )
        ,   ActionPassengerGroup< GraphPassengerT >()
    {}

    /// @brief Arrive at this targeting node.
    /// @param passenger A graph passenger
    /// @return True, if `this->can_arrive(passenger) && Node< GraphPassengerT >::arrive(passenger)`.
    virtual bool arrive( const GraphPassengerT& passenger ) {
        if ( this->can_arrive(passenger) ) {
            if ( ActionPassengerGroup< GraphPassengerT >::run( this->m_func_pre_arrive, passenger ) ) {
                return Node< GraphPassengerT >::arrive( passenger );
            }
        }
        return false;
    }

    /// @brief Leave this node with the edge at the index `edge_idx` at the known edge's list and absolate all functions, e.g.
    ///     pre-leave, edge's and pre-arriving.
    /// @param edge_idx The edge index of the edge to use for departure.
    /// @return The return value of `this->departure()` or false on no success.
    virtual bool departureTo( const unsigned int edge_idx ) {
        if ( this->m_arrivals.size() == 0 || this->m_edges_outgoing.size() < edge_idx ) {
            return false;
        }

        GraphPassengerT& passenger = this->begin();

        if ( ! ActionPassengerGroup< GraphPassengerT >::run( this->m_func_pre_departure, passenger ) ) {
            return false;
        }

        EdgeT& edge = this->m_edges_outgoing.at( edge_idx );
        
        if ( ! edge.absolve( passenger ) ) {
            return false;
        }

        return this->departure();
    }

    /// @brief Run a function actional with function's name `funcname` at this node.
    /// @param funcname The name of the function to be executed. Needs to be part of the functions of `ActionPassengerGroup< GraphPassengerT >`.
    /// @param passenger_group The passenger group that takes place in the functional.
    /// @return True on success.
    virtual unsigned int run( const std::string funcname, const passenger_group_t& passenger_group = passenger_group_t() ) {
        if ( passenger_group.size() == 0 ) {
            // Actional with all passenger markers currently available at this node
            return ActionPassengerGroup< GraphPassengerT >::run( { this->m_arrivals.begin(), this->m_arrivals.end() } );
        }

        unsigned int missed = 0;
        for ( typename passenger_group_t::const_iterator it = passenger_group.begin()
            ; it != passenger_group.end(); ++it
        ) {
            if ( ! ActionPassengerGroup< GraphPassengerT >::run( funcname, *it ) ) {
                ++missed;
            }
        }

        return missed;
    }
};

} // namespace passenger
