//Copyright (c) 2015-2021 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/connect.hpp>
#include <boost/synapse/connection.hpp>
#include <vector>
#include <deque>
#include <algorithm>

namespace
{
#ifdef NDEBUG
	bool const debug=false;
#else
	bool const debug=true;
#endif
}

namespace boost { namespace synapse {

	namespace synapse_detail
	{
		interthread_interface * get_interthread_api();

		namespace
		{
			class conn_rec
			{
				void const * ep_;
				weak_store e_;
				weak_store r_;
				std::shared_ptr<void const> fn_;
				std::shared_ptr<pconnection> c_;
				int next_;
				bool translated_;

			public:

				conn_rec( weak_store && e, weak_store && r, std::shared_ptr<void const> const & fn, std::shared_ptr<pconnection> const & c, bool translated ):
					ep_(e.maybe_lock<void const>().get()),
					e_(std::move(e)),
					r_(std::move(r)),
					fn_(fn),
					c_(c),
					next_(-1),
					translated_(translated)
				{
					BOOST_SYNAPSE_ASSERT(ep_!=0);
					BOOST_SYNAPSE_ASSERT(fn);
				}

				std::shared_ptr<pconnection> release()
				{
					std::shared_ptr<pconnection> c; c.swap(c_);
					return c;
				}

				weak_store const & emitter() const
				{
					return e_;
				}

				bool same_emitter( void const * e ) const
				{
					return e==ep_;
				}

				weak_store const & receiver() const
				{
					return r_;
				}

				bool is_free() const
				{
					bool fr=(ep_==0);
					BOOST_SYNAPSE_ASSERT(e_.empty()==fr);
					BOOST_SYNAPSE_ASSERT(!fn_==fr);
					BOOST_SYNAPSE_ASSERT(r_.expired() || !fr);
					return fr;
				}

				bool expired() const
				{
					BOOST_SYNAPSE_ASSERT(!is_free());
					return e_.expired() || r_.expired();
				}

				void clear( int first_free )
				{
					BOOST_SYNAPSE_ASSERT(first_free==-1 || first_free>=0);
					BOOST_SYNAPSE_ASSERT(!is_free());
					ep_=0;
					e_.clear();
					r_.clear();
					fn_.reset();
					c_.reset();
					next_=first_free;
					BOOST_SYNAPSE_ASSERT(is_free());
				}

				int const & next() const
				{
					BOOST_SYNAPSE_ASSERT(next_==-1 || next_>=0);
					return next_;
				}

				int & next()
				{
					BOOST_SYNAPSE_ASSERT(next_==-1 || next_>=0);
					return next_;
				}

				int emit( void const * e, args_binder_base const * args ) const
				{
					BOOST_SYNAPSE_ASSERT(!is_free());
					BOOST_SYNAPSE_ASSERT(e!=0);
					if( ep_==e )
						if( auto e_lk = e_.maybe_lock<void const>() )
							if( auto r_lk=r_.maybe_lock<void const>() )
								if( translated_ )
									if( args )
										return args->call_translated(fn_.get());
									else
										return (*static_cast<std::function<int()> const *>(fn_.get()))();
								else
								{
									if( args )
										args->call(fn_.get());
									else
										(*static_cast<std::function<void()> const *>(fn_.get()))();
									return 1;
								}
					return 0;
				};
			};
		}

		class thread_local_signal_data::connection_list
		{
			connection_list( connection_list const & );
			connection_list & operator=( connection_list const & );

			std::weak_ptr<thread_local_signal_data> const tlsd_;
			std::vector<conn_rec> conn_;
			std::vector<conn_rec> * emit_conn_ptr_;
			int first_free_;
			int first_rec_;
			int * last_next_;

