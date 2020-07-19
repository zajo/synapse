#ifndef BOOST_SYNAPSE_TEST_BARRIER_HPP
#define BOOST_SYNAPSE_TEST_BARRIER_HPP

// Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <mutex>
#include <condition_variable>
#include <cassert>

class barrier
{
	barrier( barrier const & ) = delete;
	barrier & operator=( barrier const & ) = delete;

	std::mutex mut_;
	std::condition_variable cond_;
	int const threshold_;
	int count_;
	int generation_;

public:

	explicit barrier( int count ):
	  threshold_(count),
	  count_(count),
	  generation_(0)
	{
		assert(count_ > 0);
	}

	void wait()
	{
		std::unique_lock<std::mutex> lk{mut_};
		auto g = generation_;
		if( --count_ )
			cond_.wait(lk, [this, g] { return g != generation_; });
		else
		{
			++generation_;
			count_ = threshold_;
			cond_.notify_all();
		}
	}
};

#endif
