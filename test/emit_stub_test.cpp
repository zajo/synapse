//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/emit.hpp>
#include "boost/core/lightweight_test.hpp"

namespace synapse=boost::synapse;

typedef struct test_signal_(*test_signal)(int);

int main( int argc, char const * argv[] )
{
	int e;
	BOOST_TEST_EQ(synapse::emit<test_signal>(&e, 42), 0);
	return boost::report_errors();
}
