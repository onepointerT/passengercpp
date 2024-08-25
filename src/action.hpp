
#pragma once

#include "config.hpp"
#include "graph_passenger.hpp"

#include <functional>
#include <string>
#include <vector>
#include <utility>


extern "C" {
/// @brief An action_function_t defines the type a of method for an action needs to have and the types for parameters and return type.
/// @param GraphPassenger A reference to an absolvent that is a specialiced class of `GraphPassenger` or `GraphPassenger` itself.
/// @param string The name of the `action_function_t` for synthetical sanity checks.
/// @returns A list of named result tuples with `(std::string* absolvents_name, std::string result)`
typedef typename std::vector< std::pair< std::string, std::string > > (*action_function_t)(GraphPassenger&, std::string);
/// @brief An action_function_t defines the type a of method for an action needs to have and the types for parameters and return type.
/// @param GraphPassenger A list with absolvents that are a specialiced class of `GraphPassengerNumber` or `GraphPassengerNumber` itself.
/// @param string The name of the `action_function_t` for synthetical sanity checks.
/// @returns A list of named result tuples with `(std::string* absolvents_name, std::string result)`
typedef typename std::vector< std::pair< std::string, std::string > > (*action_group_function_t)(std::vector< GraphPassenger& >, std::string);
}

/// @brief An Action function a container for a function, that is to be run on getting, need to absolve and coincidating 
///     (taking apart in) an action.
class ActionFunction {
protected:
    /// @brief The class ActionFunction stores one protected value of the type action_function_t for executing with the function 
    ///     `temlate< class absolverT > run()`.
    action_function_t m_func;

public:
    /// @brief The name of the function for similar lookups
    const std::string funcname;

    /// @brief The constructor of ActionFunction
    /// @param nameof_func The name of the function for similar lookups
    /// @param func An action_function_t for further execution on call to `temlate< class absolverT > run()`.
    ActionFunction( const std::string nameof_func, action_function_t func );

    /// @brief Execute the actional function with an absolving passenger. Will inform the absolvent by `GraphPassenger::absolvedActionFunction()`.
    /// @tparam absolverT The type of the absolving passenger, at least a specialization of the class `GraphPassenger`.
    /// @param absolver The absolver of the actional function
    /// @param nameof_func The name of the function to execute for further tests
    /// @return The return value of `GraphPassenger::absolvedActionFunction()` of the absolvent's instance.
    template< class absolverT >
    bool run( absolverT absolver, const std::string nameof_func ) {
        if ( nameof_func != funcname ) {
            return false;
        }

        parameter_list_t returned_parameters = m_func( absolver );
        return absolver->absolvedActionFunction( returned_parameters );
    };
};


/// @brief An `ActionFunction` that has a name
typedef typename std::pair< std::string, ActionFunction > named_action_function_t;

/// @brief An action that can be performed and where named functionals can be absolved.
/// @tparam GraphPassengerT The type of the graph passengers, at least a specialization of `GraphPassenger`.
template< class GraphPassengerT = GraphPassenger >
class Action {
protected:
    /// @brief A static list of `ActionFunctions` that are available to be absolved.
    static std::vector< ActionFunction > m_functions;

public:
    /// @brief Register an `ActionFunction` for absolvents at the static contingent of different functions (`m_functions`).
    ///     Sets the function instead of an existing, if a function with this name is already found.
    /// @param function The function of the type `action_function_t` for absolvents of action functions.
    /// @return Success or not.
    static bool register_function( const ActionFunction& function ) {
        for ( typename std::vector< ActionFunction >::iterator it = m_functions.begin()
            ; it != m_functions.end(); ++it
        ) {
            if ( it->funcname != function.funcname ) {
                continue;
            } else {
                return false;
            }
        }

        m_functions.push_back( function );
        return true;
    }

    /// @brief Register a list of functions. Works per function of the list `functions` like 
    ///     `Action< GraphPassengerT >::register_function()`.
    /// @param functions A list of action functions.
    /// @return The number of unsuccessful appliances (where `register_function()` returned false.)
    static unsigned int register_functions( const std::vector< ActionFunction > functions ) {
        unsigned int mistaken = 0;

        for ( typename std::vector< ActionFunction >::iterator it = functions.begin()
            ; it != functions.end(); ++it
        ) {
            if ( ! Action< GraphPassengerT >::register_function( *it ) ) {
                ++mistaken;
            }
        }

        return mistaken;
    }

