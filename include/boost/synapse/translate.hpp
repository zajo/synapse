//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_D4AA6341754C44AE9659D0E451945AAD
#define UUID_D4AA6341754C44AE9659D0E451945AAD

#include <boost/synapse/connect.hpp>

namespace boost { namespace synapse {

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    shared_ptr<connection> translate( OriginalEmitter *, TranslatedEmitter * );

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( OriginalEmitter *, weak_ptr<TranslatedEmitter> const & );

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( OriginalEmitter *, shared_ptr<TranslatedEmitter> const & );

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( weak_ptr<OriginalEmitter> const &, TranslatedEmitter * );

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( weak_ptr<OriginalEmitter> const &, weak_ptr<TranslatedEmitter> const & );

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( weak_ptr<OriginalEmitter> const &, shared_ptr<TranslatedEmitter> const & );

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( shared_ptr<OriginalEmitter> const &, TranslatedEmitter * );

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( shared_ptr<OriginalEmitter> const &, weak_ptr<TranslatedEmitter> const & );

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( shared_ptr<OriginalEmitter> const &, shared_ptr<TranslatedEmitter> const & );

} }

//Implementation details below.

namespace boost { namespace synapse {

    namespace synapse_detail
    {
        template <class T> struct wr { static T & ap( T & x ) { return x; } };
        template <class T> struct wr<T &> { static std::reference_wrapper<T> ap( T & x ) { return std::reference_wrapper<T>(x); } };
    
        template <class OriginalSignal,class TranslatedSignal> struct translate_binder;
    
        template <class OrgR,class... OrgA,class TrR,class... TrA>
    
        struct translate_binder<OrgR(*)(OrgA...),TrR(*)(TrA...)>
        {
            typedef function<int(OrgA...)> function_type;
            static function_type bind_emit( weak_store r )
            {
                return [r](TrA... a) { return emit<TrR(*)(TrA...),TrA...>(r.maybe_lock<void const>().get(),a...); };
            }
        };

        shared_ptr<connection> connect_translated_( shared_ptr<thread_local_signal_data> const &, weak_store && e, weak_store && r, shared_ptr<void const> const &, int(*emit_meta_connected)(connection &,unsigned) );
        shared_ptr<pconnection> pconnect_translated_( shared_ptr<thread_local_signal_data> const &, weak_store && e, weak_store && r, shared_ptr<void const> const &, int(*emit_meta_connected)(connection &,unsigned) );

        template <class OriginalSignal,class TranslatedSignal>
        shared_ptr<connection> translate_fwd( weak_store && e, weak_store && r )
        {
            typedef translate_binder<OriginalSignal,TranslatedSignal> translator;
            return connect_translated_(
                get_thread_local_signal_data<OriginalSignal>(true),
                std::move(e),
                std::move(r),
                synapse::make_shared<typename translator::function_type>(translator::bind_emit(r)),
                &emit_meta_connected<OriginalSignal>);
        }

        template <class OriginalSignal,class TranslatedSignal>
        shared_ptr<pconnection> ptranslate_fwd( weak_store && e, weak_store && r )
        {
            typedef translate_binder<OriginalSignal,TranslatedSignal> translator;
            return pconnect_translated_(
                get_thread_local_signal_data<OriginalSignal>(true),
                std::move(e),
                std::move(r),
                synapse::make_shared<typename translator::function_type>(translator::bind_emit(r)),
                &emit_meta_connected<OriginalSignal>);
        }
    }

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    shared_ptr<connection> translate( OriginalEmitter * oe, TranslatedEmitter * te )
    {
        return synapse_detail::translate_fwd<OriginalSignal,TranslatedSignal>(oe,te);
    }

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( OriginalEmitter * oe, weak_ptr<TranslatedEmitter> const & te )
    {
        return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe,te);
    }

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( OriginalEmitter * oe, shared_ptr<TranslatedEmitter> const & te )
    {
        return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe,te);
    }

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( weak_ptr<OriginalEmitter> const & oe, TranslatedEmitter * te )
    {
        return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe,te);
    }

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( weak_ptr<OriginalEmitter> const & oe, weak_ptr<TranslatedEmitter> const & te )
    {
        return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe,te);
    }

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( weak_ptr<OriginalEmitter> const & oe, shared_ptr<TranslatedEmitter> const & te )
    {
        return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe,te);
    }

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( shared_ptr<OriginalEmitter> const & oe, TranslatedEmitter * te )
    {
        return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe,te);
    }

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( shared_ptr<OriginalEmitter> const & oe, weak_ptr<TranslatedEmitter> const & te )
    {
        return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe,te);
    }

    template <class OriginalSignal,class TranslatedSignal,class OriginalEmitter,class TranslatedEmitter>
    weak_ptr<pconnection> translate( shared_ptr<OriginalEmitter> const & oe, shared_ptr<TranslatedEmitter> const & te )
    {
        return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe,te);
    }

} }

#endif
