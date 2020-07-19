#ifndef BOOST_SYNAPSE_DEP_SMART_PTR_HPP_INCLUDED
#define BOOST_SYNAPSE_DEP_SMART_PTR_HPP_INCLUDED

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

#ifdef BOOST_SYNAPSE_USE_BOOST

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>

namespace boost { namespace synapse {

	using boost::shared_ptr;
	using boost::weak_ptr;
	using boost::make_shared;
	using boost::get_deleter;

} }

#else

#include <memory>

namespace boost { namespace synapse {

	using std::shared_ptr;
	using std::weak_ptr;
	using std::make_shared;
	using std::get_deleter;

} }

#endif

#endif
