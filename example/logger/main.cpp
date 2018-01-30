//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//This program demonstrates how to use Boost Synapse to build a simple logging system.
//Logging is done by emitting log_message signals from different emitters based on the
//log message's severity.

#include "logger.h"
#include <boost/synapse/emit.hpp>

namespace synapse=boost::synapse;

void do_some_logging()
{
    synapse::emit<log_message>(severity(0),"Message 1, Severity 0\n");
    synapse::emit<log_message>(severity(100),"Message 2, Severity 100\n");
    synapse::emit<log_message>(severity(2),"Message 3, Severity 2\n");
}

int main()
{
    //Create a logger that can distinguish between up to 3 severity levels: 0, 1 and 2.
    boost::shared_ptr<logger> const l=init_logger(3);

    //Severity 0 and 1 go to stdout, severity 2 and above go to stdout and stderr.
    add_log_target(*l,boost::shared_ptr<FILE>(stdout,[ ]( FILE * ) { }),0);
    add_log_target(*l,boost::shared_ptr<FILE>(stderr,[ ]( FILE * ) { }),2);

    do_some_logging();
}
