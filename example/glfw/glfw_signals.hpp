// Copyright (c) 2015-2018 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// This header defines synapse signaals for use with GLFW.

#ifndef UUID_DF9B5300F10111E79DCEF6AC7105BEE2
#define UUID_DF9B5300F10111E79DCEF6AC7105BEE2

extern "C" { typedef struct GLFWwindow GLFWwindow; }

namespace glfw_signals
{
	// User input callbacks
	typedef struct Key_(*Key)( GLFWwindow *, int key, int scancode, int action, int mods );
	typedef struct Char_(*Char)( GLFWwindow *, unsigned int codepoint );
	typedef struct CharMods_(*CharMods)( GLFWwindow *, unsigned int codepoint, int mods );
	typedef struct CursorPos_ (*CursorPos)( GLFWwindow *, double xpos, double ypos );
	typedef struct CursorEnter_(*CursorEnter)( GLFWwindow *, int entered );
	typedef struct MouseButton_(*MouseButton)( GLFWwindow *, int button, int action, int mods );
	typedef struct Scroll_(*Scroll)( GLFWwindow *, double xoffset, double yoffset );
	typedef struct Drop_(*Drop)( GLFWwindow *, int count, char const * * paths );

	// Window state callbacks
	typedef struct WindowClose_(*WindowClose)( GLFWwindow * );
	typedef struct WindowSize_(*WindowSize)( GLFWwindow *, int width, int height );
	typedef struct FramebufferSize_(*FramebufferSize)( GLFWwindow *, int width, int height );
	typedef struct WindowPos_(*WindowPos)( GLFWwindow *, int xpos, int ypos );
	typedef struct WindowIconify_(*WindowIconify)( GLFWwindow *, int iconified );
	typedef struct WindowFocus_(*WindowFocus)( GLFWwindow *, int focused );
	typedef struct WindowRefresh_(*WindowRefresh)( GLFWwindow * );

	// This is emitted from the GLFWwindow object to report exceptions from connected signal handlers
	typedef struct exception_caught_(*exception_caught)( GLFWwindow * );
}

#endif
