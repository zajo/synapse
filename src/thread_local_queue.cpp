//Copyright (c) 2015-2021 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/detail/config.hpp>
#include <boost/synapse/thread_local_queue.hpp>
#include <boost/synapse/connect.hpp>
#include <thread>
#include <mutex>
#include <vector>
#include <deque>
#include <algorithm>
#include <condition_variable>

namespace boost { namespace synapse {

	namespace synapse_detail
	{
		int emit_from_emitter( thread_local_signal_data::connection_list &, void const *, args_binder_base const * );

		class thread_local_signal_data::posted_signals
		{
			posted_signals( posted_signals const & );
			posted_signals & operator=( posted_signals const & );

			struct posted
			{
				unsigned serial_number;
				void const * e;
				std::shared_ptr<args_binder_base> args;
				posted()
				{
				}
				posted( unsigned serial_number, void const * e, std::shared_ptr<args_binder_base> const & args ):
					serial_number(serial_number),
					e(e),
					args(args)
				{
					BOOST_SYNAPSE_ASSERT(e!=0);
				}
			};

			std::atomic<unsigned> & emit_serial_number_;
			std::thread::id const thread_id_;
			std::mutex q_mut_;
			std::deque<posted> q_;
			std::condition_variable & wait_cond_;

		public:

			posted_signals( std::atomic<unsigned> & emit_serial_number, std::condition_variable & wait_cond ):
				emit_serial_number_(emit_serial_number),
				thread_id_(std::this_thread::get_id()),
				wait_cond_(wait_cond)
			{
			}

			bool post( void const * e, args_binder_base const * args )
			{
				BOOST_SYNAPSE_ASSERT(e!=0);
				if( thread_id_==std::this_thread::get_id() )
					return false;
				else
				{
					std::shared_ptr<args_binder_base> a;
					if( args )
						a=args->clone();
					{
						std::lock_guard<std::mutex> lk(q_mut_);
						q_.push_back(posted(emit_serial_number_++,e,a));
					}
					wait_cond_.notify_one();
					return true;
				}
			}

			int emit_if_serial_number_matches( unsigned serial_number, thread_local_signal_data const & tlsd )
			{
				posted p;
				{
					std::lock_guard<std::mutex> lk(q_mut_);
					if( q_.empty() )
						return -1;
					if( q_.front().serial_number!=serial_number )
						return -1;
					p=q_.front();
					q_.pop_front();
				}
				BOOST_SYNAPSE_ASSERT(p.e!=0);
				if( !tlsd.emitter_blocked_(tlsd,p.e) )
					if( std::shared_ptr<thread_local_signal_data::connection_list> cl=tlsd.cl_.lock() )
						return emit_from_emitter(*cl,p.e,p.args.get());
				return 0;
			}
		};

		namespace
		{
			class cl_rec
			{
				std::weak_ptr<thread_local_signal_data> tlsd_;
				std::weak_ptr<thread_local_signal_data::connection_list> cl_;
			public:

				explicit cl_rec( std::shared_ptr<thread_local_signal_data> const & tlsd ):
					tlsd_(tlsd),
					cl_(tlsd->cl_)
				{
				}

				bool expired() const
				{
					return cl_.expired() || tlsd_.expired();
				}

				std::shared_ptr<thread_local_signal_data> lock() const
				{
					return cl_.expired()? std::shared_ptr<thread_local_signal_data>() : tlsd_.lock();
				}
			};

			template <class Container>
			static void purge( Container & c )
			{
				c.erase( std::remove_if(c.begin(),c.end(), [ ]( cl_rec const & r ) { return r.expired(); }), c.end() );
			}
		}

		class connection_list_list
		{
			connection_list_list( connection_list_list const & );
			connection_list_list & operator=( connection_list_list const & );
			std::vector<cl_rec> same_signal_different_threads_;

		public:

			std::mutex mut_;

			connection_list_list()
			{
			}

			void notify_connection_list_created( std::shared_ptr<thread_local_signal_data> const & tlsd )
			{
				std::lock_guard<std::mutex> lk(mut_);
				same_signal_different_threads_.push_back(cl_rec(tlsd));
				purge(same_signal_different_threads_);
			}

			int interthread_emit( void const * e, args_binder_base const * args )
			{
				BOOST_SYNAPSE_ASSERT(e!=0);
				int count=0;
				std::lock_guard<std::mutex> lk(mut_);
				for( auto & r : same_signal_different_threads_ )
					if( std::shared_ptr<thread_local_signal_data> sp=r.lock() )
						if( sp->ps_ )
							count+=int(sp->ps_->post(e,args));
				return count;
			}
		};

		namespace
		{
			std::shared_ptr<connection_list_list> create_connection_list_list()
			{
				return std::make_shared<connection_list_list>();
			}

			class thread_local_connection_list_list
			{
				thread_local_connection_list_list( thread_local_connection_list_list const & );
				thread_local_connection_list_list & operator=( thread_local_connection_list_list const & );

				bool has_tlq_;
				std::atomic<unsigned> emit_serial_number_;
				unsigned last_poll_serial_number_;
				std::vector<cl_rec> same_thread_different_signals_;
				std::mutex wait_mut_;
				std::condition_variable wait_cond_;

			public:

				thread_local_connection_list_list():
					has_tlq_(false),
					emit_serial_number_(0),
					last_poll_serial_number_(emit_serial_number_)
				{
				}

