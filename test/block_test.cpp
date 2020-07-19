//Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/synapse/block.hpp>
#include <boost/synapse/blocker.hpp>
#include "boost/core/lightweight_test.hpp"

namespace synapse=boost::synapse;
using synapse::shared_ptr;
using synapse::weak_ptr;

namespace
{
	struct my_emitter_type { };

	struct meta_data
	{
		shared_ptr<my_emitter_type> e;
		my_emitter_type * ep;
		synapse::blocker * eb;
		int count;

		meta_data():
			ep(0),
			eb(0),
			count(0)
		{
		}

		void reset()
		{
			e.reset();
			ep=0;
			eb=0;
		}
	};

	bool does_not_own( shared_ptr<void const> const & e )
	{
		return !(e.owner_before(shared_ptr<void const>())) && !(shared_ptr<void const>().owner_before(e));
	}

	template <class Signal>
	bool emitter_blocked( void const * e )
	{
		shared_ptr<synapse::synapse_detail::thread_local_signal_data> const & tlsd=synapse::synapse_detail::get_thread_local_signal_data<Signal>(false);
		return tlsd && tlsd->emitter_blocked_(*tlsd,e);
	}

	template <class Signal>
	meta_data & the_meta_data()
	{
		static meta_data d;
		return d;
	}
	typedef struct signal1_(*signal1)();
	typedef struct signal2_(*signal2)();
}

namespace boost { namespace synapse {

	namespace synapse_detail
	{
		template <class Signal>
		int emit_meta_blocked( blocker & eb, bool emitter_blocked )
		{
			meta_data & d=the_meta_data<Signal>();
			BOOST_TEST(!d.e);
			BOOST_TEST(!d.ep);
			BOOST_TEST(!d.eb);
			d.e=eb.emitter<my_emitter_type>();
			d.ep=eb.emitter<my_emitter_type>().get();
			d.eb=&eb;
			d.count += emitter_blocked?1:-1;
			return 0;
		}
	}

} }

