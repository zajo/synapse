//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_77496C8C9BED485F9C9FCA2DC3027E28
#define UUID_77496C8C9BED485F9C9FCA2DC3027E28

#include <boost/synapse/synapse_detail/common.hpp>
#include <boost/synapse/dep/functional.hpp>
#include <type_traits>

namespace
boost
    {
    namespace
    synapse
        {
        template <class Signal>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==0,int>::type
        emit( void const * );

        template <class Signal,class A1>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==1,int>::type
        emit( void const *, A1 );

        template <class Signal,class A1,class A2>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==2,int>::type
        emit( void const *, A1, A2 );

        template <class Signal,class A1,class A2,class A3>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==3,int>::type
        emit( void const *, A1, A2, A3 );

        template <class Signal,class A1,class A2,class A3,class A4>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==4,int>::type
        emit( void const *, A1, A2, A3, A4 );
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
            class
            args_binder_base
                {
                public:
                virtual shared_ptr<args_binder_base> clone() const=0;
                virtual void call( void const * ) const=0;
                virtual int call_translated( void const * ) const=0;
                };
            template <class Signal,class CallSignature> class args_binder;
            template <class SigR,class SigA1,class A1>
            class
            args_binder<SigR(*)(SigA1),void(*)(A1)>:
                public args_binder_base
                {
                A1 a1;
                shared_ptr<args_binder_base> clone() const { return synapse::make_shared<args_binder>(*this); }
                void call( void const * f ) const { (*static_cast<function<void(SigA1)> const *>(f))(a1); }
                int call_translated( void const * f ) const { return (*static_cast<function<int(SigA1)> const *>(f))(a1); }
                public:
                explicit args_binder( A1 a1 ): a1(a1) { }
                };
            template <class SigR,class SigA1,class SigA2,class A1,class A2>
            class
            args_binder<SigR(*)(SigA1,SigA2),void(*)(A1,A2)>:
                public args_binder_base
                {
                A1 a1; A2 a2;
                shared_ptr<args_binder_base> clone() const { return synapse::make_shared<args_binder>(*this); }
                void call( void const * f ) const { (*static_cast<function<void(SigA1,SigA2)> const *>(f))(a1,a2); }
                int call_translated( void const * f ) const { return (*static_cast<function<int(SigA1,SigA2)> const *>(f))(a1,a2); }
                public:
                args_binder( A1 a1, A2 a2 ): a1(a1), a2(a2) { }
                };
            template <class SigR,class SigA1,class SigA2,class SigA3,class A1,class A2,class A3>
            class
            args_binder<SigR(*)(SigA1,SigA2,SigA3),void(*)(A1,A2,A3)>:
                public args_binder_base
                {
                A1 a1; A2 a2; A3 a3;
                shared_ptr<args_binder_base> clone() const { return synapse::make_shared<args_binder>(*this); }
                void call( void const * f ) const { (*static_cast<function<void(SigA1,SigA2,SigA3)> const *>(f))(a1,a2,a3); }
                int call_translated( void const * f ) const { return (*static_cast<function<int(SigA1,SigA2,SigA3)> const *>(f))(a1,a2,a3); }
                public:
                args_binder( A1 a1, A2 a2, A3 a3 ): a1(a1), a2(a2), a3(a3) { }
                };
            template <class SigR,class SigA1,class SigA2,class SigA3,class SigA4,class A1,class A2,class A3,class A4>
            class
            args_binder<SigR(*)(SigA1,SigA2,SigA3,SigA4),void(*)(A1,A2,A3,A4)>:
                public args_binder_base
                {
                A1 a1; A2 a2; A3 a3; A4 a4;
                shared_ptr<args_binder_base> clone() const { return synapse::make_shared<args_binder>(*this); }
                void call( void const * f ) const { (*static_cast<function<void(SigA1,SigA2,SigA3,SigA4)> const *>(f))(a1,a2,a3,a4); }
                int call_translated( void const * f ) const { return (*static_cast<function<int(SigA1,SigA2,SigA3,SigA4)> const *>(f))(a1,a2,a3,a4); }
                public:
                args_binder( A1 a1, A2 a2, A3 a3, A4 a4 ): a1(a1), a2(a2), a3(a3), a4(a4) { }
                };
            shared_ptr<void const> & meta_emitter();
            template <class Signal>
            int
            emit_meta_connected( connection & c, unsigned connect_flags )
                {
                return emit<meta::connected<Signal> >(meta_emitter().get(),std::ref(c),connect_flags);
                }
            template <class Signal>
            int
            emit_meta_blocked( blocker & eb, bool is_blocked )
                {
                return emit<meta::blocked<Signal> >(meta_emitter().get(),std::ref(eb),is_blocked);
                }
            template <class Signal>
            int
            emit_fwd( void const * e, args_binder_base const & args )
                {
                if( shared_ptr<thread_local_signal_data> const & tlsd=get_thread_local_signal_data<Signal>(false) )
                    if( e )
                        return tlsd->emit_(*tlsd,e,&args);
                return 0;
                }
            template <class Signal>
            int
            emit_fwd_no_args( void const * e )
                {
                if( shared_ptr<thread_local_signal_data> const & tlsd=get_thread_local_signal_data<Signal>(false) )
                    if( e )
                        return tlsd->emit_(*tlsd,e,0);
                return 0;
                }
            }
        template <class Signal>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==0,int>::type
        emit( void const * e )
            {
            return synapse_detail::emit_fwd_no_args<Signal>(e);
            }
        template <class Signal,class A1>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==1,int>::type
        emit( void const * e, A1 a1 )
            {
            return synapse_detail::emit_fwd<Signal>(e,synapse_detail::args_binder<typename signal_traits<Signal>::signal_type,void(*)(A1)>(a1));
            }
        template <class Signal,class A1,class A2>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==2,int>::type
        emit( void const * e, A1 a1, A2 a2 )
            {
            return synapse_detail::emit_fwd<Signal>(e,synapse_detail::args_binder<typename signal_traits<Signal>::signal_type,void(*)(A1,A2)>(a1,a2));
            }
        template <class Signal,class A1,class A2,class A3>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==3,int>::type
        emit( void const * e, A1 a1, A2 a2, A3 a3 )
            {
            return synapse_detail::emit_fwd<Signal>(e,synapse_detail::args_binder<typename signal_traits<Signal>::signal_type,void(*)(A1,A2,A3)>(a1,a2,a3));
            }
        template <class Signal,class A1,class A2,class A3,class A4>
        typename synapse_detail::enable_if<signal_traits<Signal>::arity==4,int>::type
        emit( void const * e, A1 a1, A2 a2, A3 a3, A4 a4 )
            {
            return synapse_detail::emit_fwd<Signal>(e,synapse_detail::args_binder<typename signal_traits<Signal>::signal_type,void(*)(A1,A2,A3,A4)>(a1,a2,a3,a4));
            }
        } 
    }

#endif
