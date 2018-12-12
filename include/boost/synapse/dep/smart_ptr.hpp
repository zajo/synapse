//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_B5B59E7434FB11E6B092BD38C8C3EC59
#define UUID_B5B59E7434FB11E6B092BD38C8C3EC59

#ifdef BOOST_SYNAPSE_USE_STD_SMART_PTR

#include <memory>

namespace boost { namespace synapse {

    using std::shared_ptr;
    using std::weak_ptr;
    using std::make_shared;
    using std::get_deleter;

} }

#else

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>

namespace boost { namespace synapse {

    using boost::shared_ptr;
    using boost::weak_ptr;
    using boost::make_shared;
    using boost::get_deleter;

} }

#endif

#endif
