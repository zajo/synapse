//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/block.hpp>
#include <boost/synapse/blocker.hpp>
#include <boost/synapse/connect.hpp>
#include <boost/synapse/dep/bind.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace synapse=boost::synapse;

namespace
    {
    template <class Signal>
    bool
    emitter_blocked( void const * e )
        {
        return synapse::synapse_detail::emitter_blocked_<Signal>()(synapse::synapse_detail::get_blocked_list<Signal>(),e);
        }
    void
    inc( int & c )
        {
        ++c;
        }
    typedef struct my_signal_(*my_signal)();
    }

int
main( int argc, char const * argv[] )
    {
    int e1, e2;
    BOOST_TEST(!emitter_blocked<my_signal>(&e1));
    BOOST_TEST(!emitter_blocked<my_signal>(&e2));
    boost::shared_ptr<synapse::blocker> b1=synapse::block<my_signal>(&e1);
    BOOST_TEST(emitter_blocked<my_signal>(&e1));
    BOOST_TEST(!emitter_blocked<my_signal>(&e2));
    boost::shared_ptr<synapse::blocker> b2=synapse::block<my_signal>(&e2);
    BOOST_TEST(emitter_blocked<my_signal>(&e1));
    BOOST_TEST(emitter_blocked<my_signal>(&e2));
    int count=0;
    boost::shared_ptr<synapse::connection> c=synapse::connect<my_signal>(&e1,synapse::bind(&inc,synapse::ref(count)));
    BOOST_TEST(synapse::emit<my_signal>(&e1)==0);
    BOOST_TEST(count==0);
    b1.reset();
    BOOST_TEST(!emitter_blocked<my_signal>(&e1));
    BOOST_TEST(emitter_blocked<my_signal>(&e2));
    BOOST_TEST(synapse::emit<my_signal>(&e1)==1);
    BOOST_TEST(count==1);
    b2.reset();
    BOOST_TEST(!emitter_blocked<my_signal>(&e1));
    BOOST_TEST(!emitter_blocked<my_signal>(&e2));
    return boost::report_errors();
    }
