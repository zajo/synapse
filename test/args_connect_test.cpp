//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/connect.hpp>
#include <boost/synapse/connection.hpp>
#include "boost/core/lightweight_test.hpp"
#include <string>

namespace synapse=boost::synapse;
using synapse::shared_ptr;
using synapse::weak_ptr;

namespace
{

	struct my_emitter_type { };

	//////////////////////////////////////////////////////////////////////////////////////////////////

	typedef struct signal1_a0_(*signal1_a0)();
	typedef struct signal2_a0_(*signal2_a0)();

	void test_a0()
	{
		int connect_count=0;
		int count=0;
		my_emitter_type e1;
		shared_ptr<synapse::connection> mc_c1 = release(synapse::connect<synapse::meta::connected<signal1_a0> >(synapse::meta::emitter(),
			[&e1,&connect_count]( synapse::connection & c, unsigned flags )
			{
				if( flags&synapse::meta::connect_flags::connecting )
				{
					BOOST_TEST_EQ(&e1, c.emitter<my_emitter_type>().get());
					++connect_count;
				}
				else
					--connect_count;
			} ) );
		BOOST_TEST(mc_c1.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> c1=synapse::connect<signal1_a0>(&e1,
			[&count]()
			{
				++count;
			} );
		BOOST_TEST_EQ(connect_count, 1);
		BOOST_TEST_EQ(synapse::emit<signal2_a0>(&e1), 0);
		BOOST_TEST_EQ(count, 0);
		BOOST_TEST_EQ(synapse::emit<signal1_a0>(&e1), 1);
		BOOST_TEST_EQ(count, 1);
		c1.reset();
		BOOST_TEST_EQ(connect_count, 0);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a0>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	typedef struct signal0_a1_(*signal0_a1)(short);
	typedef struct signal1_a1_(*signal1_a1)(int);
	typedef struct signal2_a1_(*signal2_a1)(int);

	void test_a1()
	{
		int connect_count=0;
		int count=0;
		my_emitter_type e1;
		shared_ptr<synapse::connection> mc_c1 = release(synapse::connect<synapse::meta::connected<signal1_a1> >(synapse::meta::emitter(),
			[&e1,&connect_count]( synapse::connection & c, unsigned flags )
			{
				if( flags&synapse::meta::connect_flags::connecting )
				{
					BOOST_TEST_EQ(&e1, c.emitter<my_emitter_type>().get());
					++connect_count;
				}
				else
					--connect_count;
			} ) );
		BOOST_TEST(mc_c1.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> c1=synapse::connect<signal1_a1>(&e1,
			[&count]( int a1 )
			{
				BOOST_TEST_EQ(a1, 42);
				++count;
			} );
		BOOST_TEST_EQ(connect_count, 1);
		BOOST_TEST_EQ(synapse::emit<signal2_a1>(&e1,42), 0);
		BOOST_TEST_EQ(count, 0);
		BOOST_TEST_EQ(synapse::emit<signal1_a1>(&e1,42), 1);
		BOOST_TEST_EQ(count, 1);
		c1.reset();
		BOOST_TEST_EQ(connect_count, 0);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a1>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	typedef struct signal0_a2_(*signal0_a2)(int,double);
	typedef struct signal1_a2_(*signal1_a2)(int,float);
	typedef struct signal2_a2_(*signal2_a2)(int,float);

	void test_a2()
	{
		int connect_count=0;
		int count=0;
		my_emitter_type e1;
		shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a2> >(synapse::meta::emitter(),
			[&e1,&connect_count]( synapse::connection & c, unsigned flags )
			{
				if( flags&synapse::meta::connect_flags::connecting )
				{
					BOOST_TEST_EQ(&e1, c.emitter<my_emitter_type>().get());
					++connect_count;
				}
				else
					--connect_count;
			} ) );
		BOOST_TEST(mc_c1.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> c1=synapse::connect<signal1_a2>(&e1,
			[&count]( int a1, float a2 )
			{
				BOOST_TEST_EQ(a1, 42);
				BOOST_TEST_EQ(a2, 42.42f);
				++count;
			} );
		BOOST_TEST_EQ(connect_count, 1);
		BOOST_TEST_EQ(synapse::emit<signal2_a2>(&e1,42,42.42f), 0);
		BOOST_TEST_EQ(count, 0);
		BOOST_TEST_EQ(synapse::emit<signal1_a2>(&e1,42,42.42f), 1);
		BOOST_TEST_EQ(count, 1);
		c1.reset();
		BOOST_TEST_EQ(connect_count, 0);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a2>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	typedef struct signal0_a3_(*signal0_a3)(int,float,char const *);
	typedef struct signal1_a3_(*signal1_a3)(int,float,std::string const &);
	typedef struct signal2_a3_(*signal2_a3)(int,float,std::string const &);

	void test_a3()
	{
		int connect_count=0;
		int count=0;
		my_emitter_type e1;
		shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a3> >(synapse::meta::emitter(),
			[&e1,&connect_count]( synapse::connection & c, unsigned flags )
			{
				if( flags&synapse::meta::connect_flags::connecting )
				{
					BOOST_TEST_EQ(&e1, c.emitter<my_emitter_type>().get());
					++connect_count;
				}
				else
					--connect_count;
			} ) );
		BOOST_TEST(mc_c1.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> c1=synapse::connect<signal1_a3>(&e1,
			[&count]( int a1, float a2, std::string const & a3 )
			{
				BOOST_TEST_EQ(a1, 42);
				BOOST_TEST_EQ(a2, 42.42f);
				BOOST_TEST_EQ(a3, "42");
				++count;
			} );
		BOOST_TEST_EQ(connect_count, 1);
		BOOST_TEST_EQ(synapse::emit<signal2_a3>(&e1,42,42.42f,"42"), 0);
		BOOST_TEST_EQ(count, 0);
		BOOST_TEST_EQ(synapse::emit<signal1_a3>(&e1,42,42.42f,"42"), 1);
		BOOST_TEST_EQ(count, 1);
		c1.reset();
		BOOST_TEST_EQ(connect_count, 0);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a3>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	typedef struct signal0_a4_(*signal0_a4)(int,float,char const *,short &);
	typedef struct signal1_a4_(*signal1_a4)(int,float,std::string const &,short &);
	typedef struct signal2_a4_(*signal2_a4)(int,float,std::string const &,short &);

	void test_a4()
	{
		int connect_count=0;
		int count=0;
		my_emitter_type e1;
		shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a4> >(synapse::meta::emitter(),
			[&e1,&connect_count]( synapse::connection & c, unsigned flags )
			{
				if( flags&synapse::meta::connect_flags::connecting )
				{
					BOOST_TEST_EQ(&e1, c.emitter<my_emitter_type>().get());
					++connect_count;
				}
				else
					--connect_count;
			} ) );
		BOOST_TEST(mc_c1.unique());
		BOOST_TEST_EQ(connect_count, 0);
		short a4=42;
		shared_ptr<synapse::connection> c1=synapse::connect<signal1_a4>(&e1,
			[&count,&a4]( int a1, float a2, std::string const & a3, short & a4_ )
			{
				BOOST_TEST_EQ(a1, 42);
				BOOST_TEST_EQ(a2, 42.42f);
				BOOST_TEST_EQ(a3, "42");
				BOOST_TEST_EQ(&a4, &a4_);
				++count;
			} );
		BOOST_TEST_EQ(connect_count, 1);
		BOOST_TEST_EQ(synapse::emit<signal2_a4>(&e1,42,42.42f,"42",synapse::ref(a4)), 0);
		BOOST_TEST_EQ(count, 0);
		BOOST_TEST_EQ(synapse::emit<signal1_a4>(&e1,42,42.42f,"42",synapse::ref(a4)), 1);
		BOOST_TEST_EQ(count, 1);
		c1.reset();
		BOOST_TEST_EQ(connect_count, 0);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a4>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
}

int main( int argc, char const * argv[] )
{
	test_a0();
	test_a1();
	test_a2();
	test_a3();
	test_a4();
	return boost::report_errors();
}
