//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_A098543F9C6D4BEC8CE41B576AA69BA6
#define UUID_A098543F9C6D4BEC8CE41B576AA69BA6

#include <boost/synapse/signal_traits.hpp>
#include <boost/weak_ptr.hpp>

namespace
boost
    {
    namespace
    synapse
        {
        namespace
        synapse_detail
            {
            struct connection_list;
            struct blocked_list;
            struct emit_binder_base;
            ///////////////////////////////////////////////////////////////////
            template <class Signal>
            weak_ptr<connection_list> &
            get_connection_list()
                {
                static weak_ptr<connection_list> cl;
                return cl;
                }
            typedef  int emit_t( weak_ptr<connection_list> const &, weak_ptr<blocked_list> const &, void const *, emit_binder_base const & );
            inline int emit_stub( weak_ptr<connection_list> const &, weak_ptr<blocked_list> const &, void const *, emit_binder_base const & ) { return 0; }
            inline
            emit_t * &
            emit_()
                {
                static emit_t * psyn=&emit_stub;
                return psyn;
                }
            ///////////////////////////////////////////////////////////////////
            template <class Signal>
            weak_ptr<blocked_list> &
            get_blocked_list()
                {
                static weak_ptr<blocked_list> cl;
                return cl;
                }
            typedef bool emitter_blocked_t( weak_ptr<blocked_list> const &, void const * );
            inline bool emitter_blocked_stub( weak_ptr<blocked_list> const &, void const * ) { return false; }
            inline
            emitter_blocked_t * &
            emitter_blocked_()
                {
                static emitter_blocked_t * pblk=&emitter_blocked_stub;
                return pblk;
                }
            ///////////////////////////////////////////////////////////////////
            }
        } 
    }

#endif
