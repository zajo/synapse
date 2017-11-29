//Copyright (c) 2015-2017 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/synapse_detail/config.hpp>

#ifdef BOOST_SYNAPSE_DISABLE_THREADS

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
    int const thread_count=10;
    int const iteration_count=1000;
    void
    emitting_thread( weak_ptr<synapse::thread_local_queue> const & tlq, int & counter, boost::thread::id tid )
        {
        for(;;)
            if( shared_ptr<synapse::thread_local_queue> p=tlq.lock() )
                post(*p,[&counter,tid]()
                    {
                    BOOST_TEST(boost::this_thread::get_id()==tid);
                    ++counter;
                    } );
            else
                break;
        }
    void
    consuming_thread( )
        {
        assert(iteration_count>0);
        int count=0;
        shared_ptr<synapse::thread_local_queue> tlq=synapse::create_thread_local_queue();
        boost::thread th(boost::bind(&emitting_thread,weak_ptr<synapse::thread_local_queue>(tlq),boost::ref(count),boost::this_thread::get_id()));
        while( count<iteration_count )
            poll(*tlq);
        tlq.reset();
        th.join();
        }
    }

int
main( int argc, char const * argv[] )
    {
    boost::thread_group tgr;
    for( int i=0; i!=thread_count; ++i )
        tgr.create_thread(&consuming_thread);
    tgr.join_all();
    return boost::report_errors();
    }

#endif