			void check_invariants() const
			{
				if( debug )
				{
					BOOST_SYNAPSE_ASSERT(*last_next_==-1);
					int count1=0;
					for( int i=first_free_; i!=-1; i=conn_[i].next() )
					{
						BOOST_SYNAPSE_ASSERT(conn_[i].is_free());
						++count1;
					}
					int count2=0;
					bool found_last_next=false;
					for( int const * i=&first_rec_; ; )
					{
						if( last_next_==i )
							found_last_next=true;
						 if( *i==-1 )
							 break;
						conn_rec const & cr=conn_[*i];
						BOOST_SYNAPSE_ASSERT(!cr.is_free());
						++count2;
						i=&cr.next();
					}
					BOOST_SYNAPSE_ASSERT(found_last_next);
					int count3=0;
					for( std::vector<conn_rec>::const_iterator i=conn_.begin(),ie=conn_.end(); i!=ie; ++i )
						count3 += i->is_free();
					BOOST_SYNAPSE_ASSERT(count1==count3);
					BOOST_SYNAPSE_ASSERT(count1+count2==conn_.size());
				}
			}

			bool idx_valid( int idx ) const
			{
				return idx>=0 && idx<conn_.size();
			}

			static int emit_impl( thread_local_signal_data const & tlsd, void const * e, args_binder_base const * args )
			{
				BOOST_SYNAPSE_ASSERT(tlsd.emitter_blocked_!=0);
				BOOST_SYNAPSE_ASSERT(e!=0);
				BOOST_SYNAPSE_ASSERT(!tlsd.cl_.expired());
				int n1=0;
				if( tlsd.interthread_ )
					if( interthread_interface * interthread=tlsd.interthread_->load() )
						n1=interthread->emit(tlsd,e,args);
				if( !tlsd.emitter_blocked_(tlsd,e) )
					if( std::shared_ptr<thread_local_signal_data::connection_list> cl=tlsd.cl_.lock() )
					{
						int n2=cl->emit_from_emitter(e,args);
						return n1+n2;
					}
					else
						BOOST_SYNAPSE_ASSERT(0);
				return n1;
			}

			static void cleanup_impl( thread_local_signal_data const & tlsd )
			{
				std::shared_ptr<connection_list> cl=tlsd.cl_.lock();
				BOOST_SYNAPSE_ASSERT(cl);
				cl->cleanup();
				cl->destroy();
			}

			void destroy()
			{
				if( std::shared_ptr<thread_local_signal_data> tlsd=tlsd_.lock() )
				{
					BOOST_SYNAPSE_ASSERT(tlsd);
					if( tlsd->cl_count_ )
					{
						int const n=--(*tlsd->cl_count_);
						BOOST_SYNAPSE_ASSERT(n>=0);
					}
					tlsd->cl_.reset();
					tlsd->emit_=&thread_local_signal_data::emit_stub;
					tlsd->cleanup_=&thread_local_signal_data::cleanup_stub;
				}
			}

			bool empty() const
			{
				return first_rec_==-1;
			}

		public:

		int (* const emit_meta_connected_)( connection &, unsigned );

		connection_list( std::shared_ptr<thread_local_signal_data> const & tlsd, int (*emit_meta_connected)( connection &, unsigned ) ):
			tlsd_(tlsd),
			emit_conn_ptr_(0),
			first_free_(-1),
			first_rec_(-1),
			last_next_(&first_rec_),
			emit_meta_connected_(emit_meta_connected)
		{
			BOOST_SYNAPSE_ASSERT(emit_meta_connected_!=0);
			tlsd->emit_=&emit_impl;
			tlsd->cleanup_=&cleanup_impl;
			check_invariants();
			if( tlsd->cl_count_ )
			{
				int const n=++(*tlsd->cl_count_);
				BOOST_SYNAPSE_ASSERT(n>0);
			}
		}

		~connection_list()
			{
				BOOST_SYNAPSE_ASSERT(empty());
				destroy();
			}

		weak_store const & emitter( int index ) const
			{
				return conn_[index].emitter();
			}

