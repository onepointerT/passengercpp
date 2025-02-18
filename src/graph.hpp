
#pragma once

#include <vector>

#include "passenger.hpp"


namespace passenger {


/// @brief The simple graph's class. Stores all nodes of the type `NodeT*` and points also to a single start and a single end node.
/// @tparam NodeT The nodes that appear inside of the graph.
template< class NodeT >
class Graph
    :   public std::vector< NodeT* >
{
protected:
    /// @brief A pointer to the start node.
    NodeT* m_start;
    /// @brief A pointer to the end node.
    NodeT* m_end;

public:
    /// @brief The constructor of a graph.
    /// @param start The start node of the type `NodeT*`.
    /// @param end The end node of the type `NodeT*`.
    Graph( NodeT* start = nullptr, NodeT* end = nullptr )
        :   std::vector< NodeT* >()
        ,   m_start( start )
        ,   m_end( end )
    {}

    /// @brief Reset the start node to the value of `node`.
    /// @param node The start node of the type `NodeT*`.
    void reset_start( NodeT* node ) {
        m_start = node;
    }

    /// @brief Reset the start node to the value of `node`.
    /// @param node The end node of the type `NodeT*`.
    void reset_end( NodeT* node ) {
        m_end = node;
    }

    /// @brief Get the start node of the type `NodeT*`.
    /// @return The start node.
    NodeT* get_start() const {
        return m_start;
    }

    /// @brief Get the end node of the type `NodeT*`.
    /// @return The end node.
    NodeT* get_end() const {
        return m_end;
    }

    /// @brief Send a traveller marker to the start node.
    /// @param passenger A graph passenger of the type `NodeT::node_passenger_t`.
    /// @return The return value of `NodeT::arrive()` function.
    bool send_to_start( typename NodeT::node_passenger_t& passenger ) {
        this->get_start()->arrive( passenger );
    }
};

} // namespace passenger
