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
	void noop()
	{
	}
}

int main( int argc, char const * argv[] )
{
	{
		my_emitter_type e;
		shared_ptr<synapse::connection> c=synapse::connect<my_signal>(&e,&noop);
		BOOST_TEST(!c->get_user_data<int>());
		c->set_user_data(42);
		BOOST_TEST_EQ(*c->get_user_data<int>(), 42);
	}
	{
		auto e = make_shared<my_emitter_type>();
		shared_ptr<synapse::pconnection> c=synapse::connect<my_signal>(e,&noop).lock();
		BOOST_TEST(!c->get_user_data<int>());
		c->set_user_data(42);
		BOOST_TEST_EQ(*c->get_user_data<int>(), 42);
	}
	return boost::report_errors();
}
