//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/translate.hpp>
#include <boost/synapse/connection.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace synapse=boost::synapse;
using synapse::shared_ptr;
using synapse::weak_ptr;

namespace
{
    struct my_emitter_type { };

    void test_meta_callback_connect( int & count, my_emitter_type * e1, synapse::connection & c, unsigned flags )
    {
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        if( flags&synapse::meta::connect_flags::connecting )
        {
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(e1==c.emitter<my_emitter_type>().get());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            ++count;
        }
        else
            --count;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////

    typedef struct signal0_a0_(*signal0_a0)();
    typedef struct signal1_a0_(*signal1_a0)();
    typedef struct signal2_a0_(*signal2_a0)();

    void test_a0()
    {
        int connect_count=0;
        int count=0;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        my_emitter_type e1;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        my_emitter_type e2;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c0 = release( synapse::connect<synapse::meta::connected<signal0_a0> >(synapse::meta::emitter(),
            [&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c0.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a0> >(synapse::meta::emitter(),
            [&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c1.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c2 = release( synapse::connect<synapse::meta::connected<signal2_a0> >(synapse::meta::emitter(),
            [&connect_count,&e2]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e2,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c2.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c0=synapse::translate<signal0_a0,signal1_a0>(&e1,&e1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c1=synapse::translate<signal1_a0,signal2_a0>(&e1,&e2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c2=synapse::connect<signal2_a0>(&e2,
            [&count]()
            {
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
                ++count;
            } );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==3);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal0_a0>(&e1)==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a0>(&e1)==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal2_a0>(&e2)==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==3);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a0>(&e1)==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        c2.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal0_a0>(&e1)==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a0>(&e1)==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal2_a0>(&e2)==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal2_a0>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        c1.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a0>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        c0.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal0_a0>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////

    typedef struct signal0_a1_(*signal0_a1)(short);
    typedef struct signal1_a1_(*signal1_a1)(int);
    typedef struct signal2_a1_(*signal2_a1)(int);

    void test_a1()
    {
        int connect_count=0;
        int count=0;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        my_emitter_type e1;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        my_emitter_type e2;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c0 = release( synapse::connect<synapse::meta::connected<signal0_a1> >(synapse::meta::emitter(),
            [&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c0.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a1> >(synapse::meta::emitter(),
            [&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c1.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c2 = release( synapse::connect<synapse::meta::connected<signal2_a1> >(synapse::meta::emitter(),
            [&connect_count,&e2]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e2,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c2.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c0=synapse::translate<signal0_a1,signal1_a1>(&e1,&e1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c1=synapse::translate<signal1_a1,signal2_a1>(&e1,&e2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c2=synapse::connect<signal2_a1>(&e2,
            [&count]( int a1 )
            {
                BOOST_TEST(a1==42);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
                ++count;
            } );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==3);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal0_a1>(&e1,42)==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a1>(&e1,42)==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal2_a1>(&e2,42)==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==3);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a1>(&e1,42)==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        c2.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal0_a1>(&e1,42)==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a1>(&e1,42)==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal2_a1>(&e2,42)==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal2_a1>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
        c1.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a1>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
        c0.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal0_a1>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////

    typedef struct signal0_a2_(*signal0_a2)(int,double);
    typedef struct signal1_a2_(*signal1_a2)(int,float);
    typedef struct signal2_a2_(*signal2_a2)(int,float);
    void test_a2()
    {
        int connect_count=0;
        int count=0;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        my_emitter_type e1;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        my_emitter_type e2;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c0 = release( synapse::connect<synapse::meta::connected<signal0_a2> >(synapse::meta::emitter(),
            [&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c0.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a2> >(synapse::meta::emitter(),
            [&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c1.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c2 = release( synapse::connect<synapse::meta::connected<signal2_a2> >(synapse::meta::emitter(),
            [&connect_count,&e2]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e2,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c2.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c0=synapse::translate<signal0_a2,signal1_a2>(&e1,&e1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c1=synapse::translate<signal1_a2,signal2_a2>(&e1,&e2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c2=synapse::connect<signal2_a2>(&e2,
            [&count]( int a1, float a2 )
            {
                BOOST_TEST(a1==42);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
                BOOST_TEST(a2==42.42f);
                ++count;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            } );
        BOOST_TEST(connect_count==3);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal0_a2>(&e1,42,42.42)==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a2>(&e1,42,42.42f)==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal2_a2>(&e2,42,42.42f)==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==3);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a2>(&e1,42,42.42f)==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        c2.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal0_a2>(&e1,42,42.42)==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a2>(&e1,42,42.42f)==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal2_a2>(&e2,42,42.42f)==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal2_a2>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
        c1.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a2>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
        c0.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal0_a2>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////

    typedef struct signal0_a3_(*signal0_a3)(int,float,char const *);
    typedef struct signal1_a3_(*signal1_a3)(int,float,std::string const &);
    typedef struct signal2_a3_(*signal2_a3)(int,float,std::string const &);

    void test_a3()
    {
        int connect_count=0;
        int count=0;
        my_emitter_type e1;
        my_emitter_type e2;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c0 = release( synapse::connect<synapse::meta::connected<signal0_a3> >(synapse::meta::emitter(),
            [&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c0.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a3> >(synapse::meta::emitter(),
            [&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c1.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c2 = release( synapse::connect<synapse::meta::connected<signal2_a3> >(synapse::meta::emitter(),
            [&connect_count,&e2]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e2,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c2.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c0=synapse::translate<signal0_a3,signal1_a3>(&e1,&e1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c1=synapse::translate<signal1_a3,signal2_a3>(&e1,&e2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c2=synapse::connect<signal2_a3>(&e2,
            [&count]( int a1, float a2, std::string const & a3 )
            {
                BOOST_TEST(a1==42);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
                BOOST_TEST(a2==42.42f);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
                BOOST_TEST(a3=="42");
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
                ++count;
            } );
        BOOST_TEST(connect_count==3);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal0_a3>(&e1,42,42.42f,"42")==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a3>(&e1,42,42.42f,"42")==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal2_a3>(&e2,42,42.42f,"42")==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==3);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a3>(&e1,42,42.42f,"42")==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        c2.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal0_a3>(&e1,42,42.42f,"42")==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a3>(&e1,42,42.42f,"42")==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal2_a3>(&e2,42,42.42f,"42")==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal2_a3>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
        c1.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a3>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
        c0.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal0_a3>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////

    typedef struct signal0_a4_(*signal0_a4)(int,float,char const *,short &);
    typedef struct signal1_a4_(*signal1_a4)(int,float,std::string const &,short &);
    typedef struct signal2_a4_(*signal2_a4)(int,float,std::string const &,short &);

    void test_a4()
    {
        int connect_count=0;
        int count=0;
        my_emitter_type e1;
        my_emitter_type e2;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c0 = release( synapse::connect<synapse::meta::connected<signal0_a4> >(synapse::meta::emitter(),
            [&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c0.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c1 = release( synapse::connect<synapse::meta::connected<signal1_a4> >(synapse::meta::emitter(),
            [&connect_count,&e1]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e1,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c1.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> mc_c2 = release( synapse::connect<synapse::meta::connected<signal2_a4> >(synapse::meta::emitter(),
            [&connect_count,&e2]( synapse::connection & c, unsigned flags ) { test_meta_callback_connect(connect_count,&e2,c,flags); } ) );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(mc_c2.unique());
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        short a4=42;
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c0=synapse::translate<signal0_a4,signal1_a4>(&e1,&e1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c1=synapse::translate<signal1_a4,signal2_a4>(&e1,&e2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        shared_ptr<synapse::connection> c2=synapse::connect<signal2_a4>(&e2,
            [&count,&a4]( int a1, float a2, std::string const & a3, short & a4_ )
            {
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
                BOOST_TEST(a1==42);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
                BOOST_TEST(a2=42.42f);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
                BOOST_TEST(a3=="42");
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
                BOOST_TEST(&a4==&a4_);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
                ++count;
            } );
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==3);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal0_a4>(&e1,42,42.42f,"42",std::ref(a4))==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a4>(&e1,42,42.42f,"42",std::ref(a4))==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal2_a4>(&e2,42,42.42f,"42",std::ref(a4))==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==3);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a4>(&e1,42,42.42f,"42",std::ref(a4))==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        c2.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal0_a4>(&e1,42,42.42f,"42",std::ref(a4))==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal1_a4>(&e1,42,42.42f,"42",std::ref(a4))==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(synapse::emit<signal2_a4>(&e2,42,42.42f,"42",std::ref(a4))==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(count==4);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==2);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal2_a4>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
        c1.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==1);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal1_a4>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
        c0.reset();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        BOOST_TEST(connect_count==0);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        {
            shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<signal0_a4>(false);
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
            BOOST_TEST(!(weak_ptr<void>()<tlsd->cl_) && !(tlsd->cl_<weak_ptr<void>()));
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////
}

int main( int argc, char const * argv[] )
{
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
    test_a0();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
    test_a1();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
    test_a2();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
    test_a3();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
    test_a4();
std::cerr << __FILE__ << '(' << __LINE__ << ")\n";
    return boost::report_errors();
}
