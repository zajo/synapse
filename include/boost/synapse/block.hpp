#ifndef BOOST_SYNAPSE_BLOCK_HPP_INCLUDED
#define BOOST_SYNAPSE_BLOCK_HPP_INCLUDED

// Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

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

#include <boost/synapse/detail/common.hpp>
#include <boost/synapse/detail/weak_store.hpp>

namespace boost { namespace synapse {

	class blocker;

	template <class Signal, class Emitter>
	shared_ptr<blocker> block( Emitter * );

	template <class Signal, class Emitter>
	shared_ptr<blocker> block( weak_ptr<Emitter> const & );

	template <class Signal, class Emitter>
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

// Implementation details below.

namespace boost { namespace synapse {

	namespace synapse_detail
	{
		shared_ptr<blocker> block_( shared_ptr<thread_local_signal_data> const &, weak_store &&, int(*)(blocker &,bool) );
		template <class Signal> int emit_meta_blocked( blocker &, bool );

		template <class Signal, class Emitter>
		shared_ptr<blocker> block_fwd( weak_store && e )
		{
			return block_(get_thread_local_signal_data<Signal>(true),std::move(e),&emit_meta_blocked<Signal>);
		}
	}

	template <class Signal, class Emitter>
	shared_ptr<blocker> block( Emitter * e )
	{
		return synapse_detail::block_fwd<Signal,Emitter>(e);
	}

	template <class Signal, class Emitter>
	shared_ptr<blocker> block( weak_ptr<Emitter> const & e )
	{
		return synapse_detail::block_fwd<Signal,Emitter>(e);
	}

	template <class Signal, class Emitter>
	shared_ptr<blocker> block( shared_ptr<Emitter> const & e )
	{
		return synapse_detail::block_fwd<Signal,Emitter>(e);
	}

} }

#endif
