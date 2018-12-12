//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_B99C2B5F62144B96A76449BF1A5FFBBD
#define UUID_B99C2B5F62144B96A76449BF1A5FFBBD

#include <boost/synapse/synapse_detail/common.hpp>
#include <boost/synapse/synapse_detail/weak_store.hpp>

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

//Implementation details below.

namespace boost { namespace synapse {

    template <class T>
    shared_ptr<T> blocker::emitter() const
    {
        return emitter_().maybe_lock<T>();
    }

} }

#endif
