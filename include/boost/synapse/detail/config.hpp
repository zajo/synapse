#ifndef BOOST_SYNAPSE_DETAIL_CONFIG_HPP_INCLUDED
#define BOOST_SYNAPSE_DETAIL_CONFIG_HPP_INCLUDED

// Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// The following is based on Boost Config.

// (C) Copyright John Maddock 2001 - 2003.
// (C) Copyright Martin Wille 2003.
// (C) Copyright Guillaume Melquiond 2003.
// Copyright 2016, 2018, 2019 Peter Dimov.

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

#if defined( BOOST_STRICT_CONFIG ) || defined( BOOST_SYNAPSE_NO_WORKAROUNDS )
#	define BOOST_SYNAPSE_WORKAROUND( symbol, test ) 0
#else
#	define BOOST_SYNAPSE_WORKAROUND( symbol, test ) ((symbol) != 0 && ((symbol) test))
#endif

////////////////////////////////////////

#define BOOST_SYNAPSE_CLANG 0
#define BOOST_SYNAPSE_GCC 0
#define BOOST_SYNAPSE_MSVC 0

#define BOOST_SYNAPSE_CONSTEXPR constexpr

#if defined(__clang__) && !defined(__ibmxl__)
//	Clang C++ emulates GCC, so it has to appear early.

#	undef BOOST_SYNAPSE_CLANG
#	define BOOST_SYNAPSE_CLANG (__clang_major__ * 100 + __clang_minor__)

#	if !__has_feature(cxx_exceptions) && !defined(BOOST_SYNAPSE_NO_EXCEPTIONS)
#		define BOOST_SYNAPSE_NO_EXCEPTIONS
#	endif

#elif defined(__DMC__)
//	Digital Mars C++

#	if !defined(_CPPUNWIND) && !defined(BOOST_SYNAPSE_NO_EXCEPTIONS)
#		define BOOST_SYNAPSE_NO_EXCEPTIONS
#	endif

#elif defined(__GNUC__) && !defined(__ibmxl__)
//	GNU C++:

#	undef BOOST_SYNAPSE_GCC
#	define BOOST_SYNAPSE_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#	if !defined(__EXCEPTIONS) && !defined(BOOST_SYNAPSE_NO_EXCEPTIONS)
#		define BOOST_SYNAPSE_NO_EXCEPTIONS
#	endif

#elif defined(__KCC)
//	Kai C++

#	if !defined(_EXCEPTIONS) && !defined(BOOST_SYNAPSE_NO_EXCEPTIONS)
#		define BOOST_SYNAPSE_NO_EXCEPTIONS
#	endif

#elif defined(__CODEGEARC__)
//	CodeGear - must be checked for before Borland

#	if !defined(_CPPUNWIND) && !defined(__EXCEPTIONS) && !defined(BOOST_SYNAPSE_NO_EXCEPTIONS)
#		define BOOST_SYNAPSE_NO_EXCEPTIONS
#	endif

#elif defined(__BORLANDC__)
//	Borland

#	if !defined(_CPPUNWIND) && !defined(__EXCEPTIONS) && !defined(BOOST_SYNAPSE_NO_EXCEPTIONS)
#		define BOOST_SYNAPSE_NO_EXCEPTIONS
#	endif

#elif defined(__MWERKS__)
//	Metrowerks CodeWarrior

#	if !__option(exceptions) && !defined(BOOST_SYNAPSE_NO_EXCEPTIONS)
#		define BOOST_SYNAPSE_NO_EXCEPTIONS
#	endif

#elif defined(__IBMCPP__) && defined(__COMPILER_VER__) && defined(__MVS__)
//	IBM z/OS XL C/C++

#	if !defined(_CPPUNWIND) && !defined(__EXCEPTIONS) && !defined(BOOST_SYNAPSE_NO_EXCEPTIONS)
#		define BOOST_SYNAPSE_NO_EXCEPTIONS
#	endif

#elif defined(__ibmxl__)
//	IBM XL C/C++ for Linux (Little Endian)

#	if !__has_feature(cxx_exceptions) && !defined(BOOST_SYNAPSE_NO_EXCEPTIONS)
#		define BOOST_SYNAPSE_NO_EXCEPTIONS
#	endif

#elif defined(_MSC_VER)
//	Microsoft Visual C++
//
//	Must remain the last #elif since some other vendors (Metrowerks, for
//	example) also #define _MSC_VER

#	if !defined(_CPPUNWIND) && !defined(BOOST_SYNAPSE_NO_EXCEPTIONS)
#		define BOOST_SYNAPSE_NO_EXCEPTIONS
#	endif

#	undef BOOST_SYNAPSE_MSVC
#	define BOOST_SYNAPSE_MSVC _MSC_VER

#	if BOOST_SYNAPSE_WORKAROUND( BOOST_SYNAPSE_MSVC, < 1920 )
#		define BOOST_SYNAPSE_NO_CONSTEXPR
#	endif

#	if _MSC_FULL_VER < 190024210 // 2015u3
#		undef BOOST_SYNAPSE_CONSTEXPR
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