		weak_store const & receiver( int index ) const
			{
				return conn_[index].receiver();
			}

			std::shared_ptr<pconnection> release( int index )
			{
				return conn_[index].release();
			}

			int add( conn_rec const & r )
			{
				if( emit_conn_ptr_ && emit_conn_ptr_->empty() )
					*emit_conn_ptr_=conn_;
				int idx;
				if( first_free_!=-1 )
				{
					BOOST_SYNAPSE_ASSERT(idx_valid(first_free_));
					idx=first_free_;
					conn_rec & cr=conn_[idx];
					BOOST_SYNAPSE_ASSERT(cr.is_free());
					first_free_=cr.next();
					cr=r;
					*last_next_=idx;
				}
				else
				{
					idx=conn_.size();
					*last_next_=idx;
					conn_.push_back(r);
				}
				last_next_=&conn_[idx].next();
				BOOST_SYNAPSE_ASSERT(first_free_!=idx);
				BOOST_SYNAPSE_ASSERT(!conn_[idx].is_free());
				check_invariants();
				return idx;
			}

			void remove( int idx )
			{
				check_invariants();
				BOOST_SYNAPSE_ASSERT(idx_valid(idx));
				BOOST_SYNAPSE_ASSERT(first_free_!=idx);
				if( emit_conn_ptr_ && emit_conn_ptr_->empty() )
					*emit_conn_ptr_=conn_;
				conn_rec & cr=conn_[idx];
				BOOST_SYNAPSE_ASSERT(!cr.is_free());
				int * i;
				for( i=&first_rec_; *i!=-1 && *i!=idx; i=&conn_[*i].next() )
					{ }
				int const j=(*i=cr.next());
				if( j==-1 )
				{
					BOOST_SYNAPSE_ASSERT(last_next_==&cr.next());
					last_next_=i;
				}
				cr.clear(first_free_);
				first_free_=idx;
				check_invariants();
			}

			template <class F>
			int enumerate_recs( F f )
			{
				check_invariants();
				if( conn_.empty() )
					return 0;
				struct restore_emit_conn_ptr
				{
					std::vector<conn_rec> * & emit_conn_ptr_;
					std::vector<conn_rec> * const ptr_;
					explicit restore_emit_conn_ptr( std::vector<conn_rec> * & emit_conn_ptr ): emit_conn_ptr_(emit_conn_ptr), ptr_(emit_conn_ptr_) { }
					~restore_emit_conn_ptr() { emit_conn_ptr_=ptr_; }
				} restore(emit_conn_ptr_);

				std::vector<conn_rec> emit_conn;
				emit_conn_ptr_=&emit_conn;
				int counter=0;
				for( int i=first_rec_; i!=-1; )
				{
					conn_rec & cr = (emit_conn.empty()?conn_:emit_conn)[i];
					i=cr.next();
					counter += f(cr);
				}
				return counter;
			}

			int emitter_connection_count( void const * e )
			{
				return e==0?0:enumerate_recs( [e](conn_rec const & r) { return r.same_emitter(e); } );
			}

			int emit_from_emitter( void const * e, args_binder_base const * args )
			{
				BOOST_SYNAPSE_ASSERT(e!=0);
				return enumerate_recs( [e,args](conn_rec const & r) { return r.emit(e,args); } );
			}

			void purge()
			{
				check_invariants();
				std::deque<std::shared_ptr<pconnection> > purged;
				for( int const * i=&first_rec_; *i!=-1; )
				{
					conn_rec & cr=conn_[*i];
					BOOST_SYNAPSE_ASSERT(!cr.is_free());
					if( cr.expired() )
						if( std::shared_ptr<pconnection> c=cr.release() )
							purged.push_back(c);
					i=&cr.next();
				}
			}

