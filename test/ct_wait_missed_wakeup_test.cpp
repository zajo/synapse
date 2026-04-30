//Copyright (c) 2015-2021 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/thread_local_queue.hpp>
#include <boost/synapse/connect.hpp>
#include <atomic>
#include <chrono>
#include <thread>
#include "boost/core/lightweight_test.hpp"

namespace synapse = boost::synapse;

namespace
{
	int emitter;
	int const iteration_count = 100000;
	struct poke: synapse::signal<void(int)> {};

	void test()
	{
		std::shared_ptr<synapse::thread_local_queue> tlq = synapse::create_thread_local_queue();
		std::atomic<int> requested(0);
		std::atomic<int> delivered(0);
		std::atomic<int> rescues(0);
		std::shared_ptr<synapse::connection const> c = synapse::connect<poke>(&emitter,
			[&delivered]( int n )
			{
				int old = delivered.load(std::memory_order_relaxed);
				while( old < n && !delivered.compare_exchange_weak(old, n, std::memory_order_release, std::memory_order_relaxed) )
					{ }
			} );
		std::thread producer(
			[&]
			{
				for( int i = 1; i <= iteration_count; ++i )
				{
					while( requested.load(std::memory_order_acquire) < i )
						std::this_thread::yield();
					synapse::emit<poke>(&emitter, i);
					std::chrono::steady_clock::time_point const deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(100);
					while( delivered.load(std::memory_order_acquire) < i && std::chrono::steady_clock::now() < deadline )
						std::this_thread::yield();
					if( delivered.load(std::memory_order_acquire) < i )
					{
						++rescues;
						synapse::emit<poke>(&emitter, i);
					}
				}
			} );
		for( int i = 1; i <= iteration_count; ++i )
		{
			requested.store(i, std::memory_order_release);
			BOOST_TEST_GT(synapse::wait(*tlq), 0);
			BOOST_TEST_GE(delivered.load(std::memory_order_acquire), i);
		}
		producer.join();
		BOOST_TEST_EQ(rescues.load(std::memory_order_acquire), 0);
		BOOST_TEST_EQ(delivered.load(std::memory_order_acquire), iteration_count);
	}
}

int main( int argc, char const * argv[] )
{
	test();
	return boost::report_errors();
}
