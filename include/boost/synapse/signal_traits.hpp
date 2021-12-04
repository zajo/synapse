#ifndef BOOST_SYNAPSE_SIGNAL_TRAITS_HPP_INCLUDED
#define BOOST_SYNAPSE_SIGNAL_TRAITS_HPP_INCLUDED

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

namespace boost { namespace synapse { namespace meta { template <class Signal>  struct connected; } } }
namespace boost { namespace synapse { namespace meta { template <class Signal>  struct blocked; } } }

namespace boost { namespace synapse {

	template <class Signal> struct signal_traits;

	template <class R, class... A>
	struct signal_traits<R(*)(A...)>
	{
		typedef R(*signal_type)(A...);
		typedef void signature(A...);
		static int const arity = sizeof...(A);
		static bool const is_thread_local=false;
	};

	template <class Signal>
	struct signal_traits<meta::connected<Signal> >:
		signal_traits<typename meta::connected<Signal>::type>
	{
		static bool const is_thread_local=true;
	};

	template <class Signal>
	struct signal_traits<meta::blocked<Signal> >:
		signal_traits<typename meta::blocked<Signal>::type>
	{
		static bool const is_thread_local=true;
	};

} }

#if defined(_MSC_VER) && !defined(BOOST_SYNAPSE_ENABLE_WARNINGS)
#	pragma warning(pop)
#endif

#endif
