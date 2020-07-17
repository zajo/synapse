#ifndef BOOST_SYNAPSE_DEP_ASSERT_HPP_INCLUDED
#define BOOST_SYNAPSE_DEP_ASSERT_HPP_INCLUDED

//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_SYNAPSE_ASSERT
#   ifdef BOOST_SYNAPSE_USE_BOOST
#       include <boost/assert.hpp>
#       define BOOST_SYNAPSE_ASSERT BOOST_ASSERT
#   else
#       include <cassert>
#       define BOOST_SYNAPSE_ASSERT assert
#   endif
#endif

#endif
