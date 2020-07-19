#ifndef BOOST_SYNAPSE_BLOCKER_HPP_INCLUDED
#define BOOST_SYNAPSE_BLOCKER_HPP_INCLUDED

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

	class blocker
	{
		virtual synapse_detail::weak_store const & emitter_() const=0;

	protected:

		blocker();
		~blocker();

	public:

		template <class T> shared_ptr<T> emitter() const;
	};

} }

// Implementation details below.

namespace boost { namespace synapse {

	template <class T>
	shared_ptr<T> blocker::emitter() const
	{
		return emitter_().maybe_lock<T>();
	}

} }

#endif
