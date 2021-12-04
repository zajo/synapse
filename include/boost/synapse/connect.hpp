#ifndef BOOST_SYNAPSE_CONNECT_HPP_INCLUDED
#define BOOST_SYNAPSE_CONNECT_HPP_INCLUDED

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

#include <boost/synapse/emit.hpp>
#include <boost/synapse/detail/weak_store.hpp>

namespace boost { namespace synapse {

	class connection;

	template <class Signal, class Emitter, class F>
	std::shared_ptr<connection> connect( Emitter *, F );

	template <class Signal, class Emitter, class Receiver, class F>
	std::shared_ptr<connection> connect( Emitter *, Receiver *, F );

	class pconnection;

	template <class Signal, class Emitter, class F>
	std::weak_ptr<pconnection> connect( std::weak_ptr<Emitter> const &, F );

	template <class Signal, class Emitter, class F>
	std::weak_ptr<pconnection> connect( std::shared_ptr<Emitter> const &, F );

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( Emitter *, std::weak_ptr<Receiver> const &, F );

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( Emitter *, std::shared_ptr<Receiver> const &, F );

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( std::weak_ptr<Emitter> const &, Receiver *, F );

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( std::weak_ptr<Emitter> const &, std::weak_ptr<Receiver> const &, F );

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( std::weak_ptr<Emitter> const &, std::shared_ptr<Receiver> const &, F );

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( std::shared_ptr<Emitter> const &, Receiver *, F );

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( std::shared_ptr<Emitter> const &, std::weak_ptr<Receiver> const &, F );

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( std::shared_ptr<Emitter> const &, std::shared_ptr<Receiver> const &, F );

	std::shared_ptr<connection const> release( std::weak_ptr<pconnection const> const & );
	std::shared_ptr<connection> release( std::weak_ptr<pconnection> const & );

	namespace meta
	{
		std::weak_ptr<void const> emitter();
		namespace connect_flags
		{
			unsigned const connecting=1;
			unsigned const first_for_this_emitter=2;
			unsigned const last_for_this_emitter=4;
		}

		template <class Signal>
		struct connected
		{
			typedef connected<Signal>(*type)( connection &, unsigned connect_flags );
		};
	}

} }

// Implementation details below.

namespace boost { namespace synapse {

	namespace synapse_detail
	{
		std::shared_ptr<connection> connect_( std::shared_ptr<thread_local_signal_data> const &, weak_store && e, weak_store && r, std::shared_ptr<void const> const &, int(*)(connection &,unsigned) );
		std::shared_ptr<pconnection> pconnect_( std::shared_ptr<thread_local_signal_data> const &, weak_store && e, weak_store && r, std::shared_ptr<void const> const &, int(*)(connection &,unsigned) );

		////////////////////////////////////////////////////////

		template <class Signal, class F>
		std::shared_ptr<connection> connect_fwd( weak_store && e, F f )
		{
			return connect_(
				get_thread_local_signal_data<Signal>(true),
				std::move(e),
				weak_store(),
				std::make_shared<std::function<typename signal_traits<Signal>::signature> >(f),
				&emit_meta_connected<Signal> );
		}

		template <class Signal, class F>
		std::shared_ptr<pconnection> pconnect_fwd( weak_store && e, F f )
		{
			return pconnect_(
				get_thread_local_signal_data<Signal>(true),
				std::move(e),
				weak_store(),
				std::make_shared<std::function<typename signal_traits<Signal>::signature> >(f),
				&emit_meta_connected<Signal> );
		}

		template <class Signal, class Receiver, class F, class Signature>
		struct bind_front;

		template <class Signal, class Receiver, class Rm, class... Am, class R, class... A>
		struct bind_front<Signal,Receiver,Rm (Receiver::*)(Am...),R(A...)>
		{
			static std::shared_ptr<connection> connect_fwd( weak_store && e, weak_store && r, Rm (Receiver::*f)(Am...) )
			{
				return connect_(
					get_thread_local_signal_data<Signal>(true),
					std::move(e),
					std::move(r),
					std::make_shared<std::function<typename signal_traits<Signal>::signature> >([=]( A... a ) { return (r.maybe_lock<Receiver>().get()->*f)(a...); }),
					&emit_meta_connected<Signal> );
			}