			void cleanup()
			{
				check_invariants();
#ifndef BOOST_SYNAPSE_NO_EXCEPTIONS
				try
#endif
				{
					std::deque<std::shared_ptr<pconnection> > purged;
					for( int const * i=&first_rec_; *i!=-1; )
					{
						conn_rec & cr=conn_[*i];
						BOOST_SYNAPSE_ASSERT(!cr.is_free());
						if( std::shared_ptr<pconnection> c=cr.release() )
							purged.push_back(c);
						i=&cr.next();
					}
				}
#ifndef BOOST_SYNAPSE_NO_EXCEPTIONS
				catch(...)
				{
				}
#endif
			}
		};

		int emit_from_emitter( thread_local_signal_data::connection_list & cl, void const * e, args_binder_base const * args )
		{
			return cl.emit_from_emitter(e,args);
		}

		namespace
		{
			std::shared_ptr<thread_local_signal_data::connection_list>
			get_connection_list_( std::shared_ptr<thread_local_signal_data> const & tlsd, int (*emit_meta_connected)(connection &,unsigned) )
			{
				std::shared_ptr<thread_local_signal_data::connection_list> cl=tlsd->cl_.lock();
				if( !cl )
				{
					std::make_shared<thread_local_signal_data::connection_list>(tlsd,emit_meta_connected).swap(cl);
					tlsd->cl_=cl;
					if( tlsd->interthread_ )
						if( interthread_interface * interthread=get_interthread_api() )
						{
							tlsd->interthread_->store(interthread);
							interthread->notify_connection_list_created(tlsd);
						}
				}
				return cl;
			}

			template <class Base>
			class connection_impl: public Base
			{
				connection_impl( connection_impl const & );
				connection_impl & operator=( connection_impl const & );
				std::shared_ptr<thread_local_signal_data::connection_list> const cl_;
				int position_;

				weak_store const & emitter_() const final override
				{
					return cl_->emitter(position_);
				}

				weak_store const & receiver_() const final override
				{
					return cl_->receiver(position_);
				}

			public:

				explicit connection_impl( std::shared_ptr<thread_local_signal_data::connection_list> const & cl ):
					cl_(cl)
				{
				}

				void connect( conn_rec const & cr )
				{
					BOOST_SYNAPSE_ASSERT(cl_->emit_meta_connected_!=0);
					(void) cl_->purge();
					position_=cl_->add(cr);
					unsigned flags=meta::connect_flags::connecting;
					if( cl_->emitter_connection_count(emitter_().template maybe_lock<void const>().get())==1 )
						flags |= meta::connect_flags::first_for_this_emitter;
					cl_->emit_meta_connected_(*this,flags);
				}

				~connection_impl()
				{
					BOOST_SYNAPSE_ASSERT(cl_->emit_meta_connected_!=0);
					unsigned flags=0;
					if( cl_->emitter_connection_count(emitter_().template maybe_lock<void const>().get())==1 )
						flags |= meta::connect_flags::last_for_this_emitter;
#ifndef BOOST_SYNAPSE_NO_EXCEPTIONS
					try
#endif
					{
						cl_->emit_meta_connected_(*this,flags);
					}
#ifndef BOOST_SYNAPSE_NO_EXCEPTIONS
					catch(...)
					{
					}
#endif
					cl_->remove(position_);
				}

				std::shared_ptr<connection const> release() const
				{
					std::shared_ptr<void> pc = cl_->release(position_);
					return std::shared_ptr<connection const>(pc,static_cast<connection const *>(this));
				}

				std::shared_ptr<connection> release()
				{
					std::shared_ptr<void> pc = cl_->release(position_);
					return std::shared_ptr<connection>(pc,static_cast<connection *>(this));
				}
			};