namespace
{
	void block_test()
	{
		{
			my_emitter_type e1;
			my_emitter_type e2;
			BOOST_TEST(!emitter_blocked<signal1>(&e1));
			BOOST_TEST(!emitter_blocked<signal2>(&e1));
			BOOST_TEST(!emitter_blocked<signal1>(&e2));
			BOOST_TEST(!emitter_blocked<signal2>(&e2));
			shared_ptr<synapse::blocker> eb1=synapse::block<signal1>(&e1);
			{
				shared_ptr<synapse::blocker> b1=synapse::block<signal1>(&e1);
				shared_ptr<synapse::blocker> b2=synapse::block<signal1>(&e1);
				BOOST_TEST_EQ(eb1, b1);
				BOOST_TEST_EQ(eb1, b2);
			}
			BOOST_TEST(does_not_own(the_meta_data<signal1>().e));
			BOOST_TEST_EQ(the_meta_data<signal1>().ep, &e1);
			BOOST_TEST_EQ(the_meta_data<signal1>().eb, eb1.get());
			BOOST_TEST_EQ(the_meta_data<signal1>().count, 1);
			the_meta_data<signal1>().reset();
			BOOST_TEST(emitter_blocked<signal1>(&e1));
			BOOST_TEST(!emitter_blocked<signal2>(&e1));
			BOOST_TEST(!emitter_blocked<signal1>(&e2));
			BOOST_TEST(!emitter_blocked<signal2>(&e2));
			shared_ptr<synapse::blocker> eb2=synapse::block<signal2>(&e1);
			{
				shared_ptr<synapse::blocker> b1=synapse::block<signal2>(&e1);
				shared_ptr<synapse::blocker> b2=synapse::block<signal2>(&e1);
				BOOST_TEST_EQ(eb2, b1);
				BOOST_TEST_EQ(eb2, b2);
			}
			BOOST_TEST(does_not_own(the_meta_data<signal2>().e));
			BOOST_TEST_EQ(the_meta_data<signal2>().ep, &e1);
			BOOST_TEST_EQ(the_meta_data<signal2>().eb, eb2.get());
			BOOST_TEST_EQ(the_meta_data<signal2>().count, 1);
			the_meta_data<signal2>().reset();
			BOOST_TEST(emitter_blocked<signal1>(&e1));
			BOOST_TEST(emitter_blocked<signal2>(&e1));
			BOOST_TEST(!emitter_blocked<signal1>(&e2));
			BOOST_TEST(!emitter_blocked<signal2>(&e2));
			shared_ptr<synapse::blocker> eb3=synapse::block<signal1>(&e2);
			{
				shared_ptr<synapse::blocker> b1=synapse::block<signal1>(&e2);
				shared_ptr<synapse::blocker> b2=synapse::block<signal1>(&e2);
				BOOST_TEST_EQ(eb3, b1);
				BOOST_TEST_EQ(eb3, b2);
			}
			BOOST_TEST(does_not_own(the_meta_data<signal1>().e));
			BOOST_TEST_EQ(the_meta_data<signal1>().ep, &e2);
			BOOST_TEST_EQ(the_meta_data<signal1>().eb, eb3.get());
			BOOST_TEST_EQ(the_meta_data<signal1>().count, 2);
			the_meta_data<signal1>().reset();
			BOOST_TEST(emitter_blocked<signal1>(&e1));
			BOOST_TEST(emitter_blocked<signal2>(&e1));
			BOOST_TEST(emitter_blocked<signal1>(&e2));
			BOOST_TEST(!emitter_blocked<signal2>(&e2));
			shared_ptr<synapse::blocker> eb4=synapse::block<signal2>(&e2);
			{
				shared_ptr<synapse::blocker> b1=synapse::block<signal2>(&e2);
				shared_ptr<synapse::blocker> b2=synapse::block<signal2>(&e2);
				BOOST_TEST_EQ(eb4, b1);
				BOOST_TEST_EQ(eb4, b2);
			}
			BOOST_TEST(does_not_own(the_meta_data<signal2>().e));
			BOOST_TEST_EQ(the_meta_data<signal2>().ep, &e2);
			BOOST_TEST_EQ(the_meta_data<signal2>().eb, eb4.get());
			BOOST_TEST_EQ(the_meta_data<signal2>().count, 2);
			the_meta_data<signal2>().reset();
			BOOST_TEST(emitter_blocked<signal1>(&e1));
			BOOST_TEST(emitter_blocked<signal2>(&e1));
			BOOST_TEST(emitter_blocked<signal1>(&e2));
			BOOST_TEST(emitter_blocked<signal2>(&e2));
			{
				synapse::blocker * eb=eb1.get();
				eb1.reset();
				BOOST_TEST(!synapse::synapse_detail::get_thread_local_signal_data<signal1>(false)->bl_.expired());
				BOOST_TEST(does_not_own(the_meta_data<signal1>().e));
				BOOST_TEST_EQ(the_meta_data<signal1>().ep, &e1);
				BOOST_TEST_EQ(the_meta_data<signal1>().eb, eb);
			}
			BOOST_TEST_EQ(the_meta_data<signal1>().count, 1);
			the_meta_data<signal1>().reset();
			BOOST_TEST(!emitter_blocked<signal1>(&e1));
			BOOST_TEST(emitter_blocked<signal2>(&e1));
			BOOST_TEST(emitter_blocked<signal1>(&e2));
			BOOST_TEST(emitter_blocked<signal2>(&e2));
			{
				synapse::blocker * eb=eb3.get();
				eb3.reset();
				BOOST_TEST(synapse::synapse_detail::get_thread_local_signal_data<signal1>(false)->bl_.expired());
				BOOST_TEST(
					!(weak_ptr<synapse::synapse_detail::thread_local_signal_data::blocked_emitters_list>().owner_before(synapse::synapse_detail::get_thread_local_signal_data<signal1>(false)->bl_)) &&
					!(synapse::synapse_detail::get_thread_local_signal_data<signal1>(false)->bl_.owner_before(weak_ptr<synapse::synapse_detail::thread_local_signal_data::blocked_emitters_list>())) );
				BOOST_TEST(does_not_own(the_meta_data<signal1>().e));
				BOOST_TEST_EQ(the_meta_data<signal1>().ep, &e2);
				BOOST_TEST_EQ(the_meta_data<signal1>().eb, eb);
			}
			BOOST_TEST_EQ(the_meta_data<signal1>().count, 0);
			the_meta_data<signal1>().reset();
			BOOST_TEST(!emitter_blocked<signal1>(&e1));
			BOOST_TEST(emitter_blocked<signal2>(&e1));
			BOOST_TEST(!emitter_blocked<signal1>(&e2));
			BOOST_TEST(emitter_blocked<signal2>(&e2));
			{
				synapse::blocker * eb=eb2.get();
				eb2.reset();
				BOOST_TEST(!synapse::synapse_detail::get_thread_local_signal_data<signal2>(false)->bl_.expired());
				BOOST_TEST(does_not_own(the_meta_data<signal2>().e));
				BOOST_TEST_EQ(the_meta_data<signal2>().ep, &e1);
				BOOST_TEST_EQ(the_meta_data<signal2>().eb, eb);
			}
			BOOST_TEST_EQ(the_meta_data<signal2>().count, 1);
			the_meta_data<signal2>().reset();
			BOOST_TEST(!emitter_blocked<signal1>(&e1));
			BOOST_TEST(!emitter_blocked<signal2>(&e1));
			BOOST_TEST(!emitter_blocked<signal1>(&e2));
			BOOST_TEST(emitter_blocked<signal2>(&e2));
			{
				synapse::blocker * eb=eb4.get();
				eb4.reset();
				BOOST_TEST(synapse::synapse_detail::get_thread_local_signal_data<signal2>(false)->bl_.expired());
				BOOST_TEST(
					!(weak_ptr<synapse::synapse_detail::thread_local_signal_data::blocked_emitters_list>().owner_before(synapse::synapse_detail::get_thread_local_signal_data<signal2>(false)->bl_)) &&
					!(synapse::synapse_detail::get_thread_local_signal_data<signal2>(false)->bl_.owner_before(weak_ptr<synapse::synapse_detail::thread_local_signal_data::blocked_emitters_list>())) );
				BOOST_TEST(does_not_own(the_meta_data<signal2>().e));
				BOOST_TEST_EQ(the_meta_data<signal2>().ep, &e2);
				BOOST_TEST_EQ(the_meta_data<signal2>().eb, eb);
			}
			BOOST_TEST_EQ(the_meta_data<signal2>().count, 0);
			the_meta_data<signal2>().reset();
			BOOST_TEST(!emitter_blocked<signal1>(&e1));
			BOOST_TEST(!emitter_blocked<signal2>(&e1));
			BOOST_TEST(!emitter_blocked<signal1>(&e2));
			BOOST_TEST(!emitter_blocked<signal2>(&e2));
		}

		{
			shared_ptr<my_emitter_type> e(new my_emitter_type);
			BOOST_TEST(!emitter_blocked<signal1>(e.get()));
			BOOST_TEST(the_meta_data<signal1>().count==0);
			shared_ptr<synapse::blocker> eb=synapse::block<signal1>(e);
			{
				shared_ptr<synapse::blocker> b1=synapse::block<signal1>(e);
				shared_ptr<synapse::blocker> b2=synapse::block<signal1>(e);
				BOOST_TEST_EQ(eb, b1);
				BOOST_TEST_EQ(eb, b2);
			}
			BOOST_TEST_EQ(the_meta_data<signal1>().e, e);
			BOOST_TEST_EQ(the_meta_data<signal1>().ep, e.get());
			BOOST_TEST_EQ(the_meta_data<signal1>().eb, eb.get());
			BOOST_TEST_EQ(the_meta_data<signal1>().count, 1);
			BOOST_TEST(emitter_blocked<signal1>(e.get()));
			the_meta_data<signal1>().reset();
			e.reset();
			BOOST_TEST_EQ(the_meta_data<signal1>().eb, nullptr);
			BOOST_TEST_EQ(the_meta_data<signal1>().count, 1);
			the_meta_data<signal1>().reset();
			{
				synapse::blocker * p=eb.get();
				eb.reset();
				BOOST_TEST(!the_meta_data<signal1>().e);
				BOOST_TEST(!the_meta_data<signal1>().ep);
				BOOST_TEST_EQ(the_meta_data<signal1>().eb, p);
			}
			BOOST_TEST_EQ(the_meta_data<signal1>().count, 0);
			the_meta_data<signal1>().count=0;
			the_meta_data<signal1>().reset();
		}

		{
			shared_ptr<my_emitter_type> e(new my_emitter_type);
			BOOST_TEST(!emitter_blocked<signal1>(e.get()));
			BOOST_TEST_EQ(the_meta_data<signal1>().count, 0);
			shared_ptr<synapse::blocker> eb=synapse::block<signal1>(weak_ptr<my_emitter_type>(e));
			{
				shared_ptr<synapse::blocker> b1=synapse::block<signal1>(e);
				shared_ptr<synapse::blocker> b2=synapse::block<signal1>(e);
				BOOST_TEST_EQ(eb, b1);
				BOOST_TEST_EQ(eb, b2);
			}
			BOOST_TEST_EQ(the_meta_data<signal1>().e, e);
			BOOST_TEST_EQ(the_meta_data<signal1>().ep, e.get());
			BOOST_TEST_EQ(the_meta_data<signal1>().eb, eb.get());
			BOOST_TEST_EQ(the_meta_data<signal1>().count, 1);
			BOOST_TEST(emitter_blocked<signal1>(e.get()));
			the_meta_data<signal1>().reset();
			e.reset();
			BOOST_TEST_EQ(the_meta_data<signal1>().eb, nullptr);
			BOOST_TEST_EQ(the_meta_data<signal1>().count, 1);
			the_meta_data<signal1>().reset();
			{
				synapse::blocker * p=eb.get();
				eb.reset();
				BOOST_TEST(!the_meta_data<signal1>().e);
				BOOST_TEST(!the_meta_data<signal1>().ep);
				BOOST_TEST_EQ(the_meta_data<signal1>().eb, p);
			}
			BOOST_TEST_EQ(the_meta_data<signal1>().count, 0);
			the_meta_data<signal1>().count=0;
			the_meta_data<signal1>().reset();
		}
	}

	struct null_deleter { template <class T> void operator()( T * ) { } };

	void emitter_address_reuse_test()
	{
		my_emitter_type e2;
		shared_ptr<my_emitter_type> e1(&e2,null_deleter());
		shared_ptr<synapse::blocker> b1=synapse::block<signal1>(e1);
		BOOST_TEST(emitter_blocked<signal1>(&e2));
		BOOST_TEST_EQ(b1.use_count(), 1);
		e1.reset();
		the_meta_data<signal1>().reset();
		shared_ptr<synapse::blocker> b2=synapse::block<signal1>(&e2);
		BOOST_TEST(emitter_blocked<signal1>(&e2));
		BOOST_TEST_EQ(b2.use_count(), 1);
		the_meta_data<signal1>().reset();
		b1.reset();
		BOOST_TEST(emitter_blocked<signal1>(&e2));
		the_meta_data<signal1>().reset();
		b2.reset();
		BOOST_TEST(!emitter_blocked<signal1>(&e2));
	}

}

int main( int argc, char const * argv[] )
{
	block_test();
	emitter_address_reuse_test();
	return boost::report_errors();
}
