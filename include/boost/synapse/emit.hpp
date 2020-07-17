#ifndef BOOST_SYNAPSE_EMIT_HPP_INCLUDED
#define BOOST_SYNAPSE_EMIT_HPP_INCLUDED

//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/synapse_detail/common.hpp>
#include <boost/synapse/dep/functional.hpp>
#ifdef BOOST_SYNAPSE_USE_BOOST
#   include <boost/mp11/tuple.hpp>
#endif
#include <tuple>

namespace boost { namespace synapse {

    template <class Signal,class... A>
    int emit( void const *, A... );

} }

//Implementation details below.

namespace boost { namespace synapse {

    class connection;
    class blocker;

    namespace synapse_detail
    {
        class args_binder_base
        {
        public:
            virtual shared_ptr<args_binder_base> clone() const=0;
            virtual void call( void const * ) const=0;
            virtual int call_translated( void const * ) const=0;
        };

        template <class Signal,class CallSignature> class args_binder;

        template<class SigR, class... SigA, class... A>
        class args_binder<SigR(*)(SigA...), void(*)(A...)>:
            public args_binder_base
        {
            std::tuple<A...> a_;
            shared_ptr<args_binder_base> clone() const { return synapse::make_shared<args_binder>(*this); }
#ifdef BOOST_SYNAPSE_USE_BOOST
            void call( void const * f ) const { mp11::tuple_apply(*static_cast<function<void(SigA...)> const *>(f),a_); }
            int call_translated( void const * f ) const { return mp11::tuple_apply(*static_cast<function<int(SigA...)> const *>(f),a_); }
#else
            void call( void const * f ) const { std::apply(*static_cast<function<void(SigA...)> const *>(f),a_); }
            int call_translated( void const * f ) const { return std::apply(*static_cast<function<int(SigA...)> const *>(f),a_); }
#endif
        public:
            args_binder( A... a ):
                a_(a...)
                {
                }
        };

        shared_ptr<void const> & meta_emitter();

        template <class Signal>
        int emit_meta_connected( connection & c, unsigned connect_flags )
        {
            return emit<meta::connected<Signal> >(meta_emitter().get(),ref(c),connect_flags);
        }

        template <class Signal>
        int emit_meta_blocked( blocker & eb, bool is_blocked )
        {
            return emit<meta::blocked<Signal> >(meta_emitter().get(),ref(eb),is_blocked);
        }

        template <class Signal>
        int emit_fwd( void const * e, args_binder_base const & args )
        {
            if( shared_ptr<thread_local_signal_data> const & tlsd=get_thread_local_signal_data<Signal>(false) )
                if( e )
                    return tlsd->emit_(*tlsd,e,&args);
            return 0;
        }

        template <class Signal>
        int emit_fwd_no_args( void const * e )
        {
            if( shared_ptr<thread_local_signal_data> const & tlsd=get_thread_local_signal_data<Signal>(false) )
                if( e )
                    return tlsd->emit_(*tlsd,e,0);
            return 0;
        }
    } //namespace synapse_detail

    template <class Signal,class... A>
    int emit( void const * e, A... a )
    {
        return synapse_detail::emit_fwd<Signal>(e,synapse_detail::args_binder<typename signal_traits<Signal>::signal_type,void(*)(A...)>(a...));
    }

}  }

#endif
