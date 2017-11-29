//Copyright (c) 2015-2017 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "logger.h"
#include <boost/synapse/translate.hpp>
#include <vector>

namespace synapse=boost::synapse;

namespace
    {
    //Emitters, indexed by severity.
    std::vector<boost::weak_ptr<void const> > emitters_;

    void
    log_string( boost::shared_ptr<FILE> const & f, char const * str )
        {
        assert(f);
        assert(str!=0);
        (void) fprintf(f.get(),"%s",str);
        }

    boost::weak_ptr<void const> const &
    severity_( int s )
        {
        assert(s>=0);
        assert(!emitters_.empty());
        return s>=emitters_.size()? emitters_.back() : emitters_[s];
        }
    }

struct
logger
    {
    private:
    logger( logger const & );
    logger & operator=( logger const & );
    public:
    explicit
    logger( int max_severity )
        {
        std::vector<boost::weak_ptr<void const> >(max_severity).swap(emitters_);
        }
    };

boost::shared_ptr<logger>
init_logger( int max_severity )
    {
    assert(max_severity>0);
    boost::shared_ptr<logger> l(new logger(max_severity));

    //Populate the static emitters vector: each emitter is a weak_ptr initialized from a shared_ptr alias
    //of the logger object, but with a unique address.
    for( int i=0; i!=max_severity; ++i )
        emitters_[i]=boost::shared_ptr<void const>(l,&emitters_[i]);

    //Translate signals from higher severity emitters to lower severity emitters. This way a high severity
    //message will automatically trickle down to lower severity emitters. With this approach it is possible
    //to build a more complex translation DAG if needed.
    for( int i=0; i!=max_severity-1; ++i )
        (void) synapse::translate<log_message,log_message>(emitters_[i+1],emitters_[i]);

    return l;
    }

void
add_log_target( logger & l, boost::shared_ptr<FILE> const & target, int min_severity )
    {
    assert(target);
    assert(min_severity>=0);
    assert(min_severity<emitters_.size());

    //Connect the appropriate emitter based on severity. Since signals from higher severity emitters are
    //translated to lower severities, the target will only get the messages with severity >= min_severity.
    (void) synapse::connect<log_message>(severity_(min_severity),
        [target]( char const * str )
            {
            log_string(target,str);
            } );
    }

void const *
severity( int s )
    {
    assert(s>=0);
    assert(!emitters_.empty());
    return (s>=emitters_.size()? emitters_.back() : emitters_[s]).lock().get();
    }
