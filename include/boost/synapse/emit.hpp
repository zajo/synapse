//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_77496C8C9BED485F9C9FCA2DC3027E28
#define UUID_77496C8C9BED485F9C9FCA2DC3027E28

#include <boost/synapse/synapse_detail/common.hpp>
#include <boost/function.hpp>

namespace
boost
    {
    namespace
    synapse
        {
        template <class Signal>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==0,int>::type
        emit( void const * );

        template <class Signal>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==1,int>::type
        emit( void const *, typename signal_traits<Signal>::arg1_type );

        template <class Signal>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==2,int>::type
        emit( void const *, typename signal_traits<Signal>::arg1_type, typename signal_traits<Signal>::arg2_type );

        template <class Signal>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==3,int>::type
        emit( void const *, typename signal_traits<Signal>::arg1_type, typename signal_traits<Signal>::arg2_type, typename signal_traits<Signal>::arg3_type );

        template <class Signal>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==4,int>::type
        emit( void const *, typename signal_traits<Signal>::arg1_type, typename signal_traits<Signal>::arg2_type, typename signal_traits<Signal>::arg3_type, typename signal_traits<Signal>::arg4_type );
        }
    }

//Implementation details below.

namespace
boost
    {
    namespace
    synapse
        {
        class connection;
        class blocker;
        namespace
        synapse_detail
            {
            struct
            emit_binder_base
                {
                virtual void call( void const * ) const=0;
                };
            template <class Signal> struct emit_binder;
            template <class SigR>
            struct
            emit_binder<SigR(*)()>:
                emit_binder_base
                {
                emit_binder() { }
                void call( void const * f ) const { (*static_cast<function<void()> const *>(f))(); }
                };
            template <class SigR,class A1>
            struct
            emit_binder<SigR(*)(A1)>:
                emit_binder_base
                {
                A1 & a1;
                explicit emit_binder( A1 & a1 ): a1(a1) { }
                void call( void const * f ) const { (*static_cast<function<void(A1)> const *>(f))(a1); }
                };
            template <class SigR,class A1,class A2>
            struct
            emit_binder<SigR(*)(A1,A2)>:
                emit_binder_base
                {
                A1 & a1; A2 & a2;
                explicit emit_binder( A1 & a1, A2 & a2 ): a1(a1), a2(a2) { }
                void call( void const * f ) const { (*static_cast<function<void(A1,A2)> const *>(f))(a1,a2); }
                };
            template <class SigR,class A1,class A2,class A3>
            struct
            emit_binder<SigR(*)(A1,A2,A3)>:
                emit_binder_base
                {
                A1 & a1; A2 & a2; A3 & a3;
                explicit emit_binder( A1 & a1, A2 & a2, A3 & a3 ): a1(a1), a2(a2), a3(a3) { }
                void call( void const * f ) const { (*static_cast<function<void(A1,A2,A3)> const *>(f))(a1,a2,a3); }
                };
            template <class SigR,class A1,class A2,class A3,class A4>
            struct
            emit_binder<SigR(*)(A1,A2,A3,A4)>:
                emit_binder_base
                {
                A1 & a1; A2 & a2; A3 & a3; A4 & a4;
                explicit emit_binder( A1 & a1, A2 & a2, A3 & a3, A4 & a4 ): a1(a1), a2(a2), a3(a3), a4(a4) { }
                void call( void const * f ) const { (*static_cast<function<void(A1,A2,A3,A4)> const *>(f))(a1,a2,a3,a4); }
                };
            shared_ptr<void const> & meta_emitter();
            template <class Signal>
            int
            emit_meta_connected( connection & c, unsigned connect_flags )
                {
                return emit<meta::connected<Signal> >(meta_emitter().get(),c,connect_flags);
                }
            template <class Signal>
            int
            emit_meta_blocked( blocker & eb, bool is_blocked )
                {
                return emit<meta::blocked<Signal> >(meta_emitter().get(),eb,is_blocked);
                }
            }
        template <class Signal>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==0,int>::type
        emit( void const * e )
            {
            return e? synapse_detail::emit_<Signal>()(synapse_detail::get_connection_list<Signal>(),synapse_detail::get_blocked_list<Signal>(),synapse_detail::emitter_blocked_<Signal>(),e,
                synapse_detail::emit_binder<typename signal_traits<Signal>::signal_type>()) : 0;
            }
        template <class Signal>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==1,int>::type
        emit( void const * e,
                typename signal_traits<Signal>::arg1_type a1 )
            {
            return e? synapse_detail::emit_<Signal>()(synapse_detail::get_connection_list<Signal>(),synapse_detail::get_blocked_list<Signal>(),synapse_detail::emitter_blocked_<Signal>(),e,
                synapse_detail::emit_binder<typename signal_traits<Signal>::signal_type>(a1)) : 0;
            }
        template <class Signal>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==2,int>::type
        emit( void const * e,
                typename signal_traits<Signal>::arg1_type a1,
                typename signal_traits<Signal>::arg2_type a2 )
            {
            return e? synapse_detail::emit_<Signal>()(synapse_detail::get_connection_list<Signal>(),synapse_detail::get_blocked_list<Signal>(),synapse_detail::emitter_blocked_<Signal>(),e,
                synapse_detail::emit_binder<typename signal_traits<Signal>::signal_type>(a1,a2)) : 0;
            }
        template <class Signal>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==3,int>::type
        emit( void const * e,
                typename signal_traits<Signal>::arg1_type a1,
                typename signal_traits<Signal>::arg2_type a2,
                typename signal_traits<Signal>::arg3_type a3 )
            {
            return e? synapse_detail::emit_<Signal>()(synapse_detail::get_connection_list<Signal>(),synapse_detail::get_blocked_list<Signal>(),synapse_detail::emitter_blocked_<Signal>(),e,
                synapse_detail::emit_binder<typename signal_traits<Signal>::signal_type>(a1,a2,a3)) : 0;
            }
        template <class Signal>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==4,int>::type
        emit( void const * e,
                typename signal_traits<Signal>::arg1_type a1,
                typename signal_traits<Signal>::arg2_type a2,
                typename signal_traits<Signal>::arg3_type a3,
                typename signal_traits<Signal>::arg4_type a4 )
            {
            return e? synapse_detail::emit_<Signal>()(synapse_detail::get_connection_list<Signal>(),synapse_detail::get_blocked_list<Signal>(),synapse_detail::emitter_blocked_<Signal>(),e,
                synapse_detail::emit_binder<typename signal_traits<Signal>::signal_type>(a1,a2,a3,a4)) : 0;
            }
        } 
    }

#endif
