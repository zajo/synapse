//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_C1FB0C44F61511E4B92291521D5D46B0
#define UUID_C1FB0C44F61511E4B92291521D5D46B0

namespace boost { template <class> class shared_ptr; }

#include <stdio.h>

struct logger;

//Create a logger object that can discriminate between messages based on severity up to the
//specified maximum. Messages with higher max_severity are assumed to be of max_severity.
boost::shared_ptr<logger> init_logger( int max_severity );

//Add a target for logging messages with at least the specified severity.
void add_log_target( logger &, boost::shared_ptr<FILE> const &, int min_severity );

//Get a Boost Synapse emitter based on the message's severity.
void const * severity( int severity );

//Emit this Boost Synapse signal to log a message.
typedef struct log_message_(*log_message)( char const * );

#endif
