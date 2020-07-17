#ifndef BOOST_SYNAPSE_DEP_FUNCTIONAL_HPP_INCLUDED
#define BOOST_SYNAPSE_DEP_FUNCTIONAL_HPP_INCLUDED

//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef BOOST_SYNAPSE_USE_BOOST

#include <boost/function.hpp>

namespace boost { namespace synapse {

    using boost::function;
    using boost::ref;
    using boost::cref;

} }

#else

#include <functional>

namespace boost { namespace synapse {

    using std::function;
    using std::ref;
    using std::cref;

} }

#endif

#endif
