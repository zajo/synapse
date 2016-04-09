//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//This header defines a typical C-style callback API. Users can create api_handle
//objects and then set a C function pointer callback on them.

#ifndef UUID_745C68F8FF1F11E4A5A6028E1D5D46B0
#define UUID_745C68F8FF1F11E4A5A6028E1D5D46B0

typedef struct api_handle api_handle;
api_handle * api_create_object();
void api_destroy_object( api_handle * );

//The user_data pointer is typical in C-style callbacks APIs, so it's included
//here even though this example has no use for it.
typedef void (*api_callback_type)( api_handle *, void * user_data, int event );

void api_set_callback( api_handle *, api_callback_type, void * user_data );
void api_clear_callback( api_handle * );

//When api_do_this is called, it calls the callback passing API_EVENT_THIS as
//the event argument; api_do_that calls the callback with API_EVENT_THAT.
#define API_EVENT_THIS 1
#define API_EVENT_THAT 2
void api_do_this( api_handle * );
void api_do_that( api_handle * );

#endif
