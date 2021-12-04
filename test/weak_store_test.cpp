//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/detail/weak_store.hpp>
#include "boost/core/lightweight_test.hpp"

namespace synapse=boost::synapse;

struct test_type { test_type() { } };

bool shared( std::shared_ptr<void const> const & a, std::shared_ptr<void const> const & b )
{
	return !a.owner_before(b) && !b.owner_before(a);
}

int main( int argc, char const * argv[] )
{
	{
		test_type x;
		synapse::synapse_detail::weak_store sx=synapse::synapse_detail::weak_store(&x);
		BOOST_TEST(!sx.lockable());
		BOOST_TEST_EQ(sx.maybe_lock<void const>().get(), &x);
		BOOST_TEST_EQ(sx.maybe_lock<void>().get(), &x);
		BOOST_TEST_EQ(sx.maybe_lock<test_type const>().get(), &x);
		BOOST_TEST_EQ(sx.maybe_lock<test_type>().get(), &x);
		BOOST_TEST(shared(sx.maybe_lock<void const>(),std::shared_ptr<void const>()));
		BOOST_TEST(shared(sx.maybe_lock<void>(),std::shared_ptr<void const>()));
		BOOST_TEST(shared(sx.maybe_lock<test_type const>(),std::shared_ptr<void const>()));
		BOOST_TEST(shared(sx.maybe_lock<test_type>(),std::shared_ptr<void const>()));
		BOOST_TEST(!sx.empty());
		BOOST_TEST(!sx.expired());
		sx.clear();
		BOOST_TEST(sx.empty());
		BOOST_TEST(sx.expired());
	}
	{
		test_type const x;
		synapse::synapse_detail::weak_store sx=synapse::synapse_detail::weak_store(&x);
		BOOST_TEST(!sx.lockable());
		BOOST_TEST_EQ(sx.maybe_lock<void const>().get(), &x);
		BOOST_TEST(!sx.maybe_lock<void>());
		BOOST_TEST_EQ(sx.maybe_lock<test_type const>().get(), &x);
		BOOST_TEST(!sx.maybe_lock<test_type>());
		BOOST_TEST(shared(sx.maybe_lock<void const>(),std::shared_ptr<void const>()));
		BOOST_TEST(shared(sx.maybe_lock<void>(),std::shared_ptr<void const>()));
		BOOST_TEST(shared(sx.maybe_lock<test_type const>(),std::shared_ptr<void const>()));
		BOOST_TEST(shared(sx.maybe_lock<test_type>(),std::shared_ptr<void const>()));
		BOOST_TEST(!sx.empty());
		BOOST_TEST(!sx.expired());
		sx.clear();
		BOOST_TEST(sx.empty());
		BOOST_TEST(sx.expired());
	}
	{
		std::shared_ptr<test_type> x(new test_type);
		synapse::synapse_detail::weak_store sx=synapse::synapse_detail::weak_store(std::weak_ptr<test_type>(x));
		BOOST_TEST(sx.lockable());
		BOOST_TEST_EQ(sx.maybe_lock<void const>(), x);
		BOOST_TEST_EQ(sx.maybe_lock<void>(), x);
		BOOST_TEST_EQ(sx.maybe_lock<test_type const>(), x);
		BOOST_TEST_EQ(sx.maybe_lock<test_type>(), x);
		BOOST_TEST(shared(sx.maybe_lock<void const>(),x));
		BOOST_TEST(shared(sx.maybe_lock<void>(),x));
		BOOST_TEST(shared(sx.maybe_lock<test_type const>(),x));
		BOOST_TEST(shared(sx.maybe_lock<test_type>(),x));
		BOOST_TEST(!sx.empty());
		BOOST_TEST(!sx.expired());
		x.reset();
		BOOST_TEST(sx.expired());
		BOOST_TEST(!sx.empty());
		BOOST_TEST(!sx.maybe_lock<void const>());
		BOOST_TEST(!sx.maybe_lock<void>());
		BOOST_TEST(!sx.maybe_lock<test_type const>());
		BOOST_TEST(!sx.maybe_lock<test_type>());
		sx.clear();
		BOOST_TEST(sx.empty());
		BOOST_TEST(sx.expired());
	}
	{
		std::shared_ptr<test_type const> x(new test_type);
		synapse::synapse_detail::weak_store sx=synapse::synapse_detail::weak_store(std::weak_ptr<test_type const>(x));
		BOOST_TEST(sx.lockable());
		BOOST_TEST_EQ(sx.maybe_lock<void const>(), x);
		BOOST_TEST(!sx.maybe_lock<void>());
		BOOST_TEST_EQ(sx.maybe_lock<test_type const>(), x);
		BOOST_TEST(!sx.maybe_lock<test_type>());
		BOOST_TEST(shared(sx.maybe_lock<void const>(),x));
		BOOST_TEST(shared(sx.maybe_lock<void>(),std::shared_ptr<void const>()));
		BOOST_TEST(shared(sx.maybe_lock<test_type const>(),x));
		BOOST_TEST(shared(sx.maybe_lock<test_type>(),std::shared_ptr<void const>()));
		BOOST_TEST(!sx.empty());
		BOOST_TEST(!sx.expired());
		x.reset();
		BOOST_TEST(sx.expired());
		BOOST_TEST(!sx.empty());
		BOOST_TEST(!sx.maybe_lock<void const>());
		BOOST_TEST(!sx.maybe_lock<void>());
		BOOST_TEST(!sx.maybe_lock<test_type const>());
		BOOST_TEST(!sx.maybe_lock<test_type>());
		sx.clear();
		BOOST_TEST(sx.empty());
		BOOST_TEST(sx.expired());
	}
	return boost::report_errors();
}
