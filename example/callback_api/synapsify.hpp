//Copyright (c) 2015-2017 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_B889D94CFF2011E4BC4B408F1D5D46B0
#define UUID_B889D94CFF2011E4BC4B408F1D5D46B0

extern "C"
    {
    #include "callback_api.h"
    }

//This is the Synapse signal that corresponds to the callbacks received from the C-style callback API.
typedef struct synapse_callback_(*synapse_callback)( api_handle *, int event );

//Connect synapse::meta::connected<synapse_callback> and synapse::meta::disconnected<synapse_callback>
//to set/clear the api_handle callbacks when synapse_callback signals are connected or disconnected.
void synapsify();

#endif
