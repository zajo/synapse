//Copyright (c) 2015-2017 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "synapsify.hpp"
#include <boost/synapse/connect.hpp>
#include <boost/synapse/connection.hpp>
#include <iostream>

namespace synapse=boost::synapse;

namespace
{
    void emit_fwd( api_handle * h, void *, int v )
    {
        (void) synapse::emit<synapse_callback>(h,h,v);
    }
}

void synapsify()
{
    (void) synapse::connect<synapse::meta::connected<synapse_callback> >(synapse::meta::emitter(),
        [ ]( synapse::connection & c, unsigned flags )
        {
            boost::shared_ptr<api_handle> h=c.emitter<api_handle>();
            std::cout << "Detected synapse_callback " << ((flags&synapse::meta::connect_flags::connecting)?"":"dis") << "connection on api_handle at 0x" << h.get();
            if( flags&synapse::meta::connect_flags::first_for_this_emitter )
            {
                assert(flags&synapse::meta::connect_flags::connecting);
                std::cout << " (first connection, calling api_set_callback)";
                api_set_callback(h.get(),&emit_fwd,0);
            }
            else if( flags&synapse::meta::connect_flags::last_for_this_emitter )
            {
                assert(!(flags&synapse::meta::connect_flags::connecting));
                std::cout << " (last connection, calling api_clear_callback)";
                api_clear_callback(h.get());
            }
            std::cout << std::endl;
        } );
}
