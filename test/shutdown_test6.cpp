//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/connect.hpp>

namespace synapse=boost::synapse;
using synapse::shared_ptr;
using synapse::make_shared;

struct my_emitter_type { };
typedef struct my_signal_(*my_signal)();

int main( int argc, char const * argv[] )
{
	synapse::connect<synapse::meta::connected<my_signal> >( synapse::meta::emitter(), [ ]( synapse::connection &, unsigned ) { } );
	shared_ptr<my_emitter_type> e=make_shared<my_emitter_type>();
	auto connected = synapse::connect<my_signal>( e, [ ] ( ) { } );
	return 0;
}
