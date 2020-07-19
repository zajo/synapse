//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/translate.hpp>
#include <boost/synapse/connection.hpp>
#include "boost/core/lightweight_test.hpp"
#include <string>

#if BOOST_SYNAPSE_MSVC
#	pragma warning( disable: 4244 ) // 'argument': conversion from '_Ty' to 'float', possible loss of data
#endif
namespace synapse=boost::synapse;
using synapse::shared_ptr;
using synapse::weak_ptr;

namespace
{
	struct my_emitter_type { };

	void test_meta_callback_connect( int & count, my_emitter_type * e1, synapse::connection & c, unsigned flags )
	{
		if( flags&synapse::meta::connect_flags::connecting )
		{
			BOOST_TEST_EQ(e1, c.emitter<my_emitter_type>().get());
			++count;
		}
		else
			--count;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////

	typedef struct signal0_a0_(*signal0_a0)();
	typedef struct signal1_a0_(*signal1_a0)();
	typedef struct signal2_a0_(*signal2_a0)();

	void test_a0()
	{
		int connect_count=0;
		int count=0;
		my_emitter_type e1;
		my_emitter_type e2;
		shared_ptr<synapse::connection> mc_c0 = release( synapse::connect<synapse::meta::connected<signal0_a0> >(synapse::meta::emitter(),
			[&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
		BOOST_TEST(mc_c0.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a0> >(synapse::meta::emitter(),
			[&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
		BOOST_TEST(mc_c1.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> mc_c2 = release( synapse::connect<synapse::meta::connected<signal2_a0> >(synapse::meta::emitter(),
			[&connect_count,&e2]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e2,c,flags); } ) );
		BOOST_TEST(mc_c2.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> c0=synapse::translate<signal0_a0,signal1_a0>(&e1,&e1);
		BOOST_TEST_EQ(connect_count, 1);
		shared_ptr<synapse::connection> c1=synapse::translate<signal1_a0,signal2_a0>(&e1,&e2);
		BOOST_TEST_EQ(connect_count, 2);
		shared_ptr<synapse::connection> c2=synapse::connect<signal2_a0>(&e2,
			[&count]()
			{
				++count;
			} );
		BOOST_TEST_EQ(connect_count, 3);
		BOOST_TEST_EQ(synapse::emit<signal0_a0>(&e1), 1);
		BOOST_TEST_EQ(count, 1);
		BOOST_TEST_EQ(synapse::emit<signal1_a0>(&e1), 1);
		BOOST_TEST_EQ(count, 2);
		BOOST_TEST_EQ(synapse::emit<signal2_a0>(&e2), 1);
		BOOST_TEST_EQ(count, 3);
		BOOST_TEST_EQ(synapse::emit<signal1_a0>(&e1), 1);
		BOOST_TEST_EQ(count, 4);
		c2.reset();
		BOOST_TEST_EQ(synapse::emit<signal0_a0>(&e1), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(synapse::emit<signal1_a0>(&e1), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(synapse::emit<signal2_a0>(&e2), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(connect_count, 2);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal2_a0>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
		c1.reset();
		BOOST_TEST_EQ(connect_count, 1);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a0>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
		c0.reset();
		BOOST_TEST_EQ(connect_count, 0);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal0_a0>(false);
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
		my_emitter_type e2;
		shared_ptr<synapse::connection> mc_c0 = release( synapse::connect<synapse::meta::connected<signal0_a1> >(synapse::meta::emitter(),
			[&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
		BOOST_TEST(mc_c0.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a1> >(synapse::meta::emitter(),
			[&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
		BOOST_TEST(mc_c1.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> mc_c2 = release( synapse::connect<synapse::meta::connected<signal2_a1> >(synapse::meta::emitter(),
			[&connect_count,&e2]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e2,c,flags); } ) );
		BOOST_TEST(mc_c2.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> c0=synapse::translate<signal0_a1,signal1_a1>(&e1,&e1);
		BOOST_TEST_EQ(connect_count, 1);
		shared_ptr<synapse::connection> c1=synapse::translate<signal1_a1,signal2_a1>(&e1,&e2);
		BOOST_TEST_EQ(connect_count, 2);
		shared_ptr<synapse::connection> c2=synapse::connect<signal2_a1>(&e2,
			[&count]( int a1 )
			{
				BOOST_TEST_EQ(a1, 42);
				++count;
			} );
		BOOST_TEST_EQ(connect_count, 3);
		BOOST_TEST_EQ(synapse::emit<signal0_a1>(&e1,42), 1);
		BOOST_TEST_EQ(count, 1);
		BOOST_TEST_EQ(synapse::emit<signal1_a1>(&e1,42), 1);
		BOOST_TEST_EQ(count, 2);
		BOOST_TEST_EQ(synapse::emit<signal2_a1>(&e2,42), 1);
		BOOST_TEST_EQ(count, 3);
		BOOST_TEST_EQ(synapse::emit<signal1_a1>(&e1,42), 1);
		BOOST_TEST_EQ(count, 4);
		c2.reset();
		BOOST_TEST_EQ(synapse::emit<signal0_a1>(&e1,42), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(synapse::emit<signal1_a1>(&e1,42), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(synapse::emit<signal2_a1>(&e2,42), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(connect_count, 2);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal2_a1>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
		c1.reset();
		BOOST_TEST_EQ(connect_count, 1);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a1>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
		c0.reset();
		BOOST_TEST_EQ(connect_count, 0);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal0_a1>(false);
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
		my_emitter_type e2;
		shared_ptr<synapse::connection> mc_c0 = release( synapse::connect<synapse::meta::connected<signal0_a2> >(synapse::meta::emitter(),
			[&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
		BOOST_TEST(mc_c0.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a2> >(synapse::meta::emitter(),
			[&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
		BOOST_TEST(mc_c1.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> mc_c2 = release( synapse::connect<synapse::meta::connected<signal2_a2> >(synapse::meta::emitter(),
			[&connect_count,&e2]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e2,c,flags); } ) );
		BOOST_TEST(mc_c2.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> c0=synapse::translate<signal0_a2,signal1_a2>(&e1,&e1);
		BOOST_TEST_EQ(connect_count, 1);
		shared_ptr<synapse::connection> c1=synapse::translate<signal1_a2,signal2_a2>(&e1,&e2);
		BOOST_TEST_EQ(connect_count, 2);
		shared_ptr<synapse::connection> c2=synapse::connect<signal2_a2>(&e2,
			[&count]( int a1, float a2 )
			{
				BOOST_TEST_EQ(a1, 42);
				BOOST_TEST_EQ(a2, 42.42f);
				++count;
			} );
		BOOST_TEST_EQ(connect_count, 3);
		BOOST_TEST_EQ(synapse::emit<signal0_a2>(&e1,42,42.42), 1);
		BOOST_TEST_EQ(count, 1);
		BOOST_TEST_EQ(synapse::emit<signal1_a2>(&e1,42,42.42f), 1);
		BOOST_TEST_EQ(count, 2);
		BOOST_TEST_EQ(synapse::emit<signal2_a2>(&e2,42,42.42f), 1);
		BOOST_TEST_EQ(count, 3);
		BOOST_TEST_EQ(synapse::emit<signal1_a2>(&e1,42,42.42f), 1);
		BOOST_TEST_EQ(count, 4);
		c2.reset();
		BOOST_TEST_EQ(synapse::emit<signal0_a2>(&e1,42,42.42), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(synapse::emit<signal1_a2>(&e1,42,42.42f), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(synapse::emit<signal2_a2>(&e2,42,42.42f), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(connect_count, 2);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal2_a2>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
		c1.reset();
		BOOST_TEST_EQ(connect_count, 1);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a2>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
		c0.reset();
		BOOST_TEST_EQ(connect_count, 0);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal0_a2>(false);
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
		my_emitter_type e2;
		shared_ptr<synapse::connection> mc_c0 = release( synapse::connect<synapse::meta::connected<signal0_a3> >(synapse::meta::emitter(),
			[&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
		BOOST_TEST(mc_c0.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a3> >(synapse::meta::emitter(),
			[&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
		BOOST_TEST(mc_c1.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> mc_c2 = release( synapse::connect<synapse::meta::connected<signal2_a3> >(synapse::meta::emitter(),
			[&connect_count,&e2]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e2,c,flags); } ) );
		BOOST_TEST(mc_c2.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> c0=synapse::translate<signal0_a3,signal1_a3>(&e1,&e1);
		BOOST_TEST_EQ(connect_count, 1);
		shared_ptr<synapse::connection> c1=synapse::translate<signal1_a3,signal2_a3>(&e1,&e2);
		BOOST_TEST_EQ(connect_count, 2);
		shared_ptr<synapse::connection> c2=synapse::connect<signal2_a3>(&e2,
			[&count]( int a1, float a2, std::string const & a3 )
			{
				BOOST_TEST_EQ(a1, 42);
				BOOST_TEST_EQ(a2, 42.42f);
				BOOST_TEST_EQ(a3, "42");
				++count;
			} );
		BOOST_TEST_EQ(connect_count, 3);
		BOOST_TEST_EQ(synapse::emit<signal0_a3>(&e1,42,42.42f,"42"), 1);
		BOOST_TEST_EQ(count, 1);
		BOOST_TEST_EQ(synapse::emit<signal1_a3>(&e1,42,42.42f,"42"), 1);
		BOOST_TEST_EQ(count, 2);
		BOOST_TEST_EQ(synapse::emit<signal2_a3>(&e2,42,42.42f,"42"), 1);
		BOOST_TEST_EQ(count, 3);
		BOOST_TEST_EQ(synapse::emit<signal1_a3>(&e1,42,42.42f,"42"), 1);
		BOOST_TEST_EQ(count, 4);
		c2.reset();
		BOOST_TEST_EQ(synapse::emit<signal0_a3>(&e1,42,42.42f,"42"), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(synapse::emit<signal1_a3>(&e1,42,42.42f,"42"), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(synapse::emit<signal2_a3>(&e2,42,42.42f,"42"), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(connect_count, 2);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal2_a3>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
		c1.reset();
		BOOST_TEST_EQ(connect_count, 1);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a3>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
		c0.reset();
		BOOST_TEST_EQ(connect_count, 0);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal0_a3>(false);
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
		my_emitter_type e2;
		shared_ptr<synapse::connection> mc_c0 = release( synapse::connect<synapse::meta::connected<signal0_a4> >(synapse::meta::emitter(),
			[&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
		BOOST_TEST(mc_c0.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a4> >(synapse::meta::emitter(),
			[&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
		BOOST_TEST(mc_c1.unique());
		BOOST_TEST_EQ(connect_count, 0);
		shared_ptr<synapse::connection> mc_c2 = release( synapse::connect<synapse::meta::connected<signal2_a4> >(synapse::meta::emitter(),
			[&connect_count,&e2]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e2,c,flags); } ) );
		BOOST_TEST(mc_c2.unique());
		BOOST_TEST_EQ(connect_count, 0);
		short a4=42;
		shared_ptr<synapse::connection> c0=synapse::translate<signal0_a4,signal1_a4>(&e1,&e1);
		BOOST_TEST_EQ(connect_count, 1);
		shared_ptr<synapse::connection> c1=synapse::translate<signal1_a4,signal2_a4>(&e1,&e2);
		BOOST_TEST_EQ(connect_count, 2);
		shared_ptr<synapse::connection> c2=synapse::connect<signal2_a4>(&e2,
			[&count,&a4]( int a1, float a2, std::string const & a3, short & a4_ )
			{
				BOOST_TEST_EQ(a1, 42);
				BOOST_TEST(a2=42.42f);
				BOOST_TEST_EQ(a3, "42");
				BOOST_TEST_EQ(&a4, &a4_);
				++count;
			} );
		BOOST_TEST_EQ(connect_count, 3);
		BOOST_TEST_EQ(synapse::emit<signal0_a4>(&e1,42,42.42f,"42",std::ref(a4)), 1);
		BOOST_TEST_EQ(count, 1);
		BOOST_TEST_EQ(synapse::emit<signal1_a4>(&e1,42,42.42f,"42",std::ref(a4)), 1);
		BOOST_TEST_EQ(count, 2);
		BOOST_TEST_EQ(synapse::emit<signal2_a4>(&e2,42,42.42f,"42",std::ref(a4)), 1);
		BOOST_TEST_EQ(count, 3);
		BOOST_TEST_EQ(synapse::emit<signal1_a4>(&e1,42,42.42f,"42",std::ref(a4)), 1);
		BOOST_TEST_EQ(count, 4);
		c2.reset();
		BOOST_TEST_EQ(synapse::emit<signal0_a4>(&e1,42,42.42f,"42",std::ref(a4)), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(synapse::emit<signal1_a4>(&e1,42,42.42f,"42",std::ref(a4)), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(synapse::emit<signal2_a4>(&e2,42,42.42f,"42",std::ref(a4)), 0);
		BOOST_TEST_EQ(count, 4);
		BOOST_TEST_EQ(connect_count, 2);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal2_a4>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
		c1.reset();
		BOOST_TEST_EQ(connect_count, 1);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a4>(false);
			BOOST_TEST(!weak_ptr<void>().owner_before(tlsd->cl_) && !tlsd->cl_.owner_before(weak_ptr<void>()));
		}
		c0.reset();
		BOOST_TEST_EQ(connect_count, 0);
		{
			shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal0_a4>(false);
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
