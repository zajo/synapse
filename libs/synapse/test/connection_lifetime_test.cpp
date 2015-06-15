//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/connect.hpp>
#include <boost/synapse/connection.hpp>
#include <boost/bind.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace synapse=boost::synapse;

namespace
    {
    struct my_emitter_type { };
    void
    inc( int & count )
        {
        ++count;
        }
    typedef struct my_signal_(*my_signal)();
    void
    test_reset_connection()
        {
        int count1=0, count2=0;
        my_emitter_type e1;
        my_emitter_type e2;
        boost::shared_ptr<synapse::connection> c1=synapse::connect<my_signal>(&e1,boost::bind(&inc,boost::ref(count1)));
        boost::shared_ptr<synapse::connection> c2=synapse::connect<my_signal>(&e2,boost::bind(&inc,boost::ref(count2)));
        boost::shared_ptr<synapse::connection> c3=synapse::connect<my_signal>(&e1,boost::bind(&inc,boost::ref(count1)));
        boost::shared_ptr<synapse::connection> c4=synapse::connect<my_signal>(&e2,boost::bind(&inc,boost::ref(count2)));
        boost::shared_ptr<synapse::connection> c5=synapse::connect<my_signal>(&e1,boost::bind(&inc,boost::ref(count1)));
        boost::shared_ptr<synapse::connection> c6=synapse::connect<my_signal>(&e2,boost::bind(&inc,boost::ref(count2)));
        BOOST_TEST(synapse::emit<my_signal>(&e1)==3);
        BOOST_TEST(count1==3);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==3);
        BOOST_TEST(count1==3);
        BOOST_TEST(count2==3);
        c1.reset();
        count1=count2=0;
        BOOST_TEST(synapse::emit<my_signal>(&e1)==2);
        BOOST_TEST(count1==2);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==3);
        BOOST_TEST(count1==2);
        BOOST_TEST(count2==3);
        c2.reset();
        count1=count2=0;
        BOOST_TEST(synapse::emit<my_signal>(&e1)==2);
        BOOST_TEST(count1==2);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==2);
        BOOST_TEST(count1==2);
        BOOST_TEST(count2==2);
        c3.reset();
        count1=count2=0;
        BOOST_TEST(synapse::emit<my_signal>(&e1)==1);
        BOOST_TEST(count1==1);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==2);
        BOOST_TEST(count1==1);
        BOOST_TEST(count2==2);
        c4.reset();
        count1=count2=0;
        BOOST_TEST(synapse::emit<my_signal>(&e1)==1);
        BOOST_TEST(count1==1);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==1);
        BOOST_TEST(count1==1);
        BOOST_TEST(count2==1);
        c5.reset();
        count1=count2=0;
        BOOST_TEST(synapse::emit<my_signal>(&e1)==0);
        BOOST_TEST(count1==0);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==1);
        BOOST_TEST(count1==0);
        BOOST_TEST(count2==1);
        c6.reset();
        count1=count2=0;
        BOOST_TEST(synapse::emit<my_signal>(&e1)==0);
        BOOST_TEST(count1==0);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==0);
        BOOST_TEST(count1==0);
        BOOST_TEST(count2==0);
        }
    typedef struct my_signal_(*my_signal)();
    void
    test_reset_lifetime()
        {
        int count1=0, count2=0;
        my_emitter_type e1;
        my_emitter_type e2;
        boost::shared_ptr<void> lt1(new int);
        boost::shared_ptr<void> lt2(new int);
        boost::shared_ptr<void> lt3(new int);
        boost::shared_ptr<void> lt4(new int);
        boost::shared_ptr<void> lt5(new int);
        boost::shared_ptr<void> lt6(new int);
        boost::shared_ptr<synapse::connection> const c1=synapse::connect<my_signal>(&e1,boost::bind(&inc,boost::ref(count1)),lt1);
        boost::shared_ptr<synapse::connection> const c2=synapse::connect<my_signal>(&e2,boost::bind(&inc,boost::ref(count2)),lt2);
        boost::shared_ptr<synapse::connection> const c3=synapse::connect<my_signal>(&e1,boost::bind(&inc,boost::ref(count1)),lt3);
        boost::shared_ptr<synapse::connection> const c4=synapse::connect<my_signal>(&e2,boost::bind(&inc,boost::ref(count2)),lt4);
        boost::shared_ptr<synapse::connection> const c5=synapse::connect<my_signal>(&e1,boost::bind(&inc,boost::ref(count1)),lt5);
        boost::shared_ptr<synapse::connection> const c6=synapse::connect<my_signal>(&e2,boost::bind(&inc,boost::ref(count2)),lt6);
        BOOST_TEST(synapse::emit<my_signal>(&e1)==3);
        BOOST_TEST(count1==3);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==3);
        BOOST_TEST(count1==3);
        BOOST_TEST(count2==3);
        lt1.reset();
        count1=count2=0;
        BOOST_TEST(synapse::emit<my_signal>(&e1)==2);
        BOOST_TEST(count1==2);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==3);
        BOOST_TEST(count1==2);
        BOOST_TEST(count2==3);
        lt2.reset();
        count1=count2=0;
        BOOST_TEST(synapse::emit<my_signal>(&e1)==2);
        BOOST_TEST(count1==2);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==2);
        BOOST_TEST(count1==2);
        BOOST_TEST(count2==2);
        lt3.reset();
        count1=count2=0;
        BOOST_TEST(synapse::emit<my_signal>(&e1)==1);
        BOOST_TEST(count1==1);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==2);
        BOOST_TEST(count1==1);
        BOOST_TEST(count2==2);
        lt4.reset();
        count1=count2=0;
        BOOST_TEST(synapse::emit<my_signal>(&e1)==1);
        BOOST_TEST(count1==1);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==1);
        BOOST_TEST(count1==1);
        BOOST_TEST(count2==1);
        lt5.reset();
        count1=count2=0;
        BOOST_TEST(synapse::emit<my_signal>(&e1)==0);
        BOOST_TEST(count1==0);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==1);
        BOOST_TEST(count1==0);
        BOOST_TEST(count2==1);
        lt6.reset();
        count1=count2=0;
        BOOST_TEST(synapse::emit<my_signal>(&e1)==0);
        BOOST_TEST(count1==0);
        BOOST_TEST(count2==0);
        BOOST_TEST(synapse::emit<my_signal>(&e2)==0);
        BOOST_TEST(count1==0);
        BOOST_TEST(count2==0);
        }
    struct null_deleter { void  operator()(void const *) { } };
    void
    test_emitter_expiring_before_connection()
        {
        int count=0;
        my_emitter_type e;
        boost::shared_ptr<my_emitter_type> e1(&e,null_deleter());
        boost::shared_ptr<synapse::connection> c1=synapse::connect<my_signal>(e1,boost::bind(&inc,boost::ref(count)));
        e1.reset();
        BOOST_TEST(count==0);
        BOOST_TEST(synapse::emit<my_signal>(&e)==0);
        c1.reset();
        BOOST_TEST(count==0);
        BOOST_TEST(synapse::emit<my_signal>(&e)==0);
        }
    }

int
main( int argc, char const * argv[] )
    {
    test_reset_connection();
    test_reset_lifetime();
    test_emitter_expiring_before_connection();
    return boost::report_errors();
    }
