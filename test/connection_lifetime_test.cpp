//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/connect.hpp>
#include <boost/synapse/connection.hpp>
#include "boost/core/lightweight_test.hpp"

namespace synapse=boost::synapse;
using synapse::shared_ptr;
using synapse::make_shared;

namespace
{
	struct my_emitter_type { };
	typedef struct my_signal_(*my_signal)();
	int connection_count;

	class connection_counter
	{
		connection_counter( connection_counter const & );
		connection_counter & operator=( connection_counter const & );
	public:
		connection_counter()
		{
			++connection_count;
		}
		~connection_counter()
		{
			--connection_count;
		}
	};

	void test_owned_connections()
	{
		BOOST_TEST_EQ(connection_count, 0);
		{
			shared_ptr<int> em=make_shared<int>(42);
			shared_ptr<synapse::connection> c1=synapse::connect<my_signal>(&em,[ ]( ){ });
			shared_ptr<synapse::connection> c2=release(synapse::connect<my_signal>(em,[ ]( ){ }));
			c1->set_user_data(make_shared<connection_counter>());
			c2->set_user_data(make_shared<connection_counter>());
			BOOST_TEST_EQ(connection_count, 2);
			em.reset();
			BOOST_TEST_EQ(connection_count, 2);
		}
		 BOOST_TEST_EQ(connection_count, 0);
	}

	void test_reset_connection()
	{
		int count1=0, count2=0;
		my_emitter_type e1;
		my_emitter_type e2;
		shared_ptr<synapse::connection> c1=synapse::connect<my_signal>(&e1,[&count1]() { ++count1; } );
		shared_ptr<synapse::connection> c2=synapse::connect<my_signal>(&e2,[&count2]() { ++count2; } );
		shared_ptr<synapse::connection> c3=synapse::connect<my_signal>(&e1,[&count1]() { ++count1; } );
		shared_ptr<synapse::connection> c4=synapse::connect<my_signal>(&e2,[&count2]() { ++count2; } );
		shared_ptr<synapse::connection> c5=synapse::connect<my_signal>(&e1,[&count1]() { ++count1; } );
		shared_ptr<synapse::connection> c6=synapse::connect<my_signal>(&e2,[&count2]() { ++count2; } );
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 3);
		BOOST_TEST_EQ(count1, 3);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 3);
		BOOST_TEST_EQ(count1, 3);
		BOOST_TEST_EQ(count2, 3);
		c1.reset();
		count1=count2=0;
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 2);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 3);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 3);
		c2.reset();
		count1=count2=0;
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 2);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 2);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
		c3.reset();
		count1=count2=0;
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 2);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 2);
		c4.reset();
		count1=count2=0;
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 1);
		c5.reset();
		count1=count2=0;
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 0);
		BOOST_TEST_EQ(count1, 0);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 1);
		BOOST_TEST_EQ(count1, 0);
		BOOST_TEST_EQ(count2, 1);
		c6.reset();
		count1=count2=0;
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 0);
		BOOST_TEST_EQ(count1, 0);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 0);
		BOOST_TEST_EQ(count1, 0);
		BOOST_TEST_EQ(count2, 0);
	}

	typedef struct my_signal_(*my_signal)();

	void test_reset_lifetime()
	{
		int count1=0, count2=0;
		my_emitter_type e1;
		my_emitter_type e2;
		shared_ptr<int> lt1(new int(1));
		shared_ptr<int> lt2(new int(2));
		shared_ptr<int> lt3(new int(3));
		shared_ptr<int> lt4(new int(4));
		shared_ptr<int> lt5(new int(5));
		shared_ptr<int> lt6(new int(6));
		(void) synapse::connect<my_signal>(&e1,lt1,[&count1]( int * x ) { BOOST_TEST_EQ(*x, 1); ++count1; });
		(void) synapse::connect<my_signal>(&e2,lt2,[&count2]( int * x ) { BOOST_TEST_EQ(*x, 2); ++count2; });
		(void) synapse::connect<my_signal>(&e1,lt3,[&count1]( int * x ) { BOOST_TEST_EQ(*x, 3); ++count1; });
		(void) synapse::connect<my_signal>(&e2,lt4,[&count2]( int * x ) { BOOST_TEST_EQ(*x, 4); ++count2; });
		(void) synapse::connect<my_signal>(&e1,lt5,[&count1]( int * x ) { BOOST_TEST_EQ(*x, 5); ++count1; });
		(void) synapse::connect<my_signal>(&e2,lt6,[&count2]( int * x ) { BOOST_TEST_EQ(*x, 6); ++count2; });
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 3);
		BOOST_TEST_EQ(count1, 3);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 3);
		BOOST_TEST_EQ(count1, 3);
		BOOST_TEST_EQ(count2, 3);
		lt1.reset();
		count1=count2=0;
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 2);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 3);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 3);
		lt2.reset();
		count1=count2=0;
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 2);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 2);
		BOOST_TEST_EQ(count1, 2);
		BOOST_TEST_EQ(count2, 2);
		lt3.reset();
		count1=count2=0;
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 2);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 2);
		lt4.reset();
		count1=count2=0;
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 1);
		BOOST_TEST_EQ(count1, 1);
		BOOST_TEST_EQ(count2, 1);
		lt5.reset();
		count1=count2=0;
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 0);
		BOOST_TEST_EQ(count1, 0);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 1);
		BOOST_TEST_EQ(count1, 0);
		BOOST_TEST_EQ(count2, 1);
		lt6.reset();
		count1=count2=0;
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 0);
		BOOST_TEST_EQ(count1, 0);
		BOOST_TEST_EQ(count2, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e2), 0);
		BOOST_TEST_EQ(count1, 0);
		BOOST_TEST_EQ(count2, 0);
	}

	struct null_deleter { void  operator()(void const *) { } };

	void test_emitter_expiring_before_connection()
	{
		int count=0;
		my_emitter_type e;
		shared_ptr<my_emitter_type> e1(&e,null_deleter());
		(void) synapse::connect<my_signal>(e1,[&count](){ ++count; });
		e1.reset();
		BOOST_TEST_EQ(count, 0);
		BOOST_TEST_EQ(synapse::emit<my_signal>(&e), 0);
	}
}

int main( int argc, char const * argv[] )
{
	test_owned_connections();
	test_reset_connection();
	test_reset_lifetime();
	test_emitter_expiring_before_connection();
	return boost::report_errors();
}
