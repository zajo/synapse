#ifndef BOOST_SYNAPSE_DETAIL_WEAK_STORE_HPP_INCLUDED
#define BOOST_SYNAPSE_DETAIL_WEAK_STORE_HPP_INCLUDED

// Copyright (c) 2015-2021 Emil Dotchevski and Reverge Studios, Inc.

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

#include <boost/synapse/detail/config.hpp>
#include <memory>

namespace boost { namespace synapse {

	namespace synapse_detail
	{
		class weak_store
		{
			std::weak_ptr<void const> w_;
			void const * px_;
			void (*type_)();
			void (*ctype_)();
			template <class> struct access;
			template <class> static void type() { }

		public:

			weak_store & operator=( weak_store const & )=default;
			weak_store( weak_store const & )=default;
			weak_store( weak_store && )=default;

			weak_store():
				px_(0),
				type_(0),
				ctype_(0)
			{
				BOOST_SYNAPSE_ASSERT(empty());
				BOOST_SYNAPSE_ASSERT(!lockable());
			}

			template <class T>
			weak_store( T * px ):
				px_(px),
				type_(&type<T>),
				ctype_(&type<T const>)
			{
				BOOST_SYNAPSE_ASSERT(!lockable());
				BOOST_SYNAPSE_ASSERT(!empty());
			}

			template <class T>
			weak_store( T const * px ):
				px_(px),
				type_(0),
				ctype_(&type<T const>)
			{
				BOOST_SYNAPSE_ASSERT(!lockable());
				BOOST_SYNAPSE_ASSERT(!empty());
			}

			template <class T>
			weak_store( std::weak_ptr<T> const & w ):
				w_(w),
				px_(0),
				type_(&type<T>),
				ctype_(&type<T const>)
			{
				BOOST_SYNAPSE_ASSERT(w_.lock());
				BOOST_SYNAPSE_ASSERT(lockable());
				BOOST_SYNAPSE_ASSERT(!empty());
			}

			template <class T>
			weak_store( std::weak_ptr<T const> const & w ):
				w_(w),
				px_(0),
				type_(0),
				ctype_(&type<T const>)
			{
				BOOST_SYNAPSE_ASSERT(w_.lock());
				BOOST_SYNAPSE_ASSERT(lockable());
				BOOST_SYNAPSE_ASSERT(!empty());
			}

			template <class T>
			weak_store( std::shared_ptr<T> const & w ):
				w_(w),
				px_(0),
				type_(&type<T>),
				ctype_(&type<T const>)
			{
				BOOST_SYNAPSE_ASSERT(w_.lock());
				BOOST_SYNAPSE_ASSERT(lockable());
				BOOST_SYNAPSE_ASSERT(!empty());
			}

			template <class T>
			weak_store( std::shared_ptr<T const> const & w ):
				w_(w),
				px_(0),
				type_(0),
				ctype_(&type<T const>)
			{
				BOOST_SYNAPSE_ASSERT(w_.lock());
				BOOST_SYNAPSE_ASSERT(lockable());
				BOOST_SYNAPSE_ASSERT(!empty());
			}

			void clear()
			{
				w_.reset();
				px_=0;
				type_=0;
				ctype_=0;
			}

			bool empty() const
			{
				return ctype_==0;
			}

			bool expired() const
			{
				return !px_ && w_.expired();
			}

			bool lockable() const
			{
				return px_==0 && !empty();
			}

			template <class T>
			std::shared_ptr<T> maybe_lock() const
			{
				return access<T>::get(maybe_lock<void const>(), &type<T>, type_,ctype_);
			}
		};

		template <class T>
		struct weak_store:: access
		{
			static std::shared_ptr<T> get( std::shared_ptr<void const> const & p, void (*pt)(), void (*type)(), void (*)() )
			{
				return p && type==pt ? std::shared_ptr<T>(p,(T *)p.get()) : std::shared_ptr<T>();
			}
		};

		template <class T>
		struct weak_store::access<T const>
		{
			static std::shared_ptr<T const> get( std::shared_ptr<void const> const & p, void (*pt)(), void (*)(), void (*ctype)() )
			{
				return p && ctype==pt ? std::shared_ptr<T const>(p,(T const *)p.get()) : std::shared_ptr<T const>();
			}
		};

		template <>
		struct weak_store::access<void>
		{
			static std::shared_ptr<void> get( std::shared_ptr<void const> const & p, void (*)(), void (*type)(), void (*)() )
			{
				return p && type ? std::shared_ptr<void>(p,(void *)p.get()) : std::shared_ptr<void>();
			}
		};

		template <>
		inline std::shared_ptr<void const> weak_store::maybe_lock<void const>() const
		{
			return px_ ? std::shared_ptr<void const>(std::shared_ptr<void>(), px_) : w_.lock();
		}
	}

} }

#if defined(_MSC_VER) && !defined(BOOST_SYNAPSE_ENABLE_WARNINGS)
#	pragma warning(pop)
#endif

#endif
