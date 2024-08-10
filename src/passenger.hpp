
#pragma once

#include <queue>
#include <vector>

#include "action.hpp"
#include "graph_passenger.hpp"


template< typename num = num_default >
class Range {
public:
    const num from;
    const num to;

    Range( const num start, const num end )
        :   from( start )
        ,   to( end )
    {}
};


template< typename num = num_default >
class NumberEnumeration 
    :   protected std::vector< Range< num > > 
{
protected:
    typedef std::vector< Range< num > > vector_t;

public:
    typedef typename std::vector< Range< num > >::const_iterator iterator;

    NumberEnumeration()
        :   std::vector< Range< num > >()
    {}

    void add_range( const Range< num > rng ) {
        this->push_back( rng );
    }

    void add_range( const num from, const num to ) {
        Range< num > rng( from, to );
        this->push_back( rng );
    }

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

    size_t size() const {
        return this->size();
    }

    const Range< num >& at( const unsigned int pos ) const {
        if ( pos < this->size() ) {
            return *this->at( pos );
        }
        return &(new Range< num >( 0, 0 ));
    }

    iterator begin() const {
        return vector_t::begin();
    }

    iterator end() const {
        return vector_t::end();
    }
};


template< typename num = num_default >
class PassengerNumber {
public:
    typedef NumberEnumeration< num > numberenumeration_t;
    typedef Range< num > member_t;

protected:
    NumberEnumeration< num > m_members;
    num m_members_count;
    num m_members_there;

    const num highest_member_number( const NumberEnumeration< num > members ) {
        num highest_num = 0;
        for ( typename numberenumeration_t::iterator it = members.start(); it < members.end(); ++it ) {
            if ( highest_num < it->to ) {
                highest_num = it->to;
            }
        }
        return highest_num;
    }

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
    PassengerNumber( const NumberEnumeration< num > members = Range<>(0, 0) )
        :   m_members( members )
        ,   m_members_count( members.size() )
        ,   m_members_there( 0 )
    {}
    PassengerNumber()
        :   m_members( NumberEnumeration< num >() )
        ,   m_members_count( 0 )
        ,   m_members_there( 0 )
    {}

    bool add_member( const member_t member_range ) {
        if( this->member_range_disjunct( member_range ) ) {
            m_members.add_range( member_range );
            ++m_members_count;
            return true;
        }
        return false;
    }
};


template< class GraphPassengerT = GraphPassenger >
class Node {
protected:
    std::queue< GraphPassengerT& > m_arrivals;

public:
    typedef GraphPassengerT node_passenger_t;
    Node()
        :   m_arrivals()
    {}

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

    virtual GraphPassengerT& pop() {
        return m_arrivals.pop();
    }

    virtual bool departure() {
        if ( m_arrivals.size() == 0 ) {
            return false;
        }

        this->pop();
        return false;
    }
};


template< class GraphPassengerT = GraphPassengerNumber<>, typename num = num_default >
class NodeNumbered
    :   public Node< GraphPassengerT >
    ,   protected PassengerNumber< num >
{
public:
    NodeNumbered()
        :   Node< GraphPassengerT >()
        ,   PassengerNumber< num >()
    {}

    bool can_arrive( const GraphPassengerT& passenger ) {
        return this->m_members.is_member( passenger.number() );
    }

    virtual bool arrive( const GraphPassengerT& passenger ) {
        if ( this->can_arrive(passenger) ) {
            return Node< GraphPassengerT >::arrive( passenger );
        }
        return false;
    }

    bool add_member_range( const num from, const num to ) {
        Range< num > rng( from, to );
        return PassengerNumber< num >::add_member( rng );
    }
};



template< class GraphPassengerT, class EdgeT, typename num = num_default >
class PassengerNode
    :   public NodeNumbered< GraphPassengerT >
{
protected:
    std::vector< const EdgeT& > m_edges_outgoing;

public:
    typedef PassengerNumber< num > passengernumber_t;

    PassengerNode()
        :   NodeNumbered< GraphPassengerT >()
        ,   m_edges_outgoing()
    {}
    PassengerNode( const typename passengernumber_t::numberenumeration_t members )
        :   NodeNumbered< GraphPassengerT >()
        ,   m_edges_outgoing()
    {
        for ( unsigned int i = 0; i < members.size(); i++ ) {
            this->m_members.add_member( members.at(i) );
        }
    }

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


template< class GraphPassengerT, class EdgeT, typename num = num_default >
class PassengerNodeActional
    :   public PassengerNode< GraphPassengerT, EdgeT, num >
    ,   public ActionPassengerGroup< GraphPassengerT >
{
protected:
    std::string m_func_pre_arrive;
    std::string m_func_pre_departure;

public:
    typedef PassengerNumber< num > passengernumber_t;
    typedef typename std::vector< GraphPassengerT& > passenger_group_t;

    PassengerNodeActional()
        :   PassengerNode< GraphPassengerT, EdgeT, num >()
        ,   ActionPassengerGroup< GraphPassengerT >()
    {}
    PassengerNodeActional( const typename passengernumber_t::numberenumeration_t members )
        :   PassengerNode< GraphPassengerT, EdgeT, num >( members )
        ,   ActionPassengerGroup< GraphPassengerT >()
    {}

    virtual bool arrive( const GraphPassengerT& passenger ) {
        if ( this->can_arrive(passenger) ) {
            if ( ActionPassengerGroup< GraphPassengerT >::run( this->m_func_pre_arrive, passenger ) ) {
                return Node< GraphPassengerT >::arrive( passenger );
            }
        }
        return false;
    }

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
