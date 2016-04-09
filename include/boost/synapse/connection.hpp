//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_D00C85AF6C3E4AEEA6E0B6AB862B43BD
#define UUID_D00C85AF6C3E4AEEA6E0B6AB862B43BD

namespace boost { class any; }

#include <boost/synapse/synapse_detail/common.hpp>
#include <boost/synapse/synapse_detail/weak_store.hpp>

namespace
boost
    {
    namespace
    synapse
        {
        class
        connection
            {
            virtual synapse_detail::weak_store const & emitter_() const=0;
            protected:
            explicit connection( any & );
            ~connection();
            public:
            any & user_data;
            template <class T> shared_ptr<T> emitter() const;
            };
        }
    }

//Implementation details below.

namespace
boost
    {
    namespace
    synapse
        {
        template <class T>
        shared_ptr<T>
        connection::
        emitter() const
            {
            return emitter_().maybe_lock<T>();
            }
        } 
    }

#endif
