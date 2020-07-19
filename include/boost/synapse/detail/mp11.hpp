#ifndef BOOST_SYNAPSE_DETAIL_MP11_HPP_INCLUDED
#define BOOST_SYNAPSE_DETAIL_MP11_HPP_INCLUDED

// Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.
// Copyright 2015, 2017 Peter Dimov.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// This header contains code copied from Boost MP11, implementing tuple_apply.
// This is the only part Synapse needs from mp11, in order to not require C++17.

#ifndef BOOST_SYNAPSE_ENABLE_WARNINGS
#	if defined(__clang__)
#		pragma clang system_header
#	elif (__GNUC__*100+__GNUC_MINOR__>301)
#		pragma GCC system_header
#	elif defined(_MSC_VER)
#		pragma warning(push,1)
#	endif
#endif

#include <boost/synapse/detail/config.hpp>
#include <tuple>
#include <utility>
#include <type_traits>

#if defined(__has_builtin)
#   if __has_builtin(__make_integer_seq)
#		define BOOST_SYNAPSE_HAS_MAKE_INTEGER_SEQ
#	endif
#endif

#define BOOST_SYNAPSE_CONSTEXPR constexpr

namespace boost { namespace synapse { namespace synapse_detail_mp11 {

// integer_sequence
template<class T, T... I> struct integer_sequence
{
};

#if defined(BOOST_SYNAPSE_HAS_MAKE_INTEGER_SEQ)

template<class T, T N> using make_integer_sequence = __make_integer_seq<integer_sequence, T, N>;

#else

// detail::make_integer_sequence_impl
namespace detail
{

// iseq_if_c
template<bool C, class T, class E> struct iseq_if_c_impl;

template<class T, class E> struct iseq_if_c_impl<true, T, E>
{
	using type = T;
};

template<class T, class E> struct iseq_if_c_impl<false, T, E>
{
	using type = E;
};

template<bool C, class T, class E> using iseq_if_c = typename iseq_if_c_impl<C, T, E>::type;

// iseq_identity
template<class T> struct iseq_identity
{
	using type = T;
};

template<class S1, class S2> struct append_integer_sequence;

template<class T, T... I, T... J> struct append_integer_sequence<integer_sequence<T, I...>, integer_sequence<T, J...>>
{
	using type = integer_sequence< T, I..., ( J + sizeof...(I) )... >;
};

template<class T, T N> struct make_integer_sequence_impl;

template<class T, T N> struct make_integer_sequence_impl_
{
private:

	static_assert( N >= 0, "make_integer_sequence<T, N>: N must not be negative" );

	static T const M = N / 2;
	static T const R = N % 2;

	using S1 = typename make_integer_sequence_impl<T, M>::type;
	using S2 = typename append_integer_sequence<S1, S1>::type;
	using S3 = typename make_integer_sequence_impl<T, R>::type;
	using S4 = typename append_integer_sequence<S2, S3>::type;

public:

	using type = S4;
};

template<class T, T N> struct make_integer_sequence_impl: iseq_if_c<N == 0, iseq_identity<integer_sequence<T>>, iseq_if_c<N == 1, iseq_identity<integer_sequence<T, 0>>, make_integer_sequence_impl_<T, N> > >
{
};

} // namespace detail

// make_integer_sequence
template<class T, T N> using make_integer_sequence = typename detail::make_integer_sequence_impl<T, N>::type;

#endif // defined(BOOST_SYNAPSE_HAS_MAKE_INTEGER_SEQ)

// index_sequence
template<std::size_t... I> using index_sequence = integer_sequence<std::size_t, I...>;

// make_index_sequence
template<std::size_t N> using make_index_sequence = make_integer_sequence<std::size_t, N>;

////////////////////////////////////////

// tuple_apply
namespace detail
{

template<class F, class Tp, std::size_t... J> BOOST_SYNAPSE_CONSTEXPR auto tuple_apply_impl( F && f, Tp && tp, integer_sequence<std::size_t, J...> )
	-> decltype( std::forward<F>(f)( std::get<J>(std::forward<Tp>(tp))... ) )
{
	return std::forward<F>(f)( std::get<J>(std::forward<Tp>(tp))... );
}

} // namespace detail

template<class F, class Tp,
	class Seq = make_index_sequence<std::tuple_size<typename std::remove_reference<Tp>::type>::value>>
BOOST_SYNAPSE_CONSTEXPR auto tuple_apply( F && f, Tp && tp )
	-> decltype( detail::tuple_apply_impl( std::forward<F>(f), std::forward<Tp>(tp), Seq() ) )
{
	return detail::tuple_apply_impl( std::forward<F>(f), std::forward<Tp>(tp), Seq() );
}

} } }

#endif
