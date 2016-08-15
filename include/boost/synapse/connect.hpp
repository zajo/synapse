//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_2CCEDE28DF2411E4809EBF291D5D46B0
#define UUID_2CCEDE28DF2411E4809EBF291D5D46B0

#include <boost/synapse/emit.hpp>
#include <boost/synapse/synapse_detail/weak_store.hpp>

namespace
boost
    {
    namespace
    synapse
        {
        class connection;

        template <class Signal,class Emitter>
        shared_ptr<connection>
        connect( weak_ptr<Emitter> const &, function<typename signal_traits<Signal>::signature> const &,
            weak_ptr<void const> const & connection_lifetime=weak_ptr<void const>() );

        template <class Signal,class Emitter>
        shared_ptr<connection>
        connect( shared_ptr<Emitter> const &, function<typename signal_traits<Signal>::signature> const &,
            weak_ptr<void const> const & connection_lifetime=weak_ptr<void const>() );

        template <class Signal,class Emitter>
        shared_ptr<connection>
        connect( Emitter *, function<typename signal_traits<Signal>::signature> const &,
            weak_ptr<void const> const & connection_lifetime=weak_ptr<void const>() );

        namespace
        meta
            {
            shared_ptr<void const> emitter();
            namespace
            connect_flags
                {
                unsigned const connecting=1;
                unsigned const first_for_this_emitter=2;
                unsigned const last_for_this_emitter=4;
                }
            template <class Signal>
            struct
            connected
                {
                typedef connected<Signal>(*type)( connection &, unsigned connect_flags );
                };
            }
        }
    }

//Implementation details below.

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
            shared_ptr<connection> connect_( emit_t * &, weak_ptr<connection_list> &, weak_store const &, shared_ptr<void const> const &, weak_ptr<void const> const &, int(*)(connection &,unsigned) );
            template <class Signal> weak_ptr<connection_list> & get_connection_list();
            template <class Signal> int emit_meta_connected( connection &, unsigned );
            ////////////////////////////////////////////////////////
            template <class Signal,class Emitter>
            shared_ptr<connection>
            connect_fwd( weak_ptr<Emitter> const & e, Emitter * px, function<typename signal_traits<Signal>::signature> const & fn, weak_ptr<void const> const & connection_lifetime )
                {
                return connect_(
					synapse_detail::emit_<Signal>(),
                    get_connection_list<Signal>(),
                    weak_store(e,px),
                    shared_ptr<void const>(new function<typename signal_traits<Signal>::signature>(fn)),
                    connection_lifetime,
                    &emit_meta_connected<Signal>);
                }
            }
        template <class Signal,class Emitter>
        shared_ptr<connection>
        connect( weak_ptr<Emitter> const & e, function<typename signal_traits<Signal>::signature> const & fn, weak_ptr<void const> const & connection_lifetime )
            {
            return synapse_detail::connect_fwd<Signal,Emitter>(e,e.lock().get(),fn,connection_lifetime);
            }
        template <class Signal,class Emitter>
        shared_ptr<connection>
        connect( shared_ptr<Emitter> const & e, function<typename signal_traits<Signal>::signature> const & fn, weak_ptr<void const> const & connection_lifetime )
            {
            return synapse_detail::connect_fwd<Signal,Emitter>(weak_ptr<Emitter>(e),e.get(),fn,connection_lifetime);
            }
        template <class Signal,class Emitter>
        shared_ptr<connection>
        connect( Emitter * e, function<typename signal_traits<Signal>::signature> const & fn, weak_ptr<void const> const & connection_lifetime )
            {
            return synapse_detail::connect_fwd<Signal,Emitter>(shared_ptr<Emitter>(shared_ptr<void>(),e),e,fn,connection_lifetime);
            }
        } 
    }

#endif
