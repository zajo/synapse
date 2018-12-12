//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_2CCEDE28DF2411E4809EBF291D5D46B0
#define UUID_2CCEDE28DF2411E4809EBF291D5D46B0

#include <boost/synapse/emit.hpp>
#include <boost/synapse/synapse_detail/weak_store.hpp>

namespace boost { namespace synapse {

    class connection;

    template <class Signal,class Emitter,class F>
    shared_ptr<connection> connect( Emitter *, F );

    template <class Signal,class Emitter,class Receiver,class F>
    shared_ptr<connection> connect( Emitter *, Receiver *, F );

    class pconnection;

    template <class Signal,class Emitter,class F>
    weak_ptr<pconnection> connect( weak_ptr<Emitter> const &, F );

    template <class Signal,class Emitter,class F>
    weak_ptr<pconnection> connect( shared_ptr<Emitter> const &, F );

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( Emitter *, weak_ptr<Receiver> const &, F );

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( Emitter *, shared_ptr<Receiver> const &, F );

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( weak_ptr<Emitter> const &, Receiver *, F );

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( weak_ptr<Emitter> const &, weak_ptr<Receiver> const &, F );

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( weak_ptr<Emitter> const &, shared_ptr<Receiver> const &, F );

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( shared_ptr<Emitter> const &, Receiver *, F );

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( shared_ptr<Emitter> const &, weak_ptr<Receiver> const &, F );

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( shared_ptr<Emitter> const &, shared_ptr<Receiver> const &, F );

    shared_ptr<connection const> release( weak_ptr<pconnection const> const & );
    shared_ptr<connection> release( weak_ptr<pconnection> const & );

    namespace meta
    {
        weak_ptr<void const> emitter();
        namespace connect_flags
        {
            unsigned const connecting=1;
            unsigned const first_for_this_emitter=2;
            unsigned const last_for_this_emitter=4;
        }

        template <class Signal>
        struct connected
        {
            typedef connected<Signal>(*type)( connection &, unsigned connect_flags );
        };
    }

} }

//Implementation details below.

namespace boost { namespace synapse {

    namespace synapse_detail
    {
        shared_ptr<connection> connect_( shared_ptr<thread_local_signal_data> const &, weak_store && e, weak_store && r, shared_ptr<void const> const &, int(*)(connection &,unsigned) );
        shared_ptr<pconnection> pconnect_( shared_ptr<thread_local_signal_data> const &, weak_store && e, weak_store && r, shared_ptr<void const> const &, int(*)(connection &,unsigned) );

        ////////////////////////////////////////////////////////

        template <class Signal,class F>
        shared_ptr<connection> connect_fwd( weak_store && e, F f )
        {
            return connect_(
                get_thread_local_signal_data<Signal>(true),
                std::move(e),
                weak_store(),
                synapse::make_shared<function<typename signal_traits<Signal>::signature> >(f),
                &emit_meta_connected<Signal> );
        }

        template <class Signal,class F>
        shared_ptr<pconnection> pconnect_fwd( weak_store && e, F f )
        {
            return pconnect_(
                get_thread_local_signal_data<Signal>(true),
                std::move(e),
                weak_store(),
                synapse::make_shared<function<typename signal_traits<Signal>::signature> >(f),
                &emit_meta_connected<Signal> );
        }

        template <class Signal,class Receiver,class F,class Signature>
        struct bind_front;

        template <class Signal,class Receiver,class Rm,class... Am,class R,class... A>
        struct bind_front<Signal,Receiver,Rm (Receiver::*)(Am...),R(A...)>
        {
            static shared_ptr<connection> connect_fwd( weak_store && e, weak_store && r, Rm (Receiver::*f)(Am...) )
            {
                return connect_(
                    get_thread_local_signal_data<Signal>(true),
                    std::move(e),
                    std::move(r),
                    synapse::make_shared<function<typename signal_traits<Signal>::signature> >([=]( A... a ) { return (r.maybe_lock<Receiver>().get()->*f)(a...); }),
                    &emit_meta_connected<Signal> );
            }

            static shared_ptr<pconnection> pconnect_fwd( weak_store && e, weak_store && r, Rm (Receiver::*f)(Am...) )
            {
                return pconnect_(
                    get_thread_local_signal_data<Signal>(true),
                    std::move(e),
                    std::move(r),
                    synapse::make_shared<function<typename signal_traits<Signal>::signature> >([=]( A... a ) { return (r.maybe_lock<Receiver>().get()->*f)(a...); }),
                    &emit_meta_connected<Signal> );
            }
        };

        template <class Signal,class Receiver,class F,class R,class... A>
        struct bind_front<Signal,Receiver,F,R(A...)>
        {
            static shared_ptr<connection> connect_fwd( weak_store && e, weak_store && r, F f )
            {
                return connect_(
                    get_thread_local_signal_data<Signal>(true),
                    std::move(e),
                    std::move(r),
                    synapse::make_shared<function<typename signal_traits<Signal>::signature> >([=]( A... a ) { return f(&*r.maybe_lock<Receiver>(),a...); }),
                    &emit_meta_connected<Signal> );
            }

            static shared_ptr<pconnection> pconnect_fwd( weak_store && e, weak_store && r, F f )
            {
                return pconnect_(
                    get_thread_local_signal_data<Signal>(true),
                    std::move(e),
                    std::move(r),
                    synapse::make_shared<function<typename signal_traits<Signal>::signature> >([=]( A... a ) { return f(&*r.maybe_lock<Receiver>(),a...); }),
                    &emit_meta_connected<Signal> );
            }
        };
    } //namespace synapse_detail

    template <class Signal,class Emitter,class F>
    shared_ptr<connection> connect( Emitter * e, F f )
    {
        return synapse_detail::connect_fwd<Signal>(e,f);
    }

    template <class Signal,class Emitter,class Receiver,class F>
    shared_ptr<connection> connect( Emitter * e, Receiver * r, F f )
    {
        return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::connect_fwd(e,r,f);
    }

    template <class Signal,class Emitter,class F>
    weak_ptr<pconnection> connect( weak_ptr<Emitter> const & e, F f )
    {
        return synapse_detail::pconnect_fwd<Signal>(e,f);
    }

    template <class Signal,class Emitter,class F>
    weak_ptr<pconnection> connect( shared_ptr<Emitter> const & e, F f )
    {
        return synapse_detail::pconnect_fwd<Signal>(e,f);
    }

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( Emitter * e, weak_ptr<Receiver> const & r, F f )
    {
        return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
    }

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( Emitter * e, shared_ptr<Receiver> const & r, F f )
    {
        return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
    }

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( weak_ptr<Emitter> const & e, Receiver * r, F f )
    {
        return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
    }

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( weak_ptr<Emitter> const & e, weak_ptr<Receiver> const & r, F f )
    {
        return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
    }

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( weak_ptr<Emitter> const & e, shared_ptr<Receiver> const & r, F f )
    {
        return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
    }

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( shared_ptr<Emitter> const & e, Receiver * r, F f )
    {
        return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
    }

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( shared_ptr<Emitter> const & e, weak_ptr<Receiver> const & r, F f )
    {
        return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
    }

    template <class Signal,class Emitter,class Receiver,class F>
    weak_ptr<pconnection> connect( shared_ptr<Emitter> const & e, shared_ptr<Receiver> const & r, F f )
    {
        return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
    }

} }

#endif
