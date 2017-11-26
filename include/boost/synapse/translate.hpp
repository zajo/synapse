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
            template <class OrgR,class... OrgA,class TrR,class... TrA>
            struct
            translate_binder<OrgR(*)(OrgA...),TrR(*)(TrA...)>
                {
                typedef function<int(OrgA...)> function_type;
                static
                function_type
                bind_emit( void const * e )
                    {
                    return [e](TrA... a) { return emit<TrR(*)(TrA...),TrA...>(e,a...); };
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
