# Copyright (c) 2015-2020 Emil Dotchevski and Reverge Studios, Inc.

# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

project('synapse', 'cpp', default_options : ['cpp_std=c++17', 'b_pch=false'], license : 'boost')

compiler = meson.get_compiler('cpp')
compiler_id = compiler.get_id()
if not meson.is_subproject()
	if compiler_id=='clang'
		if get_option('buildtype')!='debug'
			add_global_arguments(
				'-Wno-unused-variable',
				language:'cpp' )
		endif
		add_global_arguments(
			'-Wno-delete-non-abstract-non-virtual-dtor',
			'-Wno-non-virtual-dtor',
			'-Wno-dangling-else',
			language:'cpp' )
	elif compiler_id=='gcc'
		if get_option('buildtype')!='debug'
			add_global_arguments(
				'-Wno-unused-variable',
				'-Wno-unused-but-set-variable',
				language:'cpp' )
		endif
		add_global_arguments(
			'-Wno-non-virtual-dtor',
			'-Wno-dangling-else',
			'-Wno-sign-compare',
			language:'cpp' )
	elif compiler_id=='msvc'
		add_global_arguments(
			language:'cpp' )
	endif
endif

dep_threads = dependency('threads')

defines = [ ]
dep_boost = [ ]
if get_option('synapse_use_boost') # Requires that Synapse resides under boost_root/libs/synapse.
	dep_boost = declare_dependency(include_directories: '../..')
	defines = [ '-DBOOST_SYNAPSE_USE_BOOST' ]
endif
if not get_option('synapse_suppress_warnings')
	defines = [ '-DBOOST_SYNAPSE_ENABLE_WARNINGS' ]
endif

includes = include_directories('include')

emit = declare_dependency( include_directories: includes, compile_args: defines )

thread_local_queue = declare_dependency( include_directories: includes, compile_args: defines, sources:
	'src/thread_local_queue.cpp',
	dependencies: [ dep_boost ] )

connect = declare_dependency( include_directories: includes, compile_args: defines, sources:
	'src/connect.cpp',
	dependencies: [ dep_boost, thread_local_queue ] )

block = declare_dependency( include_directories: includes, compile_args: defines, sources:
	'src/block.cpp',
	dependencies: [ dep_boost ] )

all = declare_dependency( include_directories: includes, compile_args: defines,
	link_with: static_library(
		'synapse',
		dependencies: [
			connect,
			block,
			thread_local_queue ] ) )


includes = [ include_directories('include') ]

#################################

tests = [
	[ '_hpp_compile_block_test', [ emit, dep_boost ] ],
	[ '_hpp_compile_blocker_test', [ emit, dep_boost ] ],
	[ '_hpp_compile_connect_test', [ emit, dep_boost ] ],
	[ '_hpp_compile_connection_test', [ emit, dep_boost ] ],
	[ '_hpp_compile_emit_test', [ emit, dep_boost ] ],
	[ '_hpp_compile_thread_local_queue_test', [ emit, dep_boost ] ],
	[ '_hpp_compile_translate_test', [ emit, dep_boost ] ],
	[ 'args_connect_test', [ connect ] ],
	[ 'args_translate_test', [ connect ] ],
	[ 'block_emit_test', [ connect, block ] ],
	[ 'block_test', [ connect, block ] ],
	[ 'connect_emit_test', [ connect ] ],
	[ 'connection_lifetime_test', [ connect ] ],
	[ 'connection_user_data_test', [ connect ] ],
	[ 'ct_basic_test', [ connect, dep_threads ] ],
	[ 'ct_lambda_test', [ connect, dep_threads ] ],
	[ 'ct_tlq_lifetime_test', [ connect, dep_threads ] ],
	[ 'ct_wait_test', [ connect, dep_threads ] ],
	[ 'emit_stub_test', [ emit, dep_boost ] ],
	[ 'emit_test', [ connect ] ],
	[ 'property_test', [ connect ] ],
	[ 'release_test', [ connect ] ],
	[ 'shutdown_test1', [ connect ] ],
	[ 'shutdown_test2', [ connect ] ],
	[ 'shutdown_test3', [ connect ] ],
	[ 'shutdown_test4', [ connect ] ],
	[ 'shutdown_test5', [ connect ] ],
	[ 'shutdown_test6', [ connect ] ],
	[ 'shutdown_test7', [ connect ] ],
	[ 'shutdown_test8', [ connect ] ],
	[ 'shutdown_test9', [ connect ] ],
	[ 'weak_store_test', [ connect ] ]
]
foreach t : tests
	test(t[0], executable(t[0], 'test/'+t[0]+'.cpp', dependencies: t[1]) )
endforeach

if get_option('synapse_glfw_example')
	executable('glfw_example', 'example/glfw/simple.cpp', dependencies: [ connect, dependency('glfw3'), dependency('gl') ] )
endif

if get_option('synapse_qt_example')
	executable('qt_example', 'example/qt/qt_custom_signals.cpp', dependencies: [ connect, dependency('qt5', modules:['Widgets']) ] )
endif
