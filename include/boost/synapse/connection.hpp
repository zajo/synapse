#ifndef BOOST_SYNAPSE_CONNECTION_HPP_INCLUDED
#define BOOST_SYNAPSE_CONNECTION_HPP_INCLUDED

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

#include <boost/synapse/detail/smart_ptr.hpp>
#include <boost/synapse/detail/weak_store.hpp>
#include <type_traits>

namespace boost { namespace synapse {

	class connection
	{

		virtual synapse_detail::weak_store const & emitter_() const=0;
		virtual synapse_detail::weak_store const & receiver_() const=0;
		shared_ptr<void> user_data_;

	protected:

		connection();
		~connection();

	public:

		template <class T> void set_user_data( T const & );
		template <class T> T * get_user_data() const;
		template <class T> shared_ptr<T> emitter() const;
		template <class T> shared_ptr<T> receiver() const;
	};

	class pconnection:
		protected connection
	{
	protected:

		pconnection();
		~pconnection();

	public:

		using connection::set_user_data;
		using connection::get_user_data;
		using connection::emitter;
		using connection::receiver;
	};

} }

// Implementation details below.

namespace boost { namespace synapse {

	namespace synapse_detail
	{
		template <class T>
		struct deleter_user_data
		{
			T value;

			explicit deleter_user_data( T value ):
				value(value)
			{
			}

			void operator()( void const * ) const
			{
			}
		};

		template <class T>
		typename std::enable_if<std::is_assignable<T&,T const &>::value>::type set_user_data_( shared_ptr<void> & ud, T const & x )
		{
			if( deleter_user_data<T> * d = get_deleter<deleter_user_data<T> >(ud) )
				d->value = x;
			else
				ud.reset((void *)0, deleter_user_data<T>(x));
		}

		template <class T>
		typename std::enable_if<!std::is_assignable<T&,T const &>::value>::type set_user_data_( shared_ptr<void> & ud, T const & x )
		{
			ud.reset((void *)0, deleter_user_data<T>(x));
		}
	}

	template <class T>
	void connection::set_user_data( T const & x )
	{
		synapse_detail::set_user_data_(user_data_, x);
	}

	template <class T>
	T * connection::get_user_data() const
	{
		synapse_detail::deleter_user_data<T> * d = get_deleter<synapse_detail::deleter_user_data<T> >(user_data_);
		return d ? &d->value : 0;
	}

	template <class T>
	shared_ptr<T> connection::emitter() const
	{
		return emitter_().maybe_lock<T>();
	}

	template <class T>
	shared_ptr<T> connection::receiver() const
	{
		return receiver_().maybe_lock<T>();

	}

} }

#endif
