#ifndef BOOST_SYNAPSE_CONFIG_HPP_INCLUDED
#define BOOST_SYNAPSE_CONFIG_HPP_INCLUDED

// Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// The following is based on Boost Config.

// (C) Copyright John Maddock 2001 - 2003.
// (C) Copyright Martin Wille 2003.
// (C) Copyright Guillaume Melquiond 2003.

#ifndef BOOST_SYNAPSE_ENABLE_WARNINGS
#	if defined(__clang__)
#		pragma clang system_header
#	elif (__GNUC__*100+__GNUC_MINOR__>301)
#		pragma GCC system_header
#	elif defined(_MSC_VER)
#		pragma warning(push,1)
#	endif
#endif

////////////////////////////////////////

// Configure BOOST_SYNAPSE_NO_EXCEPTIONS, unless already #defined
#ifndef BOOST_SYNAPSE_NO_EXCEPTIONS

#	if defined __clang__ && !defined(__ibmxl__)
//	Clang C++ emulates GCC, so it has to appear early.

#		if !__has_feature(cxx_exceptions)
#			define BOOST_SYNAPSE_NO_EXCEPTIONS
#		endif

#	elif defined __DMC__
//	Digital Mars C++

#		if !defined(_CPPUNWIND)
#			define BOOST_SYNAPSE_NO_EXCEPTIONS
#		endif

#	elif defined(__GNUC__) && !defined(__ibmxl__)
//	GNU C++:

#		if !defined(__EXCEPTIONS)
#			define BOOST_SYNAPSE_NO_EXCEPTIONS
#		endif

#	elif defined __KCC
//	Kai C++

#		if !defined(_EXCEPTIONS)
#			define BOOST_SYNAPSE_NO_EXCEPTIONS
#		endif

#	elif defined __CODEGEARC__
//	CodeGear - must be checked for before Borland

#		if !defined(_CPPUNWIND) && !defined(__EXCEPTIONS)
#			define BOOST_SYNAPSE_NO_EXCEPTIONS
#		endif

#	elif defined __BORLANDC__
//	Borland

#		if !defined(_CPPUNWIND) && !defined(__EXCEPTIONS)
# 			define BOOST_SYNAPSE_NO_EXCEPTIONS
#		endif

#	elif defined  __MWERKS__
//	Metrowerks CodeWarrior

#		if !__option(exceptions)
#			define BOOST_SYNAPSE_NO_EXCEPTIONS
#		endif

#	elif defined(__IBMCPP__) && defined(__COMPILER_VER__) && defined(__MVS__)
//	IBM z/OS XL C/C++

#		if !defined(_CPPUNWIND) && !defined(__EXCEPTIONS)
#			define BOOST_SYNAPSE_NO_EXCEPTIONS
#		endif

#	elif defined(__ibmxl__)
//	IBM XL C/C++ for Linux (Little Endian)

#		if !__has_feature(cxx_exceptions)
#			define BOOST_SYNAPSE_NO_EXCEPTIONS
#		endif

#	elif defined _MSC_VER
//	Microsoft Visual C++
//
//	Must remain the last #elif since some other vendors (Metrowerks, for
//	example) also #define _MSC_VER

#		if !defined(_CPPUNWIND)
#			define BOOST_SYNAPSE_NO_EXCEPTIONS
#		endif
#	endif

#endif

////////////////////////////////////////

#ifndef BOOST_SYNAPSE_CONSTEXPR
#	if __cplusplus > 201402L
#		define BOOST_SYNAPSE_CONSTEXPR constexpr
#	else
#		define BOOST_SYNAPSE_CONSTEXPR
#	endif
#endif

////////////////////////////////////////

#ifndef BOOST_SYNAPSE_ASSERT
#	ifdef BOOST_ASSERT
#		define BOOST_SYNAPSE_ASSERT BOOST_ASSERT
#	else
#       include <cassert>
#       define BOOST_SYNAPSE_ASSERT assert
#	endif
#endif

#endif
