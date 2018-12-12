//Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_21F2CE0D6E3F4C1BB14FE0290EA1F3C6
#define UUID_21F2CE0D6E3F4C1BB14FE0290EA1F3C6

namespace boost { namespace synapse { namespace meta { template <class Signal>  struct connected; } } }
namespace boost { namespace synapse { namespace meta { template <class Signal>  struct blocked; } } }

namespace boost { namespace synapse {

    template <class Signal> struct signal_traits;

    template <class R,class... A>
    struct signal_traits<R(*)(A...)>
    {
        typedef R(*signal_type)(A...);
        typedef void signature(A...);
        static int const arity=sizeof...(A);
        static bool const is_thread_local=false;
    };

    template <class Signal>
    struct signal_traits<meta::connected<Signal> >:
        signal_traits<typename meta::connected<Signal>::type>
    {
        static bool const is_thread_local=true;
    };

    template <class Signal>
    struct signal_traits<meta::blocked<Signal> >:
        signal_traits<typename meta::blocked<Signal>::type>
    {
        static bool const is_thread_local=true;
    };

} }

#endif
