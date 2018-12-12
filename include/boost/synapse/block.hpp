//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_66512920DFE44D02ACFDEF2013B16688
#define UUID_66512920DFE44D02ACFDEF2013B16688

#include <boost/synapse/synapse_detail/common.hpp>
#include <boost/synapse/synapse_detail/weak_store.hpp>

namespace boost { namespace synapse {

    class blocker;

    template <class Signal,class Emitter>
    shared_ptr<blocker> block( Emitter * );

    template <class Signal,class Emitter>
    shared_ptr<blocker> block( weak_ptr<Emitter> const & );

    template <class Signal,class Emitter>
    shared_ptr<blocker> block( shared_ptr<Emitter> const & );

    namespace meta
    {
        template <class Signal>
        struct blocked
        {
            typedef blocked<Signal>(*type)( blocker &, bool is_blocked );
        };
    }

} }

//Implementation details below.

namespace boost { namespace synapse {

    namespace synapse_detail
    {
        shared_ptr<blocker> block_( shared_ptr<thread_local_signal_data> const &, weak_store &&, int(*)(blocker &,bool) );
        template <class Signal> int emit_meta_blocked( blocker &, bool );
        ////////////////////////////////////////////////////////
        template <class Signal,class Emitter>
        shared_ptr<blocker> block_fwd( weak_store && e )
        {
            return block_(get_thread_local_signal_data<Signal>(true),std::move(e),&emit_meta_blocked<Signal>);
        }
    }

    template <class Signal,class Emitter>
    shared_ptr<blocker> block( Emitter * e )
    {
        return synapse_detail::block_fwd<Signal,Emitter>(e);
    }

    template <class Signal,class Emitter>
    shared_ptr<blocker> block( weak_ptr<Emitter> const & e )
    {
        return synapse_detail::block_fwd<Signal,Emitter>(e);
    }

    template <class Signal,class Emitter>
    shared_ptr<blocker> block( shared_ptr<Emitter> const & e )
    {
        return synapse_detail::block_fwd<Signal,Emitter>(e);
    }

} }

#endif
