# Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

project('synapse', 'cpp', default_options : ['cpp_std=c++17', 'b_pch=false'], license : 'boost')

defines = [ ]
dep_boost = [ ]
if get_option('synapse_use_boost') # Requires that LEAF resides under boost_root/libs/leaf.
	dep_boost = declare_dependency(include_directories: '../..')
	defines = [ '-DBOOST_SYNAPSE_USE_BOOST' ]
endif

includes = include_directories('include')

emit = declare_dependency( include_directories: includes, compile_args: defines )

thread_local_queue = declare_dependency( include_directories: includes, compile_args: defines, sources:
	'src/thread_local_queue.cpp',
	dependencies: [
		dep_boost ] )

connect = declare_dependency( include_directories: includes, compile_args: defines, sources:
	'src/connect.cpp',
	dependencies: [
		thread_local_queue ] )

block = declare_dependency( include_directories: includes, compile_args: defines, sources:
	'src/block.cpp',
	dependencies: [
		dep_boost ] )

all = declare_dependency( include_directories: includes, compile_args: defines,
	link_with: static_library(
		'synapse',
		dependencies: [
			connect,
			block,
			thread_local_queue ] ) )


includes = [ include_directories('include') ]

leaf = declare_dependency( include_directories: includes, compile_args: defines )

#################################

tests = [
	compile_block_hpp,
	compile_blocker_hpp,
	compile_connect_hpp,
	compile_connection_hpp,
	compile_emit_hpp,
	compile_thread_local_queue_hpp,
	compile_translate_hpp,
	args_connect_test,
	args_translate_test,
	block_emit_test,
	block_test,
	connect_emit_test,
	connection_lifetime_test,
	connection_user_data_test,
	ct_basic_test,
	ct_lambda_test,
	ct_tlq_lifetime_test,
	ct_wait_test,
	emit_stub_test,
	emit_test,
	property_test,
	release_test,
	shutdown_test1,
	shutdown_test2,
	shutdown_test3,
	shutdown_test4,
	shutdown_test5,
	shutdown_test6,
	shutdown_test7,
	shutdown_test8,
	shutdown_test9,
	weak_store_test
]
foreach t : tests
	test(t, executable(t, 'test/'+t+'.cpp', dependencies: [all, dep_boost]) )
endforeach
