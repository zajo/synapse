//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/block.hpp>
#include <boost/synapse/blocker.hpp>
#include <boost/synapse/connect.hpp>
#include "boost/core/lightweight_test.hpp"

namespace synapse=boost::synapse;
using synapse::shared_ptr;

namespace
{
	template <class Signal>

	bool emitter_blocked( void const * e )
	{
		shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<Signal>(false);
		return tlsd && tlsd->emitter_blocked_(*tlsd,e);
	}
	typedef struct my_signal_(*my_signal)();
}

int main( int argc, char const * argv[] )
{
	int e1, e2;
	BOOST_TEST(!emitter_blocked<my_signal>(&e1));
	BOOST_TEST(!emitter_blocked<my_signal>(&e2));
	shared_ptr<synapse::blocker> b1=synapse::block<my_signal>(&e1);
	BOOST_TEST(emitter_blocked<my_signal>(&e1));
	BOOST_TEST(!emitter_blocked<my_signal>(&e2));
	shared_ptr<synapse::blocker> b2=synapse::block<my_signal>(&e2);
	BOOST_TEST(emitter_blocked<my_signal>(&e1));
	BOOST_TEST(emitter_blocked<my_signal>(&e2));
	int count=0;
	shared_ptr<synapse::connection> c=synapse::connect<my_signal>(&e1,[&count]() { ++count; } );
	BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 0);
	BOOST_TEST_EQ(count, 0);
	b1.reset();
	BOOST_TEST(!emitter_blocked<my_signal>(&e1));
	BOOST_TEST(emitter_blocked<my_signal>(&e2));
	BOOST_TEST_EQ(synapse::emit<my_signal>(&e1), 1);
	BOOST_TEST_EQ(count, 1);
	b2.reset();
	BOOST_TEST(!emitter_blocked<my_signal>(&e1));
	BOOST_TEST(!emitter_blocked<my_signal>(&e2));
	return boost::report_errors();
}
