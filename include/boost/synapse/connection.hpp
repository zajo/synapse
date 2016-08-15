//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_D00C85AF6C3E4AEEA6E0B6AB862B43BD
#define UUID_D00C85AF6C3E4AEEA6E0B6AB862B43BD

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
            shared_ptr<void> user_data_;
            protected:
            ~connection();
            public:
            template <class T> void set_user_data( T const & );
            template <class T> T * get_user_data();
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
        namespace
        synapse_detail
            {
            template <class T>
            struct
            deleter_user_data
                {
                T value;
                explicit
                deleter_user_data( T value ):
                    value(value)
                    {
                    }
                void
                operator()( void const * ) const
                    {
                    }
                };
            }
        template <class T>
        void
        connection::
        set_user_data( T const & x )
            {
            user_data_.reset((void *)0,synapse_detail::deleter_user_data<T>(x));
            }
        template <class T>
        T *
        connection::
        get_user_data()
            {
            synapse_detail::deleter_user_data<T> * d=get_deleter<synapse_detail::deleter_user_data<T> >(user_data_);
            return d?&d->value:0;
            }
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
