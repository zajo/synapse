// Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// This header defines synapse signaals for use with GLFW.

#ifndef UUID_DF9B5300F10111E79DCEF6AC7105BEE2
#define UUID_DF9B5300F10111E79DCEF6AC7105BEE2

#include <boost/synapse/signal_traits.hpp>

struct GLFWwindow;

namespace synapse = boost::synapse;

namespace glfw_signals
{
	// User input callbacks
	struct Key: synapse::signal<void(GLFWwindow *, int, int, int, int)> {};
	struct Char: synapse::signal<void(GLFWwindow *, unsigned int)> {};
	struct CharMods: synapse::signal<void(GLFWwindow *, unsigned int, int)> {};
	struct CursorPos: synapse::signal<void(GLFWwindow *, double, double)> {};
	struct CursorEnter: synapse::signal<void(GLFWwindow *, int)> {};
	struct MouseButton: synapse::signal<void(GLFWwindow *, int, int, int)> {};
	struct Scroll: synapse::signal<void(GLFWwindow *, double, double)> {};
	struct Drop: synapse::signal<void(GLFWwindow *, int, char const * *)> {};

	// Window state callbacks
	struct WindowClose: synapse::signal<void(GLFWwindow *)> {};
	struct WindowSize: synapse::signal<void(GLFWwindow *, int, int)> {};
	struct FramebufferSize: synapse::signal<void(GLFWwindow *, int, int)> {};
	struct WindowPos: synapse::signal<void(GLFWwindow *, int, int)> {};
	struct WindowIconify: synapse::signal<void(GLFWwindow *, int)> {};
	struct WindowFocus: synapse::signal<void(GLFWwindow *, int)> {};
	struct WindowRefresh: synapse::signal<void(GLFWwindow *)> {};

	// This is emitted from the GLFWwindow object to report exceptions from connected signal handlers
	struct exception_caught: synapse::signal<void(GLFWwindow *)> {};
}

#endif
