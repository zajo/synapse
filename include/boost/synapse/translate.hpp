//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_D4AA6341754C44AE9659D0E451945AAD
#define UUID_D4AA6341754C44AE9659D0E451945AAD

#include <boost/synapse/connect.hpp>

namespace
boost
    {
    namespace
    synapse
        {
        template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter>
        typename synapse_detail::enable_if<is_signal<OriginalSignal>::value && is_signal<TranslatedSignal>::value, shared_ptr<connection> >::type
        translate( weak_ptr<OriginalEmitter> const & original_emitter, void const * translated_emitter,
            weak_ptr<void const> const & connection_lifetime=weak_ptr<void const>() );

        template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter>
        typename synapse_detail::enable_if<is_signal<OriginalSignal>::value && is_signal<TranslatedSignal>::value, shared_ptr<connection> >::type
        translate( shared_ptr<OriginalEmitter> const & original_emitter, void const * translated_emitter,
            weak_ptr<void const> const & connection_lifetime=weak_ptr<void const>() );

        template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter>
        typename synapse_detail::enable_if<is_signal<OriginalSignal>::value && is_signal<TranslatedSignal>::value, shared_ptr<connection> >::type
        translate( OriginalEmitter * original_emitter, void const * translated_emitter,
            weak_ptr<void const> const & connection_lifetime=weak_ptr<void const>() );
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
            template <class T> struct wr { static T & ap( T & x ) { return x; } };
            template <class T> struct wr<T &> { static std::reference_wrapper<T> ap( T & x ) { return std::reference_wrapper<T>(x); } };
            template <class OriginalSignal,class TranslatedSignal> struct translate_binder;
            template <class OrgR,class TrR>
            struct
            translate_binder<OrgR(*)(),TrR(*)()>
                {
                typedef function<int()> function_type;
                static
                function_type
                bind_emit( void const * e )
                    {
                    return [e]() { return emit<TrR(*)()>(e); };
                    }
                };
            template <class OrgR,class OrgA1,class TrR,class TrA1>
            struct
            translate_binder<OrgR(*)(OrgA1),TrR(*)(TrA1)>
                {
                typedef function<int(OrgA1)> function_type;
                static
                function_type
                bind_emit( void const * e )
                    {
                    return [e](TrA1 a1) { return emit<TrR(*)(TrA1)>(e,wr<TrA1>::ap(a1)); };
                    }
                };
            template <class OrgR,class OrgA1,class OrgA2,class TrR,class TrA1,class TrA2>
            struct
            translate_binder<OrgR(*)(OrgA1,OrgA2),TrR(*)(TrA1,TrA2)>
                {
                typedef function<int(OrgA1,OrgA2)> function_type;
                static
                function_type
                bind_emit( void const * e )
                    {
                    return [e](TrA1 a1,TrA2 a2) { return emit<TrR(*)(TrA1,TrA2)>(e,wr<TrA1>::ap(a1),wr<TrA2>::ap(a2)); };
                    }
                };
            template <class OrgR,class OrgA1,class OrgA2,class OrgA3,class TrR,class TrA1,class TrA2,class TrA3>
            struct
            translate_binder<OrgR(*)(OrgA1,OrgA2,OrgA3),TrR(*)(TrA1,TrA2,TrA3)>
                {
                typedef function<int(OrgA1,OrgA2,OrgA3)> function_type;
                static
                function_type
                bind_emit( void const * e )
                    {
                    return [e](TrA1 a1,TrA2 a2,TrA3 a3) { return emit<TrR(*)(TrA1,TrA2,TrA3)>(e,wr<TrA1>::ap(a1),wr<TrA2>::ap(a2),wr<TrA3>::ap(a3)); };
                    }
                };
            template <class OrgR,class OrgA1,class OrgA2,class OrgA3,class OrgA4,class TrR,class TrA1,class TrA2,class TrA3,class TrA4>
            struct
            translate_binder<OrgR(*)(OrgA1,OrgA2,OrgA3,OrgA4),TrR(*)(TrA1,TrA2,TrA3,TrA4)>
                {
                typedef function<int(OrgA1,OrgA2,OrgA3,OrgA4)> function_type;
                static
                function_type
                bind_emit( void const * e )
                    {
                    return [e](TrA1 a1,TrA2 a2,TrA3 a3,TrA4 a4) { return emit<TrR(*)(TrA1,TrA2,TrA3,TrA4)>(e,wr<TrA1>::ap(a1),wr<TrA2>::ap(a2),wr<TrA3>::ap(a3),wr<TrA4>::ap(a4)); };
                    }
                };
            template <class OrgR,class OrgA1,class OrgA2,class OrgA3,class OrgA4,class OrgA5,class TrR,class TrA1,class TrA2,class TrA3,class TrA4,class TrA5>
            struct
            translate_binder<OrgR(*)(OrgA1,OrgA2,OrgA3,OrgA4,OrgA5),TrR(*)(TrA1,TrA2,TrA3,TrA4,TrA5)>
                {
                typedef function<int(OrgA1,OrgA2,OrgA3,OrgA4,OrgA5)> function_type;
                static
                function_type
                bind_emit( void const * e )
                    {
                    return [e](TrA1 a1,TrA2 a2,TrA3 a3,TrA4 a4, TrA5 a5) { return emit<TrR(*)(TrA1,TrA2,TrA3,TrA4,TrA5)>(e,wr<TrA1>::ap(a1),wr<TrA2>::ap(a2),wr<TrA3>::ap(a3),wr<TrA4>::ap(a4),wr<TrA5>::ap(a5)); };
                    }
                };
            template <class OrgR,class OrgA1,class OrgA2,class OrgA3,class OrgA4,class OrgA5,class OrgA6,class TrR,class TrA1,class TrA2,class TrA3,class TrA4,class TrA5,class TrA6>
            struct
            translate_binder<OrgR(*)(OrgA1,OrgA2,OrgA3,OrgA4,OrgA5,OrgA6),TrR(*)(TrA1,TrA2,TrA3,TrA4,TrA5,TrA6)>
                {
                typedef function<int(OrgA1,OrgA2,OrgA3,OrgA4,OrgA5,OrgA6)> function_type;
                static
                function_type
                bind_emit( void const * e )
                    {
                    return [e](TrA1 a1,TrA2 a2,TrA3 a3,TrA4 a4, TrA5 a5, TrA6 a6) { return emit<TrR(*)(TrA1,TrA2,TrA3,TrA4,TrA5,TrA6)>(e,wr<TrA1>::ap(a1),wr<TrA2>::ap(a2),wr<TrA3>::ap(a3),wr<TrA4>::ap(a4),wr<TrA5>::ap(a5),wr<TrA6>::ap(a6)); };
                    }
                };
            template <class OrgR,class OrgA1,class OrgA2,class OrgA3,class OrgA4,class OrgA5,class OrgA6,class OrgA7,class TrR,class TrA1,class TrA2,class TrA3,class TrA4,class TrA5,class TrA6,class TrA7>
            struct
            translate_binder<OrgR(*)(OrgA1,OrgA2,OrgA3,OrgA4,OrgA5,OrgA6,OrgA7),TrR(*)(TrA1,TrA2,TrA3,TrA4,TrA5,TrA6,TrA7)>
                {
                typedef function<int(OrgA1,OrgA2,OrgA3,OrgA4,OrgA5,OrgA6,OrgA7)> function_type;
                static
                function_type
                bind_emit( void const * e )
                    {
                    return [e](TrA1 a1,TrA2 a2,TrA3 a3,TrA4 a4, TrA5 a5, TrA6 a6, TrA7 a7) { return emit<TrR(*)(TrA1,TrA2,TrA3,TrA4,TrA5,TrA6,TrA7)>(e,wr<TrA1>::ap(a1),wr<TrA2>::ap(a2),wr<TrA3>::ap(a3),wr<TrA4>::ap(a4),wr<TrA5>::ap(a5),wr<TrA6>::ap(a6),wr<TrA7>::ap(a7)); };
                    }
                };
            template <class OrgR,class OrgA1,class OrgA2,class OrgA3,class OrgA4,class OrgA5,class OrgA6,class OrgA7,class OrgA8,class TrR,class TrA1,class TrA2,class TrA3,class TrA4,class TrA5,class TrA6,class TrA7,class TrA8>
            struct
            translate_binder<OrgR(*)(OrgA1,OrgA2,OrgA3,OrgA4,OrgA5,OrgA6,OrgA7,OrgA8),TrR(*)(TrA1,TrA2,TrA3,TrA4,TrA5,TrA6,TrA7,TrA8)>
                {
                typedef function<int(OrgA1,OrgA2,OrgA3,OrgA4,OrgA5,OrgA6,OrgA7,OrgA8)> function_type;
                static
                function_type
                bind_emit( void const * e )
                    {
                    return [e](TrA1 a1,TrA2 a2,TrA3 a3,TrA4 a4, TrA5 a5, TrA6 a6, TrA7 a7, TrA8 a8) { return emit<TrR(*)(TrA1,TrA2,TrA3,TrA4,TrA5,TrA6,TrA7,TrA8)>(e,wr<TrA1>::ap(a1),wr<TrA2>::ap(a2),wr<TrA3>::ap(a3),wr<TrA4>::ap(a4),wr<TrA5>::ap(a5),wr<TrA6>::ap(a6),wr<TrA7>::ap(a7),wr<TrA8>::ap(a7)); };
                    }
                };
            shared_ptr<connection> connect_translated_( shared_ptr<thread_local_signal_data> const &, weak_store const &, shared_ptr<void const> const &, weak_ptr<void const> const &, int(*)(connection &,unsigned) );
            template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter>
            shared_ptr<connection>
            translate_fwd( weak_ptr<OriginalEmitter> const & original_emitter, OriginalEmitter * px, void const * translated_emitter, weak_ptr<void const> const & connection_lifetime )
                {
                typedef translate_binder<OriginalSignal,TranslatedSignal> translator;
                return connect_translated_(
                    get_thread_local_signal_data<OriginalSignal>(true),
                    weak_store(original_emitter,px),
                    synapse::make_shared<typename translator::function_type>(translator::bind_emit(translated_emitter)),
                    connection_lifetime,
                    &emit_meta_connected<OriginalSignal>);
                }
            }
        template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter>
        typename synapse_detail::enable_if<is_signal<OriginalSignal>::value && is_signal<TranslatedSignal>::value, shared_ptr<connection> >::type
        translate( weak_ptr<OriginalEmitter> const & original_emitter, void const * translated_emitter, weak_ptr<void const> const & connection_lifetime )
            {
            return synapse_detail::translate_fwd<OriginalSignal,TranslatedSignal,OriginalEmitter>(original_emitter,original_emitter.lock().get(),translated_emitter,connection_lifetime);
            }
        template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter>
        typename synapse_detail::enable_if<is_signal<OriginalSignal>::value && is_signal<TranslatedSignal>::value, shared_ptr<connection> >::type
        translate( shared_ptr<OriginalEmitter> const & original_emitter, void const * translated_emitter, weak_ptr<void const> const & connection_lifetime )
            {
            return synapse_detail::translate_fwd<OriginalSignal,TranslatedSignal,OriginalEmitter>(weak_ptr<OriginalEmitter>(original_emitter),original_emitter.get(),translated_emitter,connection_lifetime);
            }
        template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter>
        typename synapse_detail::enable_if<is_signal<OriginalSignal>::value && is_signal<TranslatedSignal>::value, shared_ptr<connection> >::type
        translate( OriginalEmitter * original_emitter, void const * translated_emitter, weak_ptr<void const> const & connection_lifetime )
            {
            return synapse_detail::translate_fwd<OriginalSignal,TranslatedSignal,OriginalEmitter>(shared_ptr<OriginalEmitter>(shared_ptr<void>(),original_emitter),original_emitter,translated_emitter,connection_lifetime);
            }
        } 
    }

#endif
