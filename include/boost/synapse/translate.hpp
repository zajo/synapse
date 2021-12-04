#ifndef BOOST_SYNAPSE_TRANSLATE_HPP_INCLUDED
#define BOOST_SYNAPSE_TRANSLATE_HPP_INCLUDED

// Copyright (c) 2015-2021 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SYNAPSE_ENABLE_WARNINGS
#	if defined(__clang__)
#		pragma clang system_header
#	elif (__GNUC__*100+__GNUC_MINOR__>301)
#		pragma GCC system_header
#	elif defined(_MSC_VER)
#		pragma warning(push,1)
#	endif
#endif

#include <boost/synapse/connect.hpp>

namespace boost { namespace synapse {

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::shared_ptr<connection> translate( OriginalEmitter *, TranslatedEmitter * );

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( OriginalEmitter *, std::weak_ptr<TranslatedEmitter> const & );

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( OriginalEmitter *, std::shared_ptr<TranslatedEmitter> const & );

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( std::weak_ptr<OriginalEmitter> const &, TranslatedEmitter * );

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( std::weak_ptr<OriginalEmitter> const &, std::weak_ptr<TranslatedEmitter> const & );

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( std::weak_ptr<OriginalEmitter> const &, std::shared_ptr<TranslatedEmitter> const & );

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( std::shared_ptr<OriginalEmitter> const &, TranslatedEmitter * );

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( std::shared_ptr<OriginalEmitter> const &, std::weak_ptr<TranslatedEmitter> const & );

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( std::shared_ptr<OriginalEmitter> const &, std::shared_ptr<TranslatedEmitter> const & );

} }

// Implementation details below.

namespace boost { namespace synapse {

	namespace synapse_detail
	{
		template <class T> struct wr { static T & ap( T & x ) { return x; } };
		template <class T> struct wr<T &> { static std::reference_wrapper<T> ap( T & x ) { return std::reference_wrapper<T>(x); } };

		template <class OriginalSignal, class TranslatedSignal> struct translate_binder;

		template <class OrgR, class... OrgA, class TrR, class... TrA>

		struct translate_binder<OrgR(*)(OrgA...),TrR(*)(TrA...)>
		{
			typedef std::function<int(OrgA...)> function_type;
			static function_type bind_emit( weak_store r )
			{
				return
					[r](TrA... a)
					{
						return emit<TrR(*)(TrA...),TrA...>(r.maybe_lock<void const>().get(), a...);
					};
			}
		};

		std::shared_ptr<connection> connect_translated_( std::shared_ptr<thread_local_signal_data> const &, weak_store && e, weak_store && r, std::shared_ptr<void const> const &, int(*emit_meta_connected)(connection &,unsigned) );
		std::shared_ptr<pconnection> pconnect_translated_( std::shared_ptr<thread_local_signal_data> const &, weak_store && e, weak_store && r, std::shared_ptr<void const> const &, int(*emit_meta_connected)(connection &,unsigned) );

		template <class OriginalSignal, class TranslatedSignal>
		std::shared_ptr<connection> translate_fwd( weak_store && e, weak_store && r )
		{
			typedef translate_binder<OriginalSignal,TranslatedSignal> translator;
			return connect_translated_(
				get_thread_local_signal_data<OriginalSignal>(true),
				std::move(e),
				std::move(r),
				std::make_shared<typename translator::function_type>(translator::bind_emit(r)),
				&emit_meta_connected<OriginalSignal>);
		}

		template <class OriginalSignal, class TranslatedSignal>
		std::shared_ptr<pconnection> ptranslate_fwd( weak_store && e, weak_store && r )
		{
			typedef translate_binder<OriginalSignal,TranslatedSignal> translator;
			return pconnect_translated_(
				get_thread_local_signal_data<OriginalSignal>(true),
				std::move(e),
				std::move(r),
				std::make_shared<typename translator::function_type>(translator::bind_emit(r)),
				&emit_meta_connected<OriginalSignal>);
		}
	}

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::shared_ptr<connection> translate( OriginalEmitter * oe, TranslatedEmitter * te )
	{
		return synapse_detail::translate_fwd<OriginalSignal,TranslatedSignal>(oe, te);
	}

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( OriginalEmitter * oe, std::weak_ptr<TranslatedEmitter> const & te )
	{
		return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe, te);
	}

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( OriginalEmitter * oe, std::shared_ptr<TranslatedEmitter> const & te )
	{
		return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe, te);
	}

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( std::weak_ptr<OriginalEmitter> const & oe, TranslatedEmitter * te )
	{
		return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe, te);
	}

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( std::weak_ptr<OriginalEmitter> const & oe, std::weak_ptr<TranslatedEmitter> const & te )
	{
		return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe, te);
	}

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( std::weak_ptr<OriginalEmitter> const & oe, std::shared_ptr<TranslatedEmitter> const & te )
	{
		return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe, te);
	}

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( std::shared_ptr<OriginalEmitter> const & oe, TranslatedEmitter * te )
	{
		return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe, te);
	}

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( std::shared_ptr<OriginalEmitter> const & oe, std::weak_ptr<TranslatedEmitter> const & te )
	{
		return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe, te);
	}

	template <class OriginalSignal, class TranslatedSignal, class OriginalEmitter, class TranslatedEmitter>
	std::weak_ptr<pconnection> translate( std::shared_ptr<OriginalEmitter> const & oe, std::shared_ptr<TranslatedEmitter> const & te )
	{
		return synapse_detail::ptranslate_fwd<OriginalSignal,TranslatedSignal>(oe, te);
	}

} }

#if defined(_MSC_VER) && !defined(BOOST_SYNAPSE_ENABLE_WARNINGS)
#	pragma warning(pop)
#endif

#endif
