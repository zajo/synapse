//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/thread_local_queue.hpp>
#include <boost/synapse/connect.hpp>
#include <boost/thread.hpp>
#include <boost/detail/lightweight_test.hpp>

namespace synapse=boost::synapse;
using synapse::shared_ptr;

namespace
    {
    int const iterations=10000;
    int emitter;
    typedef struct signal1_(*signal1)();
    typedef struct signal2_(*signal2)();
    typedef struct terminate_thread_(*terminate_thread)();
    void
    emitting_thread( boost::barrier & b, void const * terminate )
        {
        shared_ptr<synapse::thread_local_queue> tlq=synapse::create_thread_local_queue();
        bool keep_going=true;
        shared_ptr<synapse::connection> c=synapse::connect<terminate_thread>(terminate,
            [&keep_going]()
            {
            keep_going=false;
            });
        b.wait();
        while( keep_going )
            {
            (void) synapse::emit<signal1>(&emitter);
            (void) synapse::emit<signal2>(&emitter);
            (void) poll(*tlq);
            }
        }
    void
    test( int emitting_thread_count, int per_thread_emit_count, int series_count )
        {
        assert(emitting_thread_count>0);
        assert(per_thread_emit_count>0);
        assert(series_count>0);
        boost::thread_group tgr;
        boost::barrier b(emitting_thread_count+1);
        int terminate;
        for( int i=0; i!=emitting_thread_count; ++i )
            tgr.create_thread( [&b,&terminate]() { emitting_thread(b,&terminate); } );
        b.wait();
        shared_ptr<synapse::connection> c1=synapse::connect<signal1>(&emitter,[](){});
        shared_ptr<synapse::connection> c2=synapse::connect<signal2>(&emitter,[](){});
        for( int i=0; i!=series_count; ++i )
            {
            shared_ptr<synapse::thread_local_queue> tlq=synapse::create_thread_local_queue();
            for( int j=0; j<emitting_thread_count*per_thread_emit_count; )
                {
                int n=poll(*tlq);
                j+=n;
                }
            }
        int n=synapse::emit<terminate_thread>(&terminate);
        BOOST_TEST(n==emitting_thread_count);
        tgr.join_all();
        }
    }

int
main( int argc, char const * argv[] )
    {
    test(1,1000,10);
    test(20,1000,100);
    return boost::report_errors();
    }
