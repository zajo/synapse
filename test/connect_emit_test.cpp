//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/connect.hpp>
#include <functional>
#include "boost/core/lightweight_test.hpp"

namespace synapse=boost::synapse;
using synapse::shared_ptr;
using synapse::weak_ptr;
using namespace std::placeholders;
typedef struct test_signal1_(*test_signal1)();
typedef struct test_signal2_(*test_signal2)();

namespace
{
	void test_fn()
	{
		int emitter;
		int count1=0;
		int count2=0;
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 0);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 0);
		shared_ptr<synapse::connection const> c1 = synapse::connect<test_signal1>( &emitter, [&count1](){++count1;} );
		BOOST_TEST(c1.unique());
		shared_ptr<synapse::connection> c2 = synapse::connect<test_signal2>( &emitter, [&count2](){++count2;} );
		BOOST_TEST(c2.unique());
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 1);
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 1);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 1);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 1);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
		c1.reset();
		c2.reset();
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 0);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 0);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
	}

	void test_receiver_fn()
	{
		int emitter;
		int receiver;
		int count1=0;
		int count2=0;
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 0);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 0);
		shared_ptr<synapse::connection const> c1 = synapse::connect<test_signal1>( &emitter, &receiver, [&count1,&receiver]( int * r ){ BOOST_TEST(r==&receiver); ++count1;} );
		BOOST_TEST(c1.unique());
		shared_ptr<synapse::connection> c2 = synapse::connect<test_signal2>( &emitter, &receiver, [&count2,&receiver]( int * r ){ BOOST_TEST(r==&receiver); ++count2;} );
		BOOST_TEST(c2.unique());
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 1);
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 1);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 1);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 1);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
		c1.reset();
		c2.reset();
		BOOST_TEST(synapse::emit<test_signal1>(&emitter)==0);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 0);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
	}

	struct test_class
	{
		int & count;
		explicit
		test_class( int & count ):
			count(count)
		{
		}
		void
		fn()
		{
			++count;
		}
	};

	void test_receiver_mem_fn()
	{
		int emitter;
		int count1=0;
		int count2=0;
		test_class receiver1(count1);
		test_class receiver2(count2);
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 0);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 0);
		shared_ptr<synapse::connection const> c1 = synapse::connect<test_signal1>( &emitter, &receiver1, &test_class::fn );
		BOOST_TEST(c1.unique());
		shared_ptr<synapse::connection> c2 = synapse::connect<test_signal2>( &emitter, &receiver2, &test_class::fn );
		BOOST_TEST(c2.unique());
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 1);
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 1);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 1);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 1);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
		c1.reset();
		c2.reset();
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 0);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 0);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
	}

	void test_receiver_mem_fn_bind()
	{
		int emitter;
		int count1=0;
		int count2=0;
		test_class receiver1(count1);
		test_class receiver2(count2);
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 0);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 0);
		shared_ptr<synapse::connection const> c1 = synapse::connect<test_signal1>( &emitter, &receiver1, std::bind(&test_class::fn,_1) );
		BOOST_TEST(c1.unique());
		shared_ptr<synapse::connection> c2 = synapse::connect<test_signal2>( &emitter, &receiver2, std::bind(&test_class::fn,_1) );
		BOOST_TEST(c2.unique());
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 1);
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 1);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 1);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 1);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
		c1.reset();
		c2.reset();
		BOOST_TEST_EQ(synapse::emit<test_signal1>(&emitter), 0);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
		BOOST_TEST_EQ(synapse::emit<test_signal2>(&emitter), 0);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
	}
}

int main( int argc, char const * argv[] )
{
	test_fn();
	test_receiver_fn();
	test_receiver_mem_fn();
	test_receiver_mem_fn_bind();
	return boost::report_errors();
}
