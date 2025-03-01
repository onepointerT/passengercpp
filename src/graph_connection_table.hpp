
#pragma once

#include <initializer_list>
#include <map>
#include <stdexcept>
#include <vector>
#include <utility>

#include "config.hpp"
#include "graph.hpp"
#include "tree.hpp"


namespace passenger {

/// @brief A classic table with indices on rows and columns, that can save an object in its cells
/// @tparam CellT The type of object to save in the cells, no pointer allowed here, the pointer is added by-design
/// @tparam num The type of numeration for the rows and columns
template< typename CellT, typename num = num_non_default >
class Table 
    :   protected std::map< num, std::map< num, CellT* > >
{
public:
    /// @brief The type that is saved as pointer in the cells 
    typedef CellT cell_t;
    /// @brief The type of all rows
    typedef std::map< num, CellT* > row_t;
    /// @brief The type that a column has
    typedef std::pair< num, CellT* > col_t;
    /// @brief The numeric type with which the rows and columns are enumerated
    typedef num num_t;

    /// @brief Get or assign a row with the `table[row_number]` operator
    /// @param row The number of row to access
    /// @return An assignable reference to the row
    row_t& operator[]( const num_t row ) {
        try {
            return this->at( row );
        } catch ( std::out_of_range& oor ) {
            row_t* row_new = new row_t();
            this->at( row ) = *row_new;
            return this->at( row );
        }
    }

    /// @brief Get or assign a cell's value by reference
    /// @param row The number of the row to access
    /// @param column The number of the column to access
    /// @return An assignable reference to a cell
    cell_t& at( const num_t row, const num_t column ) {
        row_t& cols = (*this)[row];
        try {
            return std::make_pair( column, *(cols[column]) );
        } catch ( std::out_of_range& oor ) {
            cols[column] = nullptr;
            return std::make_pair( column, *(cols[column]) );
        }
    }

    /// @brief Get a complete column as `std::vector< col_t >`
    /// @param col The number of the column to access
    /// @return A vector with all values found in the column `col`
    std::vector< col_t > atCol( const num_t col ) {
        std::vector< col_t >* columns
            = new std::vector< col_t >();
        for ( row_t& cols : (*this) ) {
            for ( typename row_t::iterator it = cols.cbegin()
                ; it != cols.cend(); ++it
            ) {
                if ( it->first == col ) {
                    columns.push_back( std::make_pair( it->first, it->second ) );
                }
            }
        }

        return *columns;
    }

    /// @brief Get a complete row as `std::vector< col_t >`
    /// @param row The number of the row to access
    /// @return A vector with all values found in the row `row`
    std::vector< col_t > atRow( const num_t row ) {
        std::vector< col_t >* rows
            = new std::vector< col_t >();
        
        for ( row_t& cols : (*this) ) {
            for ( typename row_t::iterator it = cols.cbegin()
                ; it != cols.cend(); ++it
            ) {
                if ( it->first == row ) {
                    rows.push_back( row, it->second );
                }
            }
        }

        return *rows;
    }

    /// @brief Constructor.
    Table()
        :   std::map< num, std::map< num, CellT* > >()
    {}
};


/// @brief A `Table< CellT, num >` that is able to save objects for the row's and column's heading
/// @tparam ColT The type to be saved for the column's heading, no pointer allowed here, the pointer is added by-design
/// @tparam RowT The type to be saved for the row's heading, no pointer allowed here, the pointer is added by-design
/// @tparam CellT The type to be saved for the cells, no pointer allowed here, the pointer is added by-design
/// @tparam num The number type to enumerate the table with
template< typename ColT, typename RowT, typename CellT, typename num = num_non_default >
class VariateTable
    :   public Table< CellT, num >
{
public:
    /// @brief The type of the column's enumerational heading
    typedef ColT col_content_t;
    /// @brief The type of the row's enumerational heading
    typedef RowT row_content_t;
    /// @brief The type of the field's content
    typedef CellT cell_content_t;

protected:
    /// @brief The columns's heading objects
    std::vector< ColT* > m_columns;
    /// @brief The row's heading objects
    std::vector< RowT* > m_rows;

    /// @brief Constructor.
    VariateTable()
        :   Table< CellT, num >()
        ,   m_columns()
        ,   m_rows()
    {}

public:
    using Table< CellT, num >::operator[];
    using Table< CellT, num >::at;

    /// @brief Search the table by the row's and column's object
    /// @param row A pointer-to-object of the type ´row_t` that is suggested as row in this table
    /// @param col A pointer-to-object of the type ´col_t` that is suggested as column in this table
    /// @return An assignable reference to a cell
    cell_content_t& at( row_content_t* row, col_content_t* col) {
        unsigned int pos_col = 0;
        for ( col_content_t* colT : this->m_columns ) {
            if ( colT == col ) break;
            ++pos_col;
        }
        unsigned int pos_row = 0;
        for ( row_content_t* rowT : this->m_rows ) {
            if ( rowT == row ) break;
            ++pos_row;
        }
        return this->at( pos_row, pos_col );
    }

    
    /// @brief Search the rows of this table by the row's object
    /// @param row A pointer-to-object of the type ´row_t` that is suggested as row in this table
    /// @return A vector with all values found in the row `row`
    std::vector< cell_content_t > atRow( row_content_t* row ) {
        unsigned int pos_row = 0;
        for ( row_content_t* rowT : this->m_rows ) {
            if ( rowT == row ) break;
            ++pos_row;
        }
        return this->atRow( pos_row );
    }

    /// @brief Search the columns of this table by the column's object
    /// @param col A pointer-to-object of the type ´col_t` that is suggested as column in this table
    /// @return A vector with all values found in the column `col`
    std::vector< cell_content_t > atCol( col_content_t* col ) {
        unsigned int pos_col = 0;
        for ( col_content_t* colT : this->m_columns ) {
            if ( colT == col ) break;
            ++pos_col;
        }
        return this->atCol( pos_col );
    }
};


template< class GT, class NodeT >
concept is_graph = requires ( GT* gt ){
    std::is_base_of< passenger::Graph< NodeT >, GT >::value;
};


/// @brief A graph that is represented with an additional table for e.g. search and way alghorithms.
/// @tparam GT Your graph's type, e.g. `passengercpp::Graph` from `graph.hpp`
/// @tparam NodeT The type of all nodes in the graph
/// @tparam EdgeT The type of all edges of the graph
/// @tparam num The number type with which the table is numbered
/// @note The column represents all incoming edges to a node, the row all outgoing edges of a node.
template< class GT, class NodeT, class EdgeT, typename num = num_non_default >
    requires is_graph< GT, NodeT >
class GraphTable
    :   public GT
    ,   public VariateTable< NodeT, NodeT, EdgeT >
{
public:
    /// @brief Your graph's type
    typedef GT graph_t;
    /// @brief The variate table type that is used and inherited for this class
    typedef VariateTable< NodeT, NodeT, EdgeT > variate_table_t;
    /// @brief The type of all nodes in the graph
    typedef NodeT node_t;
    /// @brief The type of all edges of the graph
    typedef EdgeT edge_t;
    /// @brief The type of this class
    typedef GraphTable< GT, NodeT, EdgeT, num > self_t;

    /// @brief A simple map that admires the edges to their nodes that they are heading to
    class OutgoingMap
        :   public std::map< EdgeT*, NodeT* >
    {
    public:
        /// @brief The node where all this edges start
        NodeT* node_start;
        
        /// @brief Constructor
        /// @param vec A vector with all edges as pair with their targeting number (the number of the targeting node's column)
        /// @param start_node The node where all this edges start
        OutgoingMap( std::vector< std::pair< num, EdgeT* > >& vec
                   , NodeT* start_node )
            :   std::map< EdgeT*, NodeT* >()
            ,   node_start( start_node )
        {
            for ( std::pair< num, EdgeT* >& edge : vec ) {
                this->push_back( std::make_pair( edge.second, this->m_columns.at( edge.first ) ) );
            }
        }
    };

    /// @brief A simple map that admmires the edgge to their node that they are coming from
    class IncomingMap
        :   public std::map< EdgeT*, NodeT* >
    {
    public:
        /// @brief The node where all edges and
        NodeT* node_end;

        /// @brief Constructor
        /// @param vec A vector with all edges as pair with their starting-from number (the number of the outgoing node's row)
        /// @param start_node The node where all this edges start
        IncomingMap( std::vector< std::pair< num, EdgeT* > >& vec
                   , NodeT* end_node )
            :   std::map< EdgeT*, NodeT* >()
            ,   node_end( end_node )
        {
            for ( std::pair< num, EdgeT* >& edge : vec ) {
                this->push_back( std::make_pair( edge.second, this->m_rows.at( edge.first ) ) );
            }
        }
    };

    /// @brief Get a reference to a `OutgoingMap` for one node and all outgoing edges
    /// @param node The node, where all outgoing edges of the `OutgoingMap` start
    OutgoingMap& make_outgoingmap( const NodeT* node ) {
        std::vector< typename variate_table_t::cell_t > outgoings
            = this->atRow( node );
        return *(new OutgoingMap( outgoings, node ));
    }

    /// @brief Get a reference to a `IncomingMap` for one node and all incoming edges
    /// @param node The node, where all outgoing edges of the `IncomingMap` end
    IncomingMap& make_incomingmap( const NodeT* node ) {
        std::vector< typename variate_table_t::cell_t > incomings
            = this->atCol( node );
        return *(new IncomingMap( incomings, node ) );
    }

    /// @brief The class that handles routes
    class Route {
        /// @brief An index of a node or an edge
        typedef unsigned int Index;
        /// @brief An index pair, e.g. for accessing a table
        class IndexPair : public std::pair< Index, Index > {};

        /// @brief The type of `Route::Edge`
        typedef IndexPair Edge;
        /// @brief The type of `Route::Node`
        typedef Index Node;

        /// @brief A route along nodes
        class AlongNodes : public std::vector< Route::Node > {};
        /// @brief A route along edges
        class AlongEdges : public std::vector< Route::Edge > {};

        /// @brief Test, if two routes along nodes are the same
        /// @param route A pointer to an `AlongRoutes` object
        /// @param another A pointer to an `AlongRoutes` object
        /// @return True, if both objects do not differ. False, when there is a differrence at at least one node
        static bool is_same( const AlongNodes* route, const AlongNodes* another ) {
            if ( route == nullptr || another == nullptr ) return false;
            // Test, if they are new (The new routes are one shorter)
            unsigned int nr = 0;
            for ( unsigned int i = 1; i < another.size(); i++ ) {
                Route::Node old_equiv_idx_node = another->at( i );
                Route::Node new_equiv_idx_node = route->at( nr );
                if ( old_equiv_idx_node != new_equiv_idx_node ) {
                    return false;
                }
            }

            return true;
        }

        /// @brief Find a route in the graph to a node starting at another node
        /// @param to The node where the route is to be ending
        /// @param from The node where the route is to be starting
        /// @param graph The graph as object pointer
        /// @return `nullptr`, if there is no route, a valid `AlongNodes` object otherwise
        static AlongNodes* findRouteToFrom( const NodeT* to, const NodeT* from, const self_t* graph ) {
            
            AlongNodes* route = new AlongNodes();

            OutgoingMap& outgoings = make_outgoingmap( to );
            IncomingMap& incomings_start = make_incomingmap( from );

            NodeT* start = from;

            for ( typename OutgoingMap::const_iterator out_it : outgoings ) {

                route->insert( route->begin(), Route::Node( out_it->first ) );

                AlongNodes* route_current = nullptr;

                // Test all nodes, if they can have a route to start
                typename IncomingMap::const_iterator ins_it;
                for ( typename IncomingMap::const_iterator in_it : incomings_start ) {

                    route_current = Route::findRouteToFrom(
                                      graph->m_columns[ in_it->first ]
                                    , graph->m_rows[ out_it->first ]
                                    , graph
                    );

                    ins_it = in_it;

                    if  ( route_current != nullptr ) break;
                    else if ( route_current == nullptr ) continue;
                    else if ( route_current[0] == from && route_current[route_current->size()-1] == to ) return route_current;
                    else route->insert( route->begin(), route_current->begin(), route_current->end() );
                }

                if ( route_current == nullptr ) { // If no route was found, we try to reach from from one node before `to`

                    NodeT* node_before_to = graph->atRow( ins_it->first );
                    route_current = Route::findRouteToFrom( node_before_to, from );

                    // If a route was found until here, we return it and add the node to (see note at parent if-clause)
                    if ( route_current != nullptr ) {
                        route_current->push_back( to );
                        return route_current;
                    } else continue;
                }
            }

            return nullptr;
        }

        /// @brief Find all routes in the graph to a node starting at another node
        /// @param to The node where the route is to be ending
        /// @param from The node where the route is to be starting
        /// @param graph The graph as object pointer
        /// @return `nullptr`, if there is no route, a valid `std::vector< AlongNodes >` object otherwise
        static std::vector< AlongNodes* >* findRoutesToFrom( const NodeT* to, const NodeT* from, const self_t* graph ) {
            std::vector< AlongNodes* >* routes = new std::vector< AlongNodes* >();

            if ( to != from ) {

                AlongNodes* node_route = Route::findRouteToFrom( to, from, graph );
                routes.push_back( node_route );

                if ( node_route == nullptr ) return nullptr;

                for ( typename Route::Node node : *node_route ) {
                    
                    NodeT* node_to_new = graph->atRow( node_route->at(0) );
                    //NodeT* node_from_new = graph->atRow( node_route->at(node_route->size()-1) );

                    std::vector< AlongNodes* >& new_routes
                        = Route::findRoutesToFrom( node_to_new, node_to_new, graph );

                    if ( new_routes.size() == 1 ) continue;

                    for  ( AlongNodes* route : new_routes ) {
                        // Test, if they are new (The new routes are one shorter)
                        bool route_same = true;
                        unsigned int nr = 0;
                        for ( unsigned int i = 1; i < node_route.size(); i++ ) {
                            Route::Node old_equiv_idx_node = node_route->at( i );
                            Route::Node new_equiv_idx_node = route->at( nr );
                            if ( old_equiv_idx_node != new_equiv_idx_node ) {
                                route_same = false;
                                break;
                            } else ++nr;
                        }

                        if ( ! route_same ) routes.push_back( route );
                    }

                    OutgoingMap& outgoings_to = make_outgoingmap( to );
                    if ( outgoings_to.size() == 1 ) continue;
                    for ( typename OutgoingMap::iterator to_out : outgoings_to ) {
                        std::vector< AlongNodes* >* shorter_routes
                            = Route::findRoutesToFrom( to_out->second, from, graph );
                        
                        if ( shorter_routes != nullptr ) routes += shorter_routes;
                    }

                }

                return routes;
            }

            return nullptr;
        }

        /// @brief Find the shortest route in the graph to a node starting at another node
        /// @param to The node where the route is to be ending
        /// @param from The node where the route is to be starting
        /// @param graph The graph as object pointer
        /// @return `nullptr`, if there is no route, a valid `AlongNodes` object otherwise
        static AlongNodes* findRouteShortestToFrom( const NodeT* to, const NodeT* from, const self_t* graph ) {
            std::vector< AlongNodes* >& routes = Route::findRoutesToFrom( to, from, graph );

            unsigned int shortest_i = 0;
            unsigned int current_shortest = 0;
            for ( unsigned int i = 0; i < routes.size(); i++ ) {
                AlongNodes* route = routes.at( i );

                if ( current_shortest == 0 || current_shortest > route->size() ) {
                    current_shortest = route->size();
                    shortest_i = i;
                }
            }

            return routes[shortest_i];
        }

        /// @brief Convert an `AlongNodes` object pointer to a `AlongEdges` object pointer
        /// @param node_route The `AlongNodes` object pointer to convert from
        /// @param graph The graph as object pointer
        /// @return A converted `AlongEdges` object pointer
        static AlongEdges* alongNodesToAlongEdges( AlongNodes* node_route, const self_t* graph ) {

            AlongEdges* edge_route = new AlongEdges();

            unsigned int rn = 0;
            for ( typename Route::Node node : *node_route ) {

                if ( rn == 0 ) {
                    edge_route->push_back( Route::Edge( node_route->at( rn ), node_route->at( rn ) ) );
                    ++rn;
                    continue;
                } else if ( rn == node_route->size() - 1 ) {
                    edge_route->push_back( Route::Edge( node_route->at( rn ), node_route->at( rn ) ) );
                    return edge_route;
                }
                
                edge_route->push_back( Route::Edge( node, node_route->at( rn+1 ) ) );
                ++rn;
            }

            return edge_route;
        }
    };

    /// @brief The type of routes along edges in this `GraphTable< GT, NodeT, EdgeT, num >`
    typedef Route::AlongEdges edge_route_t;
    /// @brief The type of routes along nodes in this `GraphTable< GT, NodeT, EdgeT, num >`
    typedef Route::AlongNodes node_route_t;
protected:


public:
    /// @brief Constructor
    /// @param start The start node as pointer
    /// @param end The end node as pointer
    GraphTable( NodeT* start = nullptr, NodeT* end = nullptr )
        :   GT( start, end )
        ,   VariateTable< NodeT, NodeT, EdgeT >()
    {}

    using variate_table_t::at;

    /// @brief The type of unbalanced trees with nodes in this `GraphTable< GT, NodeT, EdgeT, num >`
    typedef Tree< NodeT, NodeT > tree_t;

    /// @brief Copy constructor
    /// @param custom_graph Your custom graph that thus needs to be inherited from `passenger::Graph< NodeT >`
    GraphTable( GT& custom_graph )
        :   GT( custom_graph.start, custom_graph.end )
        ,   VariateTable< NodeT, NodeT, EdgeT >()
    {
        NodeT* node_start = custom_graph.start;
        NodeT* node_end = custom_graph.end;

        while ( node_start != custom_graph.end ) {
            OutgoingMap& outgoing = this->make_outgoingmap( node_start );
        
            for ( typename OutgoingMap::iterator outIt = outgoing.begin()
                ; outIt != outgoing.end(); ++outIt
            ) {
                node_end = outIt->first;
                IncomingMap& incoming = this->make_incomingmap( node_end );

                for ( typename IncomingMap::iterator inIt = incoming.begin()
                    ; inIt != incoming.end(); ++inIt
                ) {
                    this->at( node_end, inIt->second ) = inIt->first;
                    node_start = inIt->first;
                }
            }

            
        }

        node_start = nullptr;
        node_end = custom_graph.end;
        IncomingMap& incoming = this->make_incomingmap( node_end );

        while ( node_end != custom_graph.start ) {
            for ( typename IncomingMap::iterator inIt = incoming.begin()
                ; inIt != incoming.end(); ++inIt
            ) {
                node_start = inIt->second;
                OutgoingMap& outgoing = this->make_outgoingmap( node_start );
                
                for ( typename OutgoingMap::iterator outIt = outgoing.start()
                    ; outIt != outgoing.end(); ++outIt
                ) {
                    this->at( node_start, inIt->second ) = outIt->first;

                    node_end = outIt->first;
                }
            }
        }
    }

    /// @brief Get a tree with at least one possible route from a start node to a target end node
    /// @param node_from The node to start with
    /// @param node_to The node to end with
    /// @return A tree with at least one plausible route from `tree.root == node_from` to the tree's
    ///         leafs as `node_to`. Returns an empty tree or a tree with `tree.leafs.size() == 0`, if
    ///         no route was found
    tree_t& make_tree_from_to( const NodeT* node_from, const NodeT* node_to ) {

        OutgoingMap& outgoings_start = this->make_outgoingmap( node_from );
        typename OutgoingMap::iterator nodes_start_it = outgoings_start.begin();
        NodeT* node_current = nodes_start_it->second;

        tree_t* tree = new tree_t( node_from );
        if ( node_current == node_to ) return *tree;

        IncomingMap& incoming = this->make_incomingmap( node_to );

        while ( node_current != node_to ) {
            bool has_leaf_to_node_to = false;

            for ( typename IncomingMap::iterator in_end_it = incoming.begin()
                ; in_end_it != incoming.end(); ++in_end_it
            ) {
                bool is_route = false;
                node_route_t* node_route = Route::findRouteToFrom( in_end_it, node_current );

                if ( (*node_route)[node_route->size()-1] == node_current ) is_route = true;

                if ( ! is_route ) {
                    tree_t& tree_new = this->make_tree_from_to( node_current, in_end_it->second );

                    tree.add( node_current, tree_new );
                } else {
                    typename tree_t::node_t* current_tree_node = tree.find( (*node_route)[0] );

                    if ( current_tree_node == nullptr ) current_tree_node = tree.root;

                    for ( NodeT* node : *node_route ) {
                        
                        typename tree_t::node_t* tree_node_new
                            = new tree_t::node_t( node );
                        tree.add( current_tree_node, tree_node_new );

                        if ( node == node_to ) {
                            typename tree_t::leaf_t* new_leaf
                                = new tree_t::leaf_t( node_to );
                            tree.add( new_leaf, current_tree_node );
                        }

                        current_tree_node = tree_node_new;
                    }
                }

                if ( ! has_leaf_to_node_to ) continue;

                if ( in_end_it == incoming.node_end ) break;
            }

            ++nodes_start_it;

            if ( nodes_start_it == incoming.end() ) node_current = node_to;
            else node_current = nodes_start_it->second;
        }

        return *tree;
    }

    /// @brief Get a tree with all possible routes from a start node to a target end node
    /// @param node_from The node to start with
    /// @param node_to The node to end with
    /// @return A tree with all plausible routes from `tree.root == node_from` to the tree's
    ///         leafs as `node_to`. Returns an empty tree or a tree with `tree.leafs.size() == 0`, if
    ///         no route was found
    tree_t& make_complete_tree_from_to( const NodeT* node_from, const NodeT* node_to ) {

        OutgoingMap& outgoings_start = this->make_outgoingmap( node_from );
        typename OutgoingMap::iterator nodes_start_it = outgoings_start.begin();
        NodeT* node_route = nodes_start_it->second;

        tree_t* tree = new tree_t( node_from );
        if ( node_route == node_to ) return *tree;

        IncomingMap& incomings_end = this->make_incomingmap( node_to );

        while ( node_route != node_to ) {

            NodeT* next = node_route;
            OutgoingMap& outgoings = this->make_outgoingmap( node_route );

            for ( typename OutgoingMap::iterator node = outgoings.begin()
                ; node != outgoings.end(); ++node
            ) {
                tree_t& tree_new = this->make_tree_from_to( node_route, node );

                if ( tree_new.leafs.size() == 0 ) continue;
                else next = node;

                tree.add( *node,  tree_new );
            }

            if ( node_route == node_to ) continue;
            else if ( next == node_to ) {
                ++nodes_start_it;
                node_route = nodes_start_it->second;
                continue;
            } else node_route = node_to;
        }

        return *tree;
    }

    /// @brief Get a possible route from a start node to a target end node
    /// @param start_node The node to start with
    /// @param end_node The node to end with
    /// @param shortest If set to true, the really shortest route is selected from all plausible routes. If left to false,
    ///                 then the shortest route is selected from the few first routes found by `make_tree_from_to()`
    /// @return A `node_route_t`, which thus means a pointer to `AlongNodes` with the selected route. If
    ///         `shortest == true`, the shortest route is returned
    node_route_t* make_route_along_nodes( const NodeT* start_node, const NodeT* end_node, bool shortest = false ) {
        node_route_t* node_route = new node_route_t();

        tree_t& tree;
        if ( shortest )
            tree_t& tree = this->make_complete_tree_from_to( start_node, end_node );
        else
            tree_t& tree = this->make_tree_from_to( start_node, end_node );
        
        // Select the shortest path
        unsigned int shortest_il = 0;
        unsigned int shortest_v = 0;
        unsigned int il = 0;
        typename tree_t::leaf_t& leaf;
        for ( typename tree_t::leaf_t& leaf : tree.leafs ) {
            const unsigned int depth_to_leaf = tree.depthTo( leaf );
            if ( shortest_v == 0 || depth_to_leaf < shortest_il ) {
                shortest_il = il;
                shortest_v = depth_to_leaf;
            }

            il++;
        }

        typename tree_t::node_t* node = leaf.parent;
        node_route.push_back( leaf.content );
        while ( node != tree.root ) {
            node_route.push_back( node );
            node = node.parent;
        }

        node_route.push_back( tree.root );

        return node_route;
    }

    /// @brief Get a possible route from a start node to a target end node
    /// @param start_node The node to start with
    /// @param end_node The node to end with
    /// @param shortest If set to true, the really shortest route is selected from all plausible routes. If left to false,
    ///                 then the shortest route is selected from the few first routes found by `make_tree_from_to()`
    /// @return A `edge_route_t`, which thus means a pointer to `AlongEdges` with the selected route. If
    ///         `shortest == true`, the shortest route is returned
    edge_route_t* make_route_along_edges( const NodeT* start_node, const NodeT* end_node, bool shortest = false ) {
        node_route_t* node_route = this->make_route_along_nodes( start_node, end_node, shortest );

        return Route::alongNodesToAlongEdges( node_route );
    }

    // TODO: Subgraphs
    // self_t& make_subgraph( const node_route_t* node_route, bool route_only = false ) {}
    // self_t& make_subgraph( const edge_route_t* edge_route, bool route_only = false ) {}
    // self_t& make_subgraph( const NodeT* start_node, const NodeT* end_node, bool route_only = false ) {}
};

} // namespace passenger