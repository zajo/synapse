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
    //////////////////////////////////////////////////////////////////////////////////////////////////
    struct my_emitter_type { };
    void
    test_callback_a0( int & count )
        {
        ++count;
        }
    void
    test_meta_callback_connect_a0( int & count, my_emitter_type * e1, synapse::connection & c, unsigned flags )
        {
        BOOST_TEST(e1==c.emitter<my_emitter_type>().get());
        count += (flags&synapse::meta::connect_flags::connecting)?1:-1;
        }
    typedef struct signal1_a0_(*signal1_a0)();
    typedef struct signal2_a0_(*signal2_a0)();
    void
    test_a0()
        {
        int connect_count=0;
        int count=0;
        my_emitter_type e1;
        boost::shared_ptr<synapse::connection> mc_c1=synapse::connect<synapse::meta::connected<signal1_a0> >(synapse::meta::emitter(),boost::bind(&test_meta_callback_connect_a0,boost::ref(connect_count),&e1,_1,_2));
        BOOST_TEST(connect_count==0);
        boost::shared_ptr<synapse::connection> c1=synapse::connect<signal1_a0>(&e1,boost::bind(&test_callback_a0,boost::ref(count)));
        BOOST_TEST(connect_count==1);
        BOOST_TEST(synapse::emit<signal2_a0>(&e1)==0);
        BOOST_TEST(count==0);
        BOOST_TEST(synapse::emit<signal1_a0>(&e1)==1);
        BOOST_TEST(count==1);
        c1.reset();
        BOOST_TEST(connect_count==0);
            {
            boost::weak_ptr<synapse::synapse_detail::connection_list> & cl=synapse::synapse_detail::get_connection_list<signal1_a0>();
            BOOST_TEST(!(boost::weak_ptr<void>()<cl) && !(cl<boost::weak_ptr<void>()));
            }
        }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    void
    test_callback_a1( int & count, int a1, int a1_cmp )
        {
        BOOST_TEST(a1==a1_cmp);
        ++count;
        }
    void
    test_meta_callback_connect_a1( int & count, my_emitter_type * e1, synapse::connection & c, unsigned flags )
        {
        BOOST_TEST(e1==c.emitter<my_emitter_type>().get());
        count += (flags&synapse::meta::connect_flags::connecting)?1:-1;
        }
    typedef struct signal0_a1_(*signal0_a1)(short);
    typedef struct signal1_a1_(*signal1_a1)(int);
    typedef struct signal2_a1_(*signal2_a1)(int);
    void
    test_a1()
        {
        int connect_count=0;
        int count=0;
        my_emitter_type e1;
        boost::shared_ptr<synapse::connection> mc_c1=synapse::connect<synapse::meta::connected<signal1_a1> >(synapse::meta::emitter(),boost::bind(&test_meta_callback_connect_a1,boost::ref(connect_count),&e1,_1,_2));
        BOOST_TEST(connect_count==0);
        boost::shared_ptr<synapse::connection> c1=synapse::connect<signal1_a1>(&e1,boost::bind(&test_callback_a1,boost::ref(count),_1,42));
        BOOST_TEST(connect_count==1);
        BOOST_TEST(synapse::emit<signal2_a1>(&e1,42)==0);
        BOOST_TEST(count==0);
        BOOST_TEST(synapse::emit<signal1_a1>(&e1,42)==1);
        BOOST_TEST(count==1);
        c1.reset();
        BOOST_TEST(connect_count==0);
            {
            boost::weak_ptr<synapse::synapse_detail::connection_list> & cl=synapse::synapse_detail::get_connection_list<signal1_a1>();
            BOOST_TEST(!(boost::weak_ptr<void>()<cl) && !(cl<boost::weak_ptr<void>()));
            }
        }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    void
    test_callback_a2( int & count, int a1, int a1_cmp, float a2, float a2_cmp )
        {
        BOOST_TEST(a1==a1_cmp);
        BOOST_TEST(a2==a2_cmp);
        ++count;
        }
    void
    test_meta_callback_connect_a2( int & count, my_emitter_type * e1, synapse::connection & c, unsigned flags )
        {
        BOOST_TEST(e1==c.emitter<my_emitter_type>().get());
        count += (flags&synapse::meta::connect_flags::connecting)?1:-1;
        }
    typedef struct signal0_a2_(*signal0_a2)(int,double);
    typedef struct signal1_a2_(*signal1_a2)(int,float);
    typedef struct signal2_a2_(*signal2_a2)(int,float);
    void
    test_a2()
        {
        int connect_count=0;
        int count=0;
        my_emitter_type e1;
        boost::shared_ptr<synapse::connection> mc_c1=synapse::connect<synapse::meta::connected<signal1_a2> >(synapse::meta::emitter(),boost::bind(&test_meta_callback_connect_a2,boost::ref(connect_count),&e1,_1,_2));
        BOOST_TEST(connect_count==0);
        boost::shared_ptr<synapse::connection> c1=synapse::connect<signal1_a2>(&e1,boost::bind(&test_callback_a2,boost::ref(count),_1,42,_2,42.42f));
        BOOST_TEST(connect_count==1);
        BOOST_TEST(synapse::emit<signal2_a2>(&e1,42,42.42f)==0);
        BOOST_TEST(count==0);
        BOOST_TEST(synapse::emit<signal1_a2>(&e1,42,42.42f)==1);
        BOOST_TEST(count==1);
        c1.reset();
        BOOST_TEST(connect_count==0);
            {
            boost::weak_ptr<synapse::synapse_detail::connection_list> & cl=synapse::synapse_detail::get_connection_list<signal1_a2>();
            BOOST_TEST(!(boost::weak_ptr<void>()<cl) && !(cl<boost::weak_ptr<void>()));
            }
        }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    void
    test_callback_a3( int & count, int a1, int a1_cmp, float a2, float a2_cmp, std::string const & a3, std::string const & a3_cmp )
        {
        BOOST_TEST(a1==a1_cmp);
        BOOST_TEST(a2==a2_cmp);
        BOOST_TEST(a3==a3_cmp);
        ++count;
        }
    void
    test_meta_callback_connect_a3( int & count, my_emitter_type * e1, synapse::connection & c, unsigned flags )
        {
        BOOST_TEST(e1==c.emitter<my_emitter_type>().get());
        count += (flags&synapse::meta::connect_flags::connecting)?1:-1;
        }
    typedef struct signal0_a3_(*signal0_a3)(int,float,char const *);
    typedef struct signal1_a3_(*signal1_a3)(int,float,std::string const &);
    typedef struct signal2_a3_(*signal2_a3)(int,float,std::string const &);
    void
    test_a3()
        {
        int connect_count=0;
        int count=0;
        my_emitter_type e1;
        boost::shared_ptr<synapse::connection> mc_c1=synapse::connect<synapse::meta::connected<signal1_a3> >(synapse::meta::emitter(),boost::bind(&test_meta_callback_connect_a3,boost::ref(connect_count),&e1,_1,_2));
        BOOST_TEST(connect_count==0);
        boost::shared_ptr<synapse::connection> c1=synapse::connect<signal1_a3>(&e1,boost::bind(&test_callback_a3,boost::ref(count),_1,42,_2,42.42f,_3,"42"));
        BOOST_TEST(connect_count==1);
        BOOST_TEST(synapse::emit<signal2_a3>(&e1,42,42.42f,"42")==0);
        BOOST_TEST(count==0);
        BOOST_TEST(synapse::emit<signal1_a3>(&e1,42,42.42f,"42")==1);
        BOOST_TEST(count==1);
        c1.reset();
        BOOST_TEST(connect_count==0);
            {
            boost::weak_ptr<synapse::synapse_detail::connection_list> & cl=synapse::synapse_detail::get_connection_list<signal1_a3>();
            BOOST_TEST(!(boost::weak_ptr<void>()<cl) && !(cl<boost::weak_ptr<void>()));
            }
        }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    void
    test_callback_a4( int & count, int a1, int a1_cmp, float a2, float a2_cmp, std::string const & a3, std::string const & a3_cmp, short & a4, short const & a4_cmp )
        {
        BOOST_TEST(a1==a1_cmp);
        BOOST_TEST(a2==a2_cmp);
        BOOST_TEST(a3==a3_cmp);
        BOOST_TEST(&a4==&a4_cmp);
        ++count;
        }
    void
    test_meta_callback_connect_a4( int & count, my_emitter_type * e1, synapse::connection & c, unsigned flags )
        {
        BOOST_TEST(e1==c.emitter<my_emitter_type>().get());
        count += (flags&synapse::meta::connect_flags::connecting)?1:-1;
        }
    typedef struct signal0_a4_(*signal0_a4)(int,float,char const *,short &);
    typedef struct signal1_a4_(*signal1_a4)(int,float,std::string const &,short &);
    typedef struct signal2_a4_(*signal2_a4)(int,float,std::string const &,short &);
    void
    test_a4()
        {
        int connect_count=0;
        int count=0;
        my_emitter_type e1;
        boost::shared_ptr<synapse::connection> mc_c1=synapse::connect<synapse::meta::connected<signal1_a4> >(synapse::meta::emitter(),boost::bind(&test_meta_callback_connect_a4,boost::ref(connect_count),&e1,_1,_2));
        BOOST_TEST(connect_count==0);
        short a4=42;
        boost::shared_ptr<synapse::connection> c1=synapse::connect<signal1_a4>(&e1,boost::bind(&test_callback_a4,boost::ref(count),_1,42,_2,42.42f,_3,"42",_4,boost::ref(a4)));
        BOOST_TEST(connect_count==1);
        BOOST_TEST(synapse::emit<signal2_a4>(&e1,42,42.42f,"42",a4)==0);
        BOOST_TEST(count==0);
        BOOST_TEST(synapse::emit<signal1_a4>(&e1,42,42.42f,"42",a4)==1);
        BOOST_TEST(count==1);
        c1.reset();
        BOOST_TEST(connect_count==0);
            {
            boost::weak_ptr<synapse::synapse_detail::connection_list> & cl=synapse::synapse_detail::get_connection_list<signal1_a4>();
            BOOST_TEST(!(boost::weak_ptr<void>()<cl) && !(cl<boost::weak_ptr<void>()));
            }
        }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    }

int
main( int argc, char const * argv[] )
    {
    test_a0();
    test_a1();
    test_a2();
    test_a3();
    test_a4();
    return boost::report_errors();
    }
