//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_E89FC42234FB11E680D73239C8C3EC59
#define UUID_E89FC42234FB11E680D73239C8C3EC59

#ifdef BOOST_SYNAPSE_USE_STD_FUNCTION

#include <functional>

namespace boost { namespace synapse {

    using std::function;
    using std::ref;
    using std::cref;

} }

#else

#include <boost/function.hpp>

namespace boost { namespace synapse {

    using boost::function;
    using boost::ref;
    using boost::cref;

} }

#endif

#endif