			std::shared_ptr<connection> connect_impl( std::shared_ptr<thread_local_signal_data> const & tlsd, weak_store && e, weak_store && r, std::shared_ptr<void const> const & fn, int(*emit_meta_connected)(connection &,unsigned), bool translated )
			{
				BOOST_SYNAPSE_ASSERT(fn);
				auto c=std::make_shared<connection_impl<connection> >(get_connection_list_(tlsd,emit_meta_connected));
				c->connect(
					conn_rec(
						std::move(e),
						r.empty() ? weak_store(std::weak_ptr<connection_impl<connection> >(c)) : std::move(r),
						fn,
						std::shared_ptr<pconnection>(),
						translated ) );
				return c;
			}

			std::shared_ptr<pconnection> pconnect_impl( std::shared_ptr<thread_local_signal_data> const & tlsd, weak_store && e, weak_store && r, std::shared_ptr<void const> const & fn, int(*emit_meta_connected)(connection &,unsigned), bool translated )
			{
				BOOST_SYNAPSE_ASSERT(fn);
				BOOST_SYNAPSE_ASSERT(e.lockable() || r.lockable());
				auto c=std::make_shared<connection_impl<pconnection> >(get_connection_list_(tlsd,emit_meta_connected));
				c->connect(
					conn_rec(
						std::move(e),
						r.empty() ? weak_store(std::weak_ptr<connection_impl<pconnection> >(c)) : std::move(r),
						fn,
						c,
						translated ) );
				return c;
			}
		}

		std::shared_ptr<connection> connect_( std::shared_ptr<thread_local_signal_data> const & tlsd, weak_store && e, weak_store && r, std::shared_ptr<void const> const & fn, int(*emit_meta_connected)(connection &,unsigned) )
		{
			return connect_impl(tlsd,std::move(e),std::move(r),fn,emit_meta_connected,false);
		}

		std::shared_ptr<pconnection> pconnect_( std::shared_ptr<thread_local_signal_data> const & tlsd, weak_store && e, weak_store && r, std::shared_ptr<void const> const & fn, int(*emit_meta_connected)(connection &,unsigned) )
		{
			return pconnect_impl(tlsd,std::move(e),std::move(r),fn,emit_meta_connected,false);
		}

		std::shared_ptr<connection> connect_translated_( std::shared_ptr<thread_local_signal_data> const & tlsd, weak_store && e, weak_store && r, std::shared_ptr<void const> const & fn, int(*emit_meta_connected)(connection &,unsigned) )
		{
			return connect_impl(tlsd,std::move(e),std::move(r),fn,emit_meta_connected,true);
		}

		std::shared_ptr<pconnection> pconnect_translated_( std::shared_ptr<thread_local_signal_data> const & tlsd, weak_store && e, weak_store && r, std::shared_ptr<void const> const & fn, int(*emit_meta_connected)(connection &,unsigned) )
		{
			return pconnect_impl(tlsd,std::move(e),std::move(r),fn,emit_meta_connected,true);
		}

		std::shared_ptr<void const> & meta_emitter()
		{
			static std::shared_ptr<void const> me(std::make_shared<int>(42));
			return me;
		}
	}

	namespace meta
	{
		std::weak_ptr<void const> emitter()
		{
			return synapse_detail::meta_emitter();
		}
	}

	connection::connection()
	{
	}

	connection::~connection()
	{
	}

	pconnection::pconnection()
	{
	}

	pconnection::~pconnection()
	{
	}

	std::shared_ptr<connection const> release( std::weak_ptr<pconnection const> const & c )
	{
		std::shared_ptr<pconnection const> sp = c.lock();
		std::shared_ptr<connection const> released = static_cast<synapse_detail::connection_impl<pconnection> const *>(sp.get())->release();
		return released ? released : std::shared_ptr<connection const>();
	}

	std::shared_ptr<connection> release( std::weak_ptr<pconnection> const & c )
	{
		std::shared_ptr<pconnection> sp = c.lock();
		std::shared_ptr<connection> released = static_cast<synapse_detail::connection_impl<pconnection> *>(sp.get())->release();
		return released ? released : std::shared_ptr<connection>();
	}

} }