			static std::shared_ptr<pconnection> pconnect_fwd( weak_store && e, weak_store && r, Rm (Receiver::*f)(Am...) )
			{
				return pconnect_(
					get_thread_local_signal_data<Signal>(true),
					std::move(e),
					std::move(r),
					std::make_shared<std::function<typename signal_traits<Signal>::signature> >([=]( A... a ) { return (r.maybe_lock<Receiver>().get()->*f)(a...); }),
					&emit_meta_connected<Signal> );
			}
		};

		template <class Signal, class Receiver, class F, class R, class... A>
		struct bind_front<Signal,Receiver,F,R(A...)>
		{
			static std::shared_ptr<connection> connect_fwd( weak_store && e, weak_store && r, F f )
			{
				return connect_(
					get_thread_local_signal_data<Signal>(true),
					std::move(e),
					std::move(r),
					std::make_shared<std::function<typename signal_traits<Signal>::signature> >([=]( A... a ) { return f(&*r.maybe_lock<Receiver>(),a...); }),
					&emit_meta_connected<Signal> );
			}

			static std::shared_ptr<pconnection> pconnect_fwd( weak_store && e, weak_store && r, F f )
			{
				return pconnect_(
					get_thread_local_signal_data<Signal>(true),
					std::move(e),
					std::move(r),
					std::make_shared<std::function<typename signal_traits<Signal>::signature> >([=]( A... a ) { return f(&*r.maybe_lock<Receiver>(),a...); }),
					&emit_meta_connected<Signal> );
			}
		};
	}

	template <class Signal, class Emitter, class F>
	std::shared_ptr<connection> connect( Emitter * e, F f )
	{
		return synapse_detail::connect_fwd<Signal>(e,f);
	}

	template <class Signal, class Emitter, class Receiver, class F>
	std::shared_ptr<connection> connect( Emitter * e, Receiver * r, F f )
	{
		return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::connect_fwd(e,r,f);
	}

	template <class Signal, class Emitter, class F>
	std::weak_ptr<pconnection> connect( std::weak_ptr<Emitter> const & e, F f )
	{
		return synapse_detail::pconnect_fwd<Signal>(e,f);
	}

	template <class Signal, class Emitter, class F>
	std::weak_ptr<pconnection> connect( std::shared_ptr<Emitter> const & e, F f )
	{
		return synapse_detail::pconnect_fwd<Signal>(e,f);
	}

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( Emitter * e, std::weak_ptr<Receiver> const & r, F f )
	{
		return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
	}

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( Emitter * e, std::shared_ptr<Receiver> const & r, F f )
	{
		return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
	}

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( std::weak_ptr<Emitter> const & e, Receiver * r, F f )
	{
		return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
	}

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( std::weak_ptr<Emitter> const & e, std::weak_ptr<Receiver> const & r, F f )
	{
		return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
	}

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( std::weak_ptr<Emitter> const & e, std::shared_ptr<Receiver> const & r, F f )
	{
		return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
	}

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( std::shared_ptr<Emitter> const & e, Receiver * r, F f )
	{
		return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
	}

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( std::shared_ptr<Emitter> const & e, std::weak_ptr<Receiver> const & r, F f )
	{
		return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
	}

	template <class Signal, class Emitter, class Receiver, class F>
	std::weak_ptr<pconnection> connect( std::shared_ptr<Emitter> const & e, std::shared_ptr<Receiver> const & r, F f )
	{
		return synapse_detail::bind_front<Signal,Receiver,F,typename signal_traits<Signal>::signature>::pconnect_fwd(e,r,f);
	}

} }

#if defined(_MSC_VER) && !defined(BOOST_SYNAPSE_ENABLE_WARNINGS)
#	pragma warning(pop)
#endif

#endif
