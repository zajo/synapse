//Copyright (c) 2015-2021 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/connect.hpp>
#include "boost/core/lightweight_test.hpp"

namespace synapse = boost::synapse;

typedef struct old_signal_(*old_signal)(int);

int main( int argc, char const * argv[] )
{
	int emitter;
	int count = 0;
	std::shared_ptr<synapse::connection> c = synapse::connect<old_signal>(&emitter,
		[&count]( int x )
		{
			BOOST_TEST_EQ(x, 42);
			++count;
		} );
	BOOST_TEST_EQ(synapse::emit<old_signal>(&emitter, 42), 1);
	BOOST_TEST_EQ(count, 1);
	return boost::report_errors();
}
