//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/synapse_detail/config.hpp>

#ifdef BOOST_SYNAPSE_NO_THREADS

int
main( int argc, char const * argv[ ] )
    {
    return 0;
    }

#else

#include <boost/synapse/thread_local_queue.hpp>
#include <boost/synapse/connect.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace synapse=boost::synapse;
using synapse::shared_ptr;
using synapse::weak_ptr;

namespace
    {
    int const iteration_count=50;
    typedef struct sig1_(*sig1)();
    typedef struct sig2_(*sig2)();
    template <class Signal>
    void
    emitting_thread( int & counter )
        {
        for( int i=0; i!=iteration_count; ++i )
            {
            synapse::emit<Signal>(&counter);
            boost::this_thread::sleep_for(boost::chrono::milliseconds(rand()%100));
            }
        }
    void
    test()
        {
        shared_ptr<synapse::thread_local_queue> tlq=synapse::create_thread_local_queue();
        int counter1=0; shared_ptr<synapse::connection const> c1=synapse::connect<sig1>(&counter1,[&counter1](){++counter1;});
        int counter2=0; shared_ptr<synapse::connection const> c2=synapse::connect<sig2>(&counter2,[&counter2](){++counter2;});
        boost::thread th1(boost::bind(&emitting_thread<sig1>,boost::ref(counter1)));
        boost::thread th2(boost::bind(&emitting_thread<sig2>,boost::ref(counter2)));
        while( counter1!=iteration_count || counter2!=iteration_count )
            {
            int n=synapse::wait(*tlq);
            BOOST_TEST(n>0);
            BOOST_TEST(counter1>=0);
            BOOST_TEST(counter1<=iteration_count);
            BOOST_TEST(counter2>=0);
            BOOST_TEST(counter2<=iteration_count);
            }
        th1.join();
        th2.join();
        tlq.reset();
        BOOST_TEST(counter1==iteration_count);
        BOOST_TEST(counter2==iteration_count);
        }
    }

int
main( int argc, char const * argv[] )
    {
    test();
    return boost::report_errors();
    }

#endif
