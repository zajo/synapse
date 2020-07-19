//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/connect.hpp>
#include "boost/core/lightweight_test.hpp"

namespace synapse=boost::synapse;
using synapse::shared_ptr;

namespace
{
	struct my_emitter_type { };
	typedef struct my_signal_(*my_signal)();

	void test_connection_expiration_during_emit1()
	{
		my_emitter_type e;
		int count0=0, count1=0, count2=0;
		shared_ptr<synapse::connection> c1;
		shared_ptr<synapse::connection> c0=synapse::connect<my_signal>(&e,
			[&c1,&count0]()
			{
				if( ++count0==3 )
					c1.reset();
			} );
		c1=synapse::connect<my_signal>(&e,[&count1]() { ++count1; } );
		shared_ptr<synapse::connection> c2=synapse::connect<my_signal>(&e,[&count2]() { ++count2; } );
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e), 3);
		BOOST_TEST(c1);
		BOOST_TEST_EQ(count0, 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 1);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e), 3);
		BOOST_TEST(c1);
		BOOST_TEST_EQ(count0, 2);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e), 2);
		BOOST_TEST(!c1);
		BOOST_TEST_EQ(count0, 3);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 3);
	}

	void test_connection_expiration_during_emit2()
	{
		my_emitter_type e;
		int count0=0, count1=0, count2=0;
		shared_ptr<int> lifetime(new int(42));
		shared_ptr<synapse::connection> c0=synapse::connect<my_signal>(&e,
			[&lifetime,&count0]()
			{
				if( ++count0==3 )
					lifetime.reset();
			} );
		(void) synapse::connect<my_signal>(&e,lifetime,[&count1]( int * x ) { BOOST_TEST_EQ(*x, 42); ++count1; } );
		shared_ptr<synapse::connection> c2=synapse::connect<my_signal>(&e,[&count2]() { ++count2; } );
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e), 3);
		BOOST_TEST(lifetime);
		BOOST_TEST_EQ(count0, 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 1);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e), 3);
		BOOST_TEST(lifetime);
		BOOST_TEST_EQ(count0, 2);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e), 2);
		BOOST_TEST(!lifetime);
		BOOST_TEST_EQ(count0, 3);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 3);
	}

	struct null_deleter { template <class T> void operator()( T * ) { } };

	void emitter_address_reuse_test()
	{
		my_emitter_type e2;
		int n1=0;
		shared_ptr<my_emitter_type> e1(&e2,null_deleter());
		(void) synapse::connect<my_signal>(e1,[&n1]() { ++n1; } );
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 1);
		BOOST_TEST_EQ(n1, 1);
		e1.reset();
		int n2=0;
		shared_ptr<synapse::connection> c2=synapse::connect<my_signal>(&e2,[&n2]() { ++n2; } );
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 1);
		BOOST_TEST_EQ(n1, 1);
		BOOST_TEST_EQ(n2, 1);
		c2.reset();
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 0);
		BOOST_TEST_EQ(n1, 1);
		BOOST_TEST_EQ(n2, 1);
	}
}

int main( int argc, char const * argv[] )
{
	test_connection_expiration_during_emit1();
	test_connection_expiration_during_emit2();
	emitter_address_reuse_test();
	return boost::report_errors();
}
