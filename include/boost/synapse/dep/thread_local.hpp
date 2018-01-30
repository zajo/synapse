//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_CEDA579A5F8B11E6840250456066FB5E
#define UUID_CEDA579A5F8B11E6840250456066FB5E

#include <boost/synapse/synapse_detail/config.hpp>

#ifdef BOOST_SYNAPSE_NO_THREADS
#define BOOST_SYNAPSE_THREAD_LOCAL_
#else
#define BOOST_SYNAPSE_THREAD_LOCAL_ thread_local
#endif

#ifndef BOOST_SYNAPSE_THREAD_LOCAL
#define BOOST_SYNAPSE_THREAD_LOCAL(type,object) static BOOST_SYNAPSE_THREAD_LOCAL_ type object
#endif
#ifndef BOOST_SYNAPSE_THREAD_LOCAL_INIT
#define BOOST_SYNAPSE_THREAD_LOCAL_INIT(type,object,init) static BOOST_SYNAPSE_THREAD_LOCAL_ type object init
#endif
#ifndef BOOST_SYNAPSE_STATIC
#define BOOST_SYNAPSE_STATIC(type,object) static type object
#endif
#ifndef BOOST_SYNAPSE_STATIC_INIT
#define BOOST_SYNAPSE_STATIC_INIT(type,object,init) static type object init
#endif

#endif
