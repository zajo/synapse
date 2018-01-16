//Copyright (c) 2015-2017 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//This example program demonstrates how to use Boost Synapse to bind a C-style callback API that uses
//function pointers. By using meta signals, such C-style callbacks can be connected directly through the
//Boost Synapse connect function. The program consists of several source files:

//callback_api.h/.c define an example 3rd-party API that uses C-style callbacks. It lets users set a single
//function pointer callback on objects of type api_handle. The callback is invoked with different values
//for the event argument to report on different events. In this case calling api_do_this generates
//API_EVENT_THIS, and calling api_do_that generates API_EVENT_THAT.

//synapsify.hpp/.cpp contain the machinery needed to handle Boost Synapse meta signals. Once
//initialized, user calls to connect<synapse_callback> are forwarded automatically to api_set_callback/
//api_clear_callback (from callback_api.h) if needed.

//main.cpp creates an api_handle object and then connects two different handlers that get called when
//the api_do_this/api_do_that is called.

#include "synapsify.hpp"
#include <boost/synapse/connect.hpp>
#include <iostream>

namespace synapse=boost::synapse;

namespace
{
    void handler1( api_handle * h, int event )
    {
        std::cout << "handler1 called on api_handle object at 0x" << h << ", event=" << event << std::endl;
    }
    void handler2( api_handle * h, int event )
    {
        std::cout << "handler2 called on api_handle object at 0x" << h << ", event=" << event << std::endl;
    }
}

int main()
{
    //Connect the meta signal handlers to deal with  api_set_callback and api_clear_callback
    //automatically -- see callback_api.h and synapsify.cpp.
    synapsify();

    //Use shared_ptr with a custom deleter to hold an api_handle object.
    boost::shared_ptr<api_handle> h(api_create_object(),&api_destroy_object);

    {
        //The meta signal handlers deal with api_set_callback and api_clear_callback as needed,
        //so now we can use Synapse to connect to the C-style API callbacks. Note that we can
        //create many connections even though the C-style API supports only a single callback
        //per api_handle object.
        boost::shared_ptr<synapse::connection> c1 = synapse::connect<synapse_callback>(h,&handler1);
        boost::shared_ptr<synapse::connection> c2 = synapse::connect<synapse_callback>(h,&handler2);

        //This invokes handler1 and handler2, in that order, passing API_EVENT_THIS.
        api_do_this(h.get());

        //This invokes handler1 and handler2, in that order, passing API_EVENT_THAT.
        api_do_that(h.get());
    }

    //At this point all synapse_callback connections have expired and the meta handlers have
    //cleared the callback on the api_handle object, so the calls below do not invoke any handlers.
    api_do_this(h.get());
    api_do_that(h.get());

    return 0;
}
