// Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// This header defines synapse signaals for use with GLFW.

#include "glfw_signals.hpp"
#include <boost/synapse/connect.hpp>
#include <boost/synapse/connection.hpp>
#include "GLFW/glfw3.h"

template <class Signal>
class synapsifier;

template <class R, class... A>
class synapsifier<R(*)(GLFWwindow *,A...)>
{
	typedef R(*Signal)(GLFWwindow *,A...);
	typedef void (*GLFWfun)( GLFWwindow *,A... );
	static GLFWfun prev_;

	// This is the handler that GLFW calls. It emits the corresponding Synapse
	// signal and calls the previous GLFW handler for the same event, if any.
	static void handler( GLFWwindow * w, A... a )
	{
		using namespace boost::synapse;
		try
		{
			(void) emit<Signal>(w,w,a...);
		}
		catch(...)
		{
			// We can't let exceptions propagate up into C code, so the window
			// emits the exception_caught signal, which (if exceptions are
			// expected) should be connected to capture and handle the current
			// exception.
			bool handled = emit<glfw_signals::exception_caught>(w,w)>0;
			assert(handled);
		}
		if( prev_ )
			prev_(w,a...);
	}

	public:

	explicit synapsifier( GLFWfun (*setter)(GLFWwindow *,GLFWfun) )
	{
		using namespace boost::synapse;
		// Here we connect the Synapse meta::connected<Signal> signal. This
		// signal is emitted by the meta::emitter() when the Signal is being
		// connected (the user calls synapse::connect<Signal>) or disconnected
		// (when the connection expires). The emitter pointer passed to connect
		// (which in this case is of type GLFWwindow) is stored in the
		// synapse::connection object passed to the lambda below, and can be
		// accessed by the connection::emitter member function template.
		connect<meta::connected<Signal> >( meta::emitter(),
			[setter]( connection & c, unsigned flags )
			{
				if( flags&meta::connect_flags::connecting )
				{
					// When the Signal is being connected for the first time,
					// use the GLFW API to install our handler.
					if( flags&meta::connect_flags::first_for_this_emitter )
						prev_=setter(c.emitter<GLFWwindow>().get(),&handler);
				}
				else
				{
					// When the last Signal connection expires, use the GLFW API
					// to uninstall our handler and restore the previous handler.
					if( flags&meta::connect_flags::last_for_this_emitter )
					{
						GLFWfun p=setter(c.emitter<GLFWwindow>().get(),prev_);
						assert(p==&handler);
					}
				}
			} );
	}
};

template <class R, class... A>
typename synapsifier<R(*)(GLFWwindow *,A...)>::GLFWfun synapsifier<R(*)(GLFWwindow *,A...)>::prev_;

// Install all the synapse::meta::connected<....> handlers
synapsifier<glfw_signals::WindowClose> s1(&glfwSetWindowCloseCallback);
synapsifier<glfw_signals::WindowSize> s2(&glfwSetWindowSizeCallback);
synapsifier<glfw_signals::FramebufferSize> s3(&glfwSetFramebufferSizeCallback);
synapsifier<glfw_signals::WindowPos> s4(&glfwSetWindowPosCallback);
synapsifier<glfw_signals::WindowIconify> s5(&glfwSetWindowIconifyCallback);
synapsifier<glfw_signals::WindowFocus> s6(&glfwSetWindowFocusCallback);
synapsifier<glfw_signals::WindowRefresh> s7(&glfwSetWindowRefreshCallback);
synapsifier<glfw_signals::Key> s8(&glfwSetKeyCallback);
synapsifier<glfw_signals::Char> s9(&glfwSetCharCallback);
synapsifier<glfw_signals::CharMods> s10(&glfwSetCharModsCallback);
synapsifier<glfw_signals::CursorPos> s11(&glfwSetCursorPosCallback);
synapsifier<glfw_signals::CursorEnter> s12(&glfwSetCursorEnterCallback);
synapsifier<glfw_signals::MouseButton> s13(&glfwSetMouseButtonCallback);
synapsifier<glfw_signals::Scroll> s14(&glfwSetScrollCallback);
synapsifier<glfw_signals::Drop> s15(&glfwSetDropCallback);
