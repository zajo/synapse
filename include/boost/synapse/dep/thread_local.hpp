//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_CEDA579A5F8B11E6840250456066FB5E
#define UUID_CEDA579A5F8B11E6840250456066FB5E

#define BOOST_SYNAPSE_STATIC_THREAD_LOCAL(type,object) static thread_local type object
#define BOOST_SYNAPSE_STATIC_THREAD_LOCAL_INIT(type,object,init) static thread_local type object=init
#define BOOST_SYNAPSE_STATIC(type,object) static type object
#define BOOST_SYNAPSE_STATIC_INIT(type,object,init) static type object=init

#endif
