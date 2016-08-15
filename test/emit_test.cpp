//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/connect.hpp>
#include <boost/synapse/dep/bind.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace synapse=boost::synapse;

namespace
    {
    struct my_emitter_type { };
    typedef struct my_signal_(*my_signal)();
    void
    inc( int & count )
        {
        ++count;
        }
    template <class T>
    void
    disconnect( boost::shared_ptr<T> & c, int & count )
        {
        if( ++count==3 )
            c.reset();
        }              
    void
    test_connection_expiration_during_emit1()
        {
        my_emitter_type e;
        int count0=0, count1=0, count2=0;
        boost::shared_ptr<synapse::connection> c1;
        boost::shared_ptr<synapse::connection> c0=synapse::connect<my_signal>(&e,synapse::bind(&disconnect<synapse::connection>,synapse::ref(c1),synapse::ref(count0)));
        c1=synapse::connect<my_signal>(&e,synapse::bind(&inc,synapse::ref(count1)));
        boost::shared_ptr<synapse::connection> c2=synapse::connect<my_signal>(&e,synapse::bind(&inc,synapse::ref(count2)));
        BOOST_TEST(synapse::emit<my_signal>(&e)==3);
        BOOST_TEST(c1);
        BOOST_TEST(count0==1);
        BOOST_TEST(count1==1);
        BOOST_TEST(count2==1);
        BOOST_TEST(synapse::emit<my_signal>(&e)==3);
        BOOST_TEST(c1);
        BOOST_TEST(count0==2);
        BOOST_TEST(count1==2);
        BOOST_TEST(count2==2);
        BOOST_TEST(synapse::emit<my_signal>(&e)==2);
        BOOST_TEST(!c1);
        BOOST_TEST(count0==3);
        BOOST_TEST(count1==2);
        BOOST_TEST(count2==3);
        }
    void
    test_connection_expiration_during_emit2()
        {
        my_emitter_type e;
        int count0=0, count1=0, count2=0;
        boost::shared_ptr<void> lifetime(new int);
        boost::shared_ptr<synapse::connection> c0=synapse::connect<my_signal>(&e,synapse::bind(&disconnect<void>,synapse::ref(lifetime),synapse::ref(count0)));
        boost::shared_ptr<synapse::connection>c1=synapse::connect<my_signal>(&e,synapse::bind(&inc,synapse::ref(count1)),lifetime);
        boost::shared_ptr<synapse::connection> c2=synapse::connect<my_signal>(&e,synapse::bind(&inc,synapse::ref(count2)));
        BOOST_TEST(synapse::emit<my_signal>(&e)==3);
        BOOST_TEST(lifetime);
        BOOST_TEST(count0==1);
        BOOST_TEST(count1==1);
        BOOST_TEST(count2==1);
        BOOST_TEST(synapse::emit<my_signal>(&e)==3);
        BOOST_TEST(lifetime);
        BOOST_TEST(count0==2);
        BOOST_TEST(count1==2);
        BOOST_TEST(count2==2);
        BOOST_TEST(synapse::emit<my_signal>(&e)==2);
        BOOST_TEST(!lifetime);
        BOOST_TEST(count0==3);
        BOOST_TEST(count1==2);
        BOOST_TEST(count2==3);
        }
    struct null_deleter { template <class T> void operator()( T * ) { } };
    void
    emitter_address_reuse_test()
        {
        my_emitter_type e2;
        int n1=0;
        boost::shared_ptr<my_emitter_type> e1(&e2,null_deleter());
        boost::shared_ptr<synapse::connection> c1=synapse::connect<my_signal>(e1,synapse::bind(&inc,synapse::ref(n1)));
        BOOST_TEST(synapse::emit<my_signal>(&e2)==1);
        BOOST_TEST(n1==1);
        e1.reset();
        int n2=0;
        boost::shared_ptr<synapse::connection> c2=synapse::connect<my_signal>(&e2,synapse::bind(&inc,synapse::ref(n2)));
        BOOST_TEST(synapse::emit<my_signal>(&e2)==1);
        BOOST_TEST(n1==1);
        BOOST_TEST(n2==1);
        c2.reset();
        BOOST_TEST(synapse::emit<my_signal>(&e2)==0);
        BOOST_TEST(n1==1);
        BOOST_TEST(n2==1);
        c1.reset();
        BOOST_TEST(synapse::emit<my_signal>(&e2)==0);
        BOOST_TEST(n1==1);
        BOOST_TEST(n2==1);
        }
    }

int
main( int argc, char const * argv[] )
    {
    test_connection_expiration_during_emit1();
    test_connection_expiration_during_emit2();
    emitter_address_reuse_test();
    return boost::report_errors();
    }
