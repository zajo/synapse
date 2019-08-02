project('synapse', 'cpp', license : 'boost')

boost_headers = subproject('boost').get_variable('headers')

includes = include_directories('include')

defines  = [ '-DBOOST_SYNAPSE_USE_STD_FUNCTION', '-DBOOST_SYNAPSE_USE_STD_SMART_PTR' ]

emit = declare_dependency( include_directories: includes, compile_args: defines )

thread_local_queue = declare_dependency( include_directories: includes, compile_args: defines, sources:
	'src/thread_local_queue.cpp',
	dependencies: [
		boost_headers ] )

connect = declare_dependency( include_directories: includes, compile_args: defines, sources:
	'src/connect.cpp',
	dependencies: [
		thread_local_queue ] )

block = declare_dependency( include_directories: includes, compile_args: defines, sources:
	'src/block.cpp',
	dependencies: [
		boost_headers ] )

all = declare_dependency( include_directories: includes, compile_args: defines,
	link_with: static_library(
		'synapse',
		dependencies: [
			connect,
			block,
			thread_local_queue ] ) )
