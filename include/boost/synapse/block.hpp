//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_66512920DFE44D02ACFDEF2013B16688
#define UUID_66512920DFE44D02ACFDEF2013B16688

#include <boost/synapse/synapse_detail/common.hpp>
#include <boost/synapse/synapse_detail/weak_store.hpp>

namespace
boost
    {
    namespace
    synapse
        {
        class blocker;

        template <class Signal,class Emitter>
        typename synapse_detail::enable_if<is_signal<Signal>::value,shared_ptr<blocker> >::type
        block( weak_ptr<Emitter> const & );

        template <class Signal,class Emitter>
        typename synapse_detail::enable_if<is_signal<Signal>::value,shared_ptr<blocker> >::type
        block( shared_ptr<Emitter> const & );

        template <class Signal,class Emitter>
        typename synapse_detail::enable_if<is_signal<Signal>::value,shared_ptr<blocker> >::type
        block( Emitter * );

        namespace
        meta
            {
            template <class Signal>
            struct
            blocked
                {
                typedef blocked<Signal>(*type)( blocker &, bool is_blocked );
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
            shared_ptr<blocker> block_( emitter_blocked_t * &, weak_ptr<blocked_list> &, weak_store const &, int(*)(blocker &,bool) );
            template <class Signal> int emit_meta_blocked( blocker &, bool );
            ////////////////////////////////////////////////////////
            template <class Signal,class Emitter>
            shared_ptr<blocker>
            block_fwd( weak_ptr<Emitter> const & e, Emitter * px )
                {
                return block_(emitter_blocked_<Signal>(),get_blocked_list<Signal>(),weak_store(e,px),&emit_meta_blocked<Signal>);
                }
            }
        template <class Signal,class Emitter>
        typename synapse_detail::enable_if<is_signal<Signal>::value,shared_ptr<blocker> >::type
        block( weak_ptr<Emitter> const & e )
            {
            return synapse_detail::block_fwd<Signal,Emitter>(e,e.lock().get());
            }
        template <class Signal,class Emitter>
        typename synapse_detail::enable_if<is_signal<Signal>::value,shared_ptr<blocker> >::type
        block( shared_ptr<Emitter> const & e )
            {
            return synapse_detail::block_fwd<Signal,Emitter>(weak_ptr<Emitter>(e),e.get());
            }
        template <class Signal,class Emitter>
        typename synapse_detail::enable_if<is_signal<Signal>::value,shared_ptr<blocker> >::type
        block( Emitter * e )
            {
            return synapse_detail::block_fwd<Signal,Emitter>(shared_ptr<Emitter>(shared_ptr<void>(),e),e);
            }
        } 
    }

#endif
