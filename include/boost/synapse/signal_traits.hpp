//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_21F2CE0D6E3F4C1BB14FE0290EA1F3C6
#define UUID_21F2CE0D6E3F4C1BB14FE0290EA1F3C6

namespace boost { namespace synapse { namespace meta { template <class Signal>  struct connected; } } }
namespace boost { namespace synapse { namespace meta { template <class Signal>  struct blocked; } } }

namespace
boost
    {
    namespace
    synapse
        {
        template <class Signal> struct signal_traits;
        template <class R>
        struct
        signal_traits<R(*)()>
            {
            typedef R signal_ret_id;
            typedef R(*signal_type)();
            static int const arity=0;
            typedef void signature();
            };
        template <class R,class A1>
        struct
        signal_traits<R(*)(A1)>
            {
            typedef R signal_ret_id;
            typedef R(*signal_type)(A1);
            static int const arity=1;
            typedef A1 arg1_type;
            typedef void signature(arg1_type);
            };
        template <class R,class A1,class A2>
        struct
        signal_traits<R(*)(A1,A2)>
            {
            typedef R signal_ret_id;
            typedef R(*signal_type)(A1,A2);
            static int const arity=2;
            typedef A1 arg1_type;
            typedef A2 arg2_type;
            typedef void signature(arg1_type,arg2_type);
            };
        template <class R,class A1,class A2,class A3>
        struct
        signal_traits<R(*)(A1,A2,A3)>
            {
            typedef R signal_ret_id;
            typedef R(*signal_type)(A1,A2,A3);
            static int const arity=3;
            typedef A1 arg1_type;
            typedef A2 arg2_type;
            typedef A3 arg3_type;
            typedef void signature(arg1_type,arg2_type,arg3_type);
            };
        template <class R,class A1,class A2,class A3,class A4>
        struct
        signal_traits<R(*)(A1,A2,A3,A4)>
            {
            typedef R signal_ret_id;
            typedef R(*signal_type)(A1,A2,A3,A4);
            static int const arity=4;
            typedef A1 arg1_type;
            typedef A2 arg2_type;
            typedef A3 arg3_type;
            typedef A4 arg4_type;
            typedef void signature(arg1_type,arg2_type,arg3_type,arg4_type);
            };
        template <class R,class A1,class A2,class A3,class A4,class A5>
        struct
        signal_traits<R(*)(A1,A2,A3,A4,A5)>
            {
            typedef R signal_ret_id;
            typedef R(*signal_type)(A1,A2,A3,A4,A5);
            static int const arity=5;
            typedef A1 arg1_type;
            typedef A2 arg2_type;
            typedef A3 arg3_type;
            typedef A4 arg4_type;
            typedef A5 arg5_type;
            typedef void signature(arg1_type,arg2_type,arg3_type,arg4_type,arg5_type);
            };
        template <class R,class A1,class A2,class A3,class A4,class A5,class A6>
        struct
        signal_traits<R(*)(A1,A2,A3,A4,A5,A6)>
            {
            typedef R signal_ret_id;
            typedef R(*signal_type)(A1,A2,A3,A4,A5,A6);
            static int const arity=6;
            typedef A1 arg1_type;
            typedef A2 arg2_type;
            typedef A3 arg3_type;
            typedef A4 arg4_type;
            typedef A5 arg5_type;
            typedef A6 arg6_type;
            typedef void signature(arg1_type,arg2_type,arg3_type,arg4_type,arg5_type,arg6_type);
            };
        template <class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7>
        struct
        signal_traits<R(*)(A1,A2,A3,A4,A5,A6,A7)>
            {
            typedef R signal_ret_id;
            typedef R(*signal_type)(A1,A2,A3,A4,A5,A6,A7);
            static int const arity=7;
            typedef A1 arg1_type;
            typedef A2 arg2_type;
            typedef A3 arg3_type;
            typedef A4 arg4_type;
            typedef A5 arg5_type;
            typedef A6 arg6_type;
            typedef A7 arg7_type;
            typedef void signature(arg1_type,arg2_type,arg3_type,arg4_type,arg5_type,arg6_type,arg7_type);
            };
        template <class R,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8>
        struct
        signal_traits<R(*)(A1,A2,A3,A4,A5,A6,A7,A8)>
            {
            typedef R signal_ret_id;
            typedef R(*signal_type)(A1,A2,A3,A4,A5,A6,A7,A8);
            static int const arity=8;
            typedef A1 arg1_type;
            typedef A2 arg2_type;
            typedef A3 arg3_type;
            typedef A4 arg4_type;
            typedef A5 arg5_type;
            typedef A6 arg6_type;
            typedef A7 arg7_type;
            typedef A8 arg8_type;
            typedef void signature(arg1_type,arg2_type,arg3_type,arg4_type,arg5_type,arg6_type,arg7_type,arg8_type);
            };
        template <class Signal>
        struct
        signal_traits<meta::connected<Signal> >:
            signal_traits<typename meta::connected<Signal>::type>
            {
            };
        template <class Signal>
        struct
        signal_traits<meta::blocked<Signal> >:
            signal_traits<typename meta::blocked<Signal>::type>
            {
            };
        template <class T>
        struct
        is_signal
            {
            static bool const value = signal_traits<T>::arity>=0;
            };
        namespace
        synapse_detail
            {
            template <bool,class> struct enable_if;
            template <class T> struct enable_if<true,T> { typedef T type; };
            }
        } 
    }

#endif
