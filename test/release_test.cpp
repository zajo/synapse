//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/connect.hpp>
#include "boost/core/lightweight_test.hpp"

namespace synapse=boost::synapse;
using synapse::shared_ptr;
using synapse::weak_ptr;
using synapse::make_shared;

typedef struct my_signal_(*my_signal)();
struct my_emitter { };

int main( int argc, char const * argv[] )
{
	int meta_counter=0;
	shared_ptr<synapse::connection> mc=release(synapse::connect<synapse::meta::connected<my_signal> >(synapse::meta::emitter(),
		[&meta_counter]( synapse::connection & c, unsigned flags )
		{
			if( flags&synapse::meta::connect_flags::connecting )
				++meta_counter;
			else
				--meta_counter;
		}));

	shared_ptr<my_emitter> e1 = make_shared<my_emitter>();
	int emit_counter1=0;
	BOOST_TEST_EQ(meta_counter, 0);
	weak_ptr<synapse::pconnection> c1 = synapse::connect<my_signal>(e1,
		[&emit_counter1]()
	{
		++emit_counter1;
	});
	BOOST_TEST(!c1.expired());

	shared_ptr<my_emitter> e2 = make_shared<my_emitter>();
	int emit_counter2=0;
	BOOST_TEST_EQ(meta_counter, 1);
	weak_ptr<synapse::pconnection> c2 = synapse::connect<my_signal>(e2,
		[&emit_counter2]()
	{
		++emit_counter2;
	});
	BOOST_TEST(!c2.expired());

	BOOST_TEST_EQ(meta_counter, 2);
	BOOST_TEST_EQ(emit_counter1, 0);
	BOOST_TEST_EQ(emit_counter2, 0);
	synapse::emit<my_signal>(e1.get());
	BOOST_TEST_EQ(emit_counter1, 1);
	BOOST_TEST_EQ(emit_counter2, 0);
	synapse::emit<my_signal>(e2.get());
	BOOST_TEST_EQ(emit_counter1, 1);
	BOOST_TEST_EQ(emit_counter2, 1);

	e1.reset();
	e2.reset();
	BOOST_TEST_EQ(meta_counter, 2);
	{
		BOOST_TEST(!c1.expired());
		BOOST_TEST(!c2.expired());
		shared_ptr<synapse::connection> c=synapse::connect<my_signal>(&meta_counter,[ ]( ){ });
		BOOST_TEST(c1.expired());
		BOOST_TEST(c2.expired());
		BOOST_TEST_EQ(meta_counter, 1);
	}

	BOOST_TEST_EQ(meta_counter, 0);
	return boost::report_errors();
}
