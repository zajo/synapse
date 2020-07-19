//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/thread_local_queue.hpp>
#include <boost/synapse/connect.hpp>
#include <thread>
#include <vector>
#include "boost/core/lightweight_test.hpp"

namespace synapse=boost::synapse;
using synapse::shared_ptr;
using synapse::weak_ptr;

namespace
{
	int const thread_count=10;
	int const iteration_count=1000;

	void emitting_thread( weak_ptr<synapse::thread_local_queue> tlq, int & counter, std::thread::id tid )
	{
		for(;;)
			if( shared_ptr<synapse::thread_local_queue> p=tlq.lock() )
				post( *p, [&counter, tid]()
					{
						BOOST_TEST_EQ(std::this_thread::get_id(), tid);
						++counter;
					} );
			else
				break;
	}

	void consuming_thread()
	{
		assert(iteration_count>0);
		int count=0;
		shared_ptr<synapse::thread_local_queue> tlq=synapse::create_thread_local_queue();
		std::thread::id const tid = std::this_thread::get_id();
		std::thread th( [&]
			{
				emitting_thread( tlq, count, tid );
			} );
		while( count<iteration_count )
			poll(*tlq);
		tlq.reset();
		th.join();
	}
}

int main( int argc, char const * argv[] )
{
	std::vector<std::thread> tgr;
	tgr.reserve(thread_count);
	for( int i=0; i!=thread_count; ++i )
		tgr.emplace_back(&consuming_thread);
	for( auto & t : tgr )
		t.join();
	return boost::report_errors();
}