				void notify_connection_list_created( std::shared_ptr<thread_local_signal_data> const & tlsd )
				{
					if( has_tlq_ )
					{
						std::shared_ptr<thread_local_signal_data::posted_signals> ps=std::make_shared<thread_local_signal_data::posted_signals>(emit_serial_number_,wait_cond_);
						{
							std::lock_guard<std::mutex> lk(tlsd->get_cll_(&create_connection_list_list)->mut_);
							tlsd->ps_.swap(ps);
						}
					}
					same_thread_different_signals_.push_back(cl_rec(tlsd));
					purge(same_thread_different_signals_);
				}

				void enable_tlq()
				{
					BOOST_SYNAPSE_ASSERT(!has_tlq_);
					for( auto & r : same_thread_different_signals_ )
						if( std::shared_ptr<thread_local_signal_data> sp=r.lock() )
						{
							std::shared_ptr<thread_local_signal_data::posted_signals> ps=std::make_shared<thread_local_signal_data::posted_signals>(emit_serial_number_,wait_cond_);
							{
								std::lock_guard<std::mutex> lk(sp->get_cll_(&create_connection_list_list)->mut_);
								sp->ps_.swap(ps);
							}
						}
					has_tlq_=true;
				}

				void disable_tlq()
				{
					BOOST_SYNAPSE_ASSERT(has_tlq_);
					for( auto & r : same_thread_different_signals_ )
						if( std::shared_ptr<thread_local_signal_data> sp = r.lock() )
						{
							std::shared_ptr<thread_local_signal_data::posted_signals> ps;
							{
								std::lock_guard<std::mutex> lk(sp->get_cll_(&create_connection_list_list)->mut_);
								sp->ps_.swap(ps);
							}
						}
					has_tlq_=false;
					last_poll_serial_number_=emit_serial_number_;
				}

				int poll()
				{
					unsigned current=emit_serial_number_;
					unsigned last_poll_serial_number=last_poll_serial_number_;
					last_poll_serial_number_=current;
					int count=0;
					for( unsigned serial_number=last_poll_serial_number; serial_number!=current; ++serial_number )
					{
						bool found = std::find_if( same_thread_different_signals_.begin(), same_thread_different_signals_.end(),
							[&count,serial_number]( cl_rec const & r )
							{
								if( std::shared_ptr<thread_local_signal_data> sp=r.lock() )
								{
									int n=sp->ps_->emit_if_serial_number_matches(serial_number,*sp);
									if( n>=0 )
									{
										count+=n;
										return true;
									}
									else
										BOOST_SYNAPSE_ASSERT(n==-1);
								}
								return false;
							} )!=same_thread_different_signals_.end();
						BOOST_SYNAPSE_ASSERT(found);
					}
					return count;
				}

				int wait()
				{
					for( std::unique_lock<std::mutex> lk(wait_mut_); ; )
						if( int n=poll() )
							return n;
						else
							wait_cond_.wait(lk);
				}
			};

			std::shared_ptr<thread_local_connection_list_list> get_thread_local_connection_list_list()
			{
				static thread_local std::shared_ptr<thread_local_connection_list_list> tlcll(std::make_shared<thread_local_connection_list_list>());
				return tlcll;
			}
		}

		interthread_interface * get_interthread_api()
		{
			class interthread_impl:
				public interthread_interface
			{
				void notify_connection_list_created( std::shared_ptr<thread_local_signal_data> const & tlsd ) final override
				{
					tlsd->get_cll_(&create_connection_list_list)->notify_connection_list_created(tlsd);
					get_thread_local_connection_list_list()->notify_connection_list_created(tlsd);
				}

				int emit( thread_local_signal_data const & tlsd, void const * e, args_binder_base const * args ) final override
				{
					return tlsd.get_cll_(&create_connection_list_list)->interthread_emit(e,args);
				}
			};
			static interthread_impl impl;
			return &impl;
		}
	}

	namespace
	{
		typedef struct bare_lambda_(*bare_lambda)( std::function<void()> const & );
	}

	struct thread_local_queue
	{
	private:
		thread_local_queue( thread_local_queue const & );
		thread_local_queue & operator=( thread_local_queue const & );

		std::thread::id const tid_;
		std::shared_ptr<synapse_detail::thread_local_connection_list_list> const tlcll_;

	public:

		thread_local_queue():
			tid_(std::this_thread::get_id()),
			tlcll_(synapse_detail::get_thread_local_connection_list_list())
		{
			tlcll_->enable_tlq();
		}

		~thread_local_queue()
		{
			tlcll_->disable_tlq();
		}

		int poll()
		{
			BOOST_SYNAPSE_ASSERT(tid_==std::this_thread::get_id());
			return tlcll_->poll();
		}

		int wait()
		{
			BOOST_SYNAPSE_ASSERT(tid_==std::this_thread::get_id());
			return tlcll_->wait();
		}

		void post( std::function<void()> const & f )
		{
			BOOST_SYNAPSE_ASSERT(f);
			emit<bare_lambda>(this,f);
		}
	};

	std::shared_ptr<thread_local_queue> create_thread_local_queue()
	{
		std::shared_ptr<thread_local_queue> tlq = std::make_shared<thread_local_queue>();
		(void) connect<bare_lambda>(tlq, []( std::function<void()> const & f )
			{
				BOOST_SYNAPSE_ASSERT(f);
				f();
			} );
		return tlq;
	}

	int poll( thread_local_queue & tlq )
	{
		return tlq.poll();
	}

	int wait( thread_local_queue & tlq )
	{
		return tlq.wait();
	}

	void post( thread_local_queue & tlq, std::function<void()> const & f )
	{
		tlq.post(f);
	}

} }
