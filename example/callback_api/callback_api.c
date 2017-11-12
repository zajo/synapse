//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "callback_api.h"
#include <stdlib.h>

struct
api_handle
    {
    api_callback_type callback;
    void * user_data;
    };

api_handle *
api_create_object()
    {
    api_handle * p=(api_handle *)malloc(sizeof(api_handle));
    p->callback=0;
    return p;
    }

void
api_destroy_object( api_handle * p )
    {
    free(p);
    }

void
api_set_callback( api_handle * p, api_callback_type c, void * user_data )
    {
    p->callback=c;
    p->user_data=user_data;
    }

void
api_clear_callback( api_handle * p )
    {
    p->callback=0;
    }

void
api_do_this( api_handle * p )
    {
    if( p->callback )
        p->callback(p,p->user_data,API_EVENT_THIS);
    }

void
api_do_that( api_handle * p )
    {
    if( p->callback )
        p->callback(p,p->user_data,API_EVENT_THAT);
    }
