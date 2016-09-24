//Copyright (c1) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/connect.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace synapse=boost::synapse;
using synapse::shared_ptr;
typedef struct test_signal1_(*test_signal1)();
typedef struct test_signal2_(*test_signal2)();
int count1;
int count2;
void
receiver1()
    {
    ++count1;
    }
void
receiver2()
    {
    ++count2;
    }
int
main( int argc, char const * argv[] )
    {
    int emitter;
    BOOST_TEST(synapse::emit<test_signal1>(&emitter)==0);
    BOOST_TEST(synapse::emit<test_signal2>(&emitter)==0);
    shared_ptr<synapse::connection const> c1=synapse::connect<test_signal1>(&emitter,&receiver1);
    shared_ptr<synapse::connection const> c2=synapse::connect<test_signal2>(&emitter,&receiver2);
    BOOST_TEST(synapse::emit<test_signal1>(&emitter)==1);
    BOOST_TEST(count1==1);
    BOOST_TEST(count2==0);
    BOOST_TEST(synapse::emit<test_signal2>(&emitter)==1);
    BOOST_TEST(count1==1);
    BOOST_TEST(count2==1);
    BOOST_TEST(synapse::emit<test_signal1>(&emitter)==1);
    BOOST_TEST(count1==2);
    BOOST_TEST(count2==1);
    BOOST_TEST(synapse::emit<test_signal2>(&emitter)==1);
    BOOST_TEST(count1==2);
    BOOST_TEST(count2==2);
    c1.reset();
    c2.reset();
    BOOST_TEST(synapse::emit<test_signal1>(&emitter)==0);
    BOOST_TEST(count1==2);
    BOOST_TEST(count2==2);
    BOOST_TEST(synapse::emit<test_signal2>(&emitter)==0);
    BOOST_TEST(count1==2);
    BOOST_TEST(count2==2);
    return boost::report_errors();
    }
