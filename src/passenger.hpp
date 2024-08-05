
#pragma once

#include <queue>
#include <vector>

#include "graph_passenger.hpp"


template< typename num = int >
class Range {
public:
    const num from;
    const num to;

    Range( const num start, const num end )
        :   from( start )
        ,   to( end )
    {}
};


template< typename num = int >
class NumberEnumeration 
    :   protected std::vector< Range< num > > 
{
protected:
    typedef typename std::vector< Range< num > > vector_t;

public:
    typedef typename std::vector< Range< num > >::const_iterator iterator;

    NumberEnumeration()
        :   std::vector< Range< num > >()
    {}

    void add_range( const Range< num > rng ) {
        this->push_back( rng )
    }

    void add_range( const num from, const num to ) {
        Range< num > rng( from, to );
        this->push_back( rng );
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


template< typename num = int >
class PassengerNumber {
public:
    typedef typename NumberEnumeration< num > numberenumeration_t;
    typedef typedef Range< num > member_t;

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
    PassengerNumber( const NumberEnumeration< num > members )
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
    Node()
        :   m_arrivals()
    {}

    bool arrive( const GraphPassengerT& passenger ) {
        for ( std::queue< GraphPassengerT& >::const_iterator it = m_arrivals.front()
            ; it =< m_arrivals.back(); ++it
        ) {
            if ( (*it) == passenger ) {
                return true;
            }
        }
        m_arrivals.push( passenger )
        return true;
    }
};



template< class GraphPassengerT, class EdgeT, typename num = int >
class PassengerNode
    :   public PassengerNumber< num >
    ,   public Node< GraphPassengerT >
{
protected:
    std::vector< const EdgeT& > m_edges_outgoing;

public:
    typedef typename PassengerNumber< num > passengernumber_t;

    PassengerNode()
        :   PassengerNumber()
        ,   Node< GraphPassengerT >()
        ,   m_edges_outgoing()
    {}
    PassengerNode( const typename passengernumber_t::numberenumeration_t members )
        :   PassengerNumber( members )
        ,   Node< GraphPassengerT >()
        ,   m_edges_outgoing()
    {}

    bool add_edge( const EdgeT& new_edge ) {
        for ( std::vector< const EdgeT& >::const_iterator it = m_edges_outgoing.begin()
            ; it < m_edges_outgoing.end(); ++it
        ) {
            if ( (*it) == new_edge ) {
                return true;
            }
        }
        m_edges_outgoing.push_back( new_edge );
        return true;
    }
};

