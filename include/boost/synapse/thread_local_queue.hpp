#ifndef BOOST_SYNAPSE_THREAD_LOCAL_QUEUE_HPP_INCLUDED
#define BOOST_SYNAPSE_THREAD_LOCAL_QUEUE_HPP_INCLUDED

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

#include <boost/synapse/detail/smart_ptr.hpp>
#include <boost/synapse/detail/functional.hpp>

namespace boost { namespace synapse {

	struct thread_local_queue;

	shared_ptr<thread_local_queue> create_thread_local_queue();
	int poll( thread_local_queue & );
	int wait( thread_local_queue & );
	void post( thread_local_queue &, function<void()> const & );

} }

#endif
