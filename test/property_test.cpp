//Copyright (c) 2015-2017 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/properties.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace synapse=boost::synapse;
using synapse::shared_ptr;

typedef synapse::property<struct prop1_,std::string> prop1;

int
main( int argc, char const * argv[] )
    {
    int obj;
    BOOST_TEST(synapse::get<prop1>(&obj)==0);
    auto c=synapse::set<prop1>(&obj,"Hello World");
    BOOST_TEST(*synapse::get<prop1>(&obj)=="Hello World");
    c.reset();
    BOOST_TEST(synapse::get<prop1>(&obj)==0);
    return boost::report_errors();
    }
