//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/thread_local_queue.hpp>
#include <boost/synapse/connect.hpp>
#include <boost/synapse/connection.hpp>
#include "barrier.hpp"
#include <thread>
#include <vector>
#include "boost/core/lightweight_test.hpp"

namespace synapse=boost::synapse;
using synapse::shared_ptr;
using synapse::weak_ptr;
using synapse::make_shared;

namespace
{
	std::atomic<int> connection_count;
	int emitter;
	typedef struct signal1_(*signal1)();
	typedef struct signal2_(*signal2)();
	typedef struct terminate_thread_(*terminate_thread)();

	class thread_connection_counter
	{
		thread_connection_counter( thread_connection_counter const & );
		thread_connection_counter & operator=( thread_connection_counter const & );

	public:

		thread_connection_counter()
		{
			++connection_count;
		}

		~thread_connection_counter()
		{
			--connection_count;
		}
	};

	void emitting_thread( barrier & b, weak_ptr<void> const & terminate )
	{
		shared_ptr<synapse::thread_local_queue> tlq=synapse::create_thread_local_queue();
		bool keep_going=true;
		synapse::connect<terminate_thread>( terminate,
			[&keep_going]
			{
				keep_going=false;
			}).lock()->set_user_data(make_shared<thread_connection_counter>());
		b.wait();
		while( keep_going )
		{
			(void) synapse::emit<signal1>(&emitter);
			(void) synapse::emit<signal2>(&emitter);
			(void) poll(*tlq);
		}
	}

	void test( int emitting_thread_count, int per_thread_emit_count, int series_count )
	{
		assert(emitting_thread_count>0);
		assert(per_thread_emit_count>0);
		assert(series_count>0);
		std::cout << "*** " << emitting_thread_count << '/' << per_thread_emit_count << '/' << series_count << " ***" << std::endl;
		barrier b( emitting_thread_count + 1 );
		shared_ptr<int> terminate(make_shared<int>(42));
		std::vector<std::thread> tgr;
		tgr.reserve(emitting_thread_count);
		for( int i=0; i!=emitting_thread_count; ++i )
			tgr.emplace_back(
				[&b, &terminate]
				{
					emitting_thread(b,terminate);
				} );
		b.wait();
		BOOST_TEST(terminate.unique());
		BOOST_TEST_EQ(connection_count, emitting_thread_count);
		shared_ptr<synapse::connection> c1=synapse::connect<signal1>(&emitter,[](){});
		shared_ptr<synapse::connection> c2=synapse::connect<signal2>(&emitter,[](){});
		for( int i=0; i!=series_count; ++i )
		{
			shared_ptr<synapse::thread_local_queue> tlq=synapse::create_thread_local_queue();
			for( int j=0; j<emitting_thread_count*per_thread_emit_count; )
			{
				int n=poll(*tlq);
				j+=n;
			}
		}
		std::cout << "Requesting terminate..." << std::endl;
		int n=synapse::emit<terminate_thread>(terminate.get());
		BOOST_TEST_EQ(n, emitting_thread_count);
		std::cout << "Joining..." << std::endl;
		for( auto & t : tgr )
			t.join();
		std::cout << "Joined." << std::endl;
		BOOST_TEST_EQ(connection_count, 0);
	}
}

int main( int argc, char const * argv[] )
{
	test(1,1,50);
	test(20,5,50);
	test(30,10,50);
	return boost::report_errors();
}
