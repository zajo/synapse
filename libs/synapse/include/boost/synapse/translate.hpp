//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_D4AA6341754C44AE9659D0E451945AAD
#define UUID_D4AA6341754C44AE9659D0E451945AAD

#include <boost/synapse/connect.hpp>
#include <boost/bind.hpp>

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
            template <class OriginalSignal,class TranslatedSignal> struct translate_binder;
            template <class OrgR,class TrR>
            struct
            translate_binder<OrgR(*)(),TrR(*)()>
                {
                typedef function<void()> function_type;
                static
                function_type
                bind_( void const * e )
                    {
                    return bind(&emit<TrR(*)()>,e);
                    }
                };
            template <class OrgR,class OrgA1,class TrR,class TrA1>
            struct
            translate_binder<OrgR(*)(OrgA1),TrR(*)(TrA1)>
                {
                typedef function<void(OrgA1)> function_type;
                static
                function_type
                bind_( void const * e )
                    {
                    return bind(&emit<TrR(*)(TrA1)>,e,_1);
                    }
                };
            template <class OrgR,class OrgA1,class OrgA2,class TrR,class TrA1,class TrA2>
            struct
            translate_binder<OrgR(*)(OrgA1,OrgA2),TrR(*)(TrA1,TrA2)>
                {
                typedef function<void(OrgA1,OrgA2)> function_type;
                static
                function_type
                bind_( void const * e )
                    {
                    return bind(&emit<TrR(*)(TrA1,TrA2)>,e,_1,_2);
                    }
                };
            template <class OrgR,class OrgA1,class OrgA2,class OrgA3,class TrR,class TrA1,class TrA2,class TrA3>
            struct
            translate_binder<OrgR(*)(OrgA1,OrgA2,OrgA3),TrR(*)(TrA1,TrA2,TrA3)>
                {
                typedef function<void(OrgA1,OrgA2,OrgA3)> function_type;
                static
                function_type
                bind_( void const * e )
                    {
                    return bind(&emit<TrR(*)(TrA1,TrA2,TrA3)>,e,_1,_2,_3);
                    }
                };
            template <class OrgR,class OrgA1,class OrgA2,class OrgA3,class OrgA4,class TrR,class TrA1,class TrA2,class TrA3,class TrA4>
            struct
            translate_binder<OrgR(*)(OrgA1,OrgA2,OrgA3,OrgA4),TrR(*)(TrA1,TrA2,TrA3,TrA4)>
                {
                typedef function<void(OrgA1,OrgA2,OrgA3,OrgA4)> function_type;
                static
                function_type
                bind_( void const * e )
                    {
                    return bind(&emit<TrR(*)(TrA1,TrA2,TrA3,TrA4)>,e,_1,_2,_3,_4);
                    }
                };
            template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter>
            shared_ptr<connection>
            translate_fwd( weak_ptr<OriginalEmitter> const & original_emitter, OriginalEmitter * px, void const * translated_emitter, weak_ptr<void const> const & connection_lifetime )
                {
                typedef translate_binder<OriginalSignal,TranslatedSignal> translator;
                return connect_(
                    get_connection_list<OriginalSignal>(),
                    weak_store(original_emitter,px),
                    shared_ptr<void const>(new typename translator::function_type(translator::bind_(translated_emitter))),
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