    /// @brief Absolve a function with a funcname and get the passenger absolvent absolving the functional
    ///     `ActionFunction::run< GraphPassengerT >( passenger, funcname )`.
    /// @param funcname The name of the chosen function
    /// @param passenger The absolvent of the corresponding graph passengers.
    /// @return The return value of `ActionFunction::run()` or false, if the function could not be found.
    virtual bool run( const std::string funcname, const GraphPassengerT& passenger ) {
        for ( typename std::vector< ActionFunction >::iterator it = m_functions.begin()
            ; it != m_functions.end(); ++it
        ) {
            if ( it->funcname == funcname ) {
                return it->run< GraphPassengerT >( passenger, funcname );
            }
        }

        return false;
    }

    /// @brief The constructor of `Action`.
    /// @note Does not need to be called to register functions.
    Action() {}
};



template< class GraphPassengerT = GraphPassengerNumber<> >
class ActionGroupFunction
    :   public Action< GraphPassengerT >
{
protected:
    static std::vector< ActionGroupFunction > m_group_functions;

public:
    /// @brief Register an `ActionGroupFunction` for absolvents at the static contingent of different functions (`m_group_functions`).
    ///     Sets the function instead of an existing, if a function with this name is already found.
    /// @param function The function of the type `action_group_function_t` absolvents of action functions.
    /// @return Success or not.
    static bool register_group_function( const ActionGroupFunction& function ) {
        for ( typename std::vector< ActionGroupFunction >::iterator it = m_group_functions.begin()
            ; it != m_group_functions.end(); ++it
        ) {
            if ( it->funcname != function.funcname ) {
                continue;
            } else {
                return false;
            }
        }

        m_group_functions.push_back( function );
        return true;
    }

    /// @brief The constructor.
    /// @note An instance does not need to be called to register functions.
    ActionGroupFunction() {}
};


/// @brief Initializer for the function list with all available action functions.
/// @tparam GraphPassengerT The type of absolvent, a passenger in the graph.
template<class GraphPassengerT> std::vector<ActionFunction> Action<GraphPassengerT>::m_functions
    = std::vector<ActionFunction>();



/// @brief A specialization of `Action` with functions for grouped graph passenger absolvents.
/// @tparam GraphPassengerT 
template< class GraphPassengerT = GraphPassengerNumber<> >
class ActionPassengerGroup
    :   public Action< GraphPassengerT >
{
protected:
    /// @brief A static list of `ActionFunctions` that are available to be absolved.
    static std::vector< ActionFunction > m_group_functions;

public:
    /// @brief A list of absolving graph passengers of the type `GraphPassengerT`
    typedef typename std::vector< GraphPassengerT& > passenger_group_t;

    /// @brief The constructor of `Action`.
    /// @note Does not need to be called to register functions.
    ActionPassengerGroup()
        :   Action< GraphPassengerT >()
    {}


    /// @brief Absolve a function with a funcname and get the passenger absolvents absolving the functional
    ///     `ActionFunction::run< GraphPassengerT >( passenger, funcname )`.
    /// @param funcname The name of the chosen function
    /// @param passenger The absolvents of the corresponding graph passengers.
    /// @return The return value of `ActionFunction::run()` or false, if the function could not be found.
    virtual bool run( const std::string funcname, const passenger_group_t& passenger_group ) {
        for ( typename std::vector< ActionFunction >::iterator it = m_group_functions.begin()
            ; it != m_group_functions.end(); ++it
        ) {
            if ( it->funcname == funcname ) {
                return it->run< const passenger_group_t& >( passenger_group, funcname );
            }
        }

        return false;
    }
};



/// @brief Initializer for the function list with all available action functions.
/// @tparam GraphPassengerT The type of absolvent, a passenger in the graph.
template<class GraphPassengerT> std::vector<ActionFunction> ActionPassengerGroup<GraphPassengerT>::m_group_functions
    = std::vector<ActionGroupFunction<GraphPassengerT>>();
