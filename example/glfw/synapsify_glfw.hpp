//Copyright (c) 2015-2017 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//This header defines synapse signaals for use with GLFW.

#ifndef UUID_AE227DEA750611E5993CF1961D5D46B0
#define UUID_AE227DEA750611E5993CF1961D5D46B0

extern "C" { typedef struct GLFWwindow GLFWwindow; }

namespace glfw_signals
{
	typedef struct exception_caught_(*exception_caught)( GLFWwindow * ); //reports exceptions from connected functions.
	typedef struct framebuffer_size_(*framebuffer_size)( GLFWwindow *, int width, int height );
	typedef struct key_ (*key)( GLFWwindow *, int key, int scancode, int action, int mods );
	typedef struct text_(*text)( GLFWwindow *, unsigned int codepoint );
	typedef struct cursor_position_ (*cursor_position)( GLFWwindow *, double xpos, double ypos );
	typedef struct cursor_enter_(*cursor_enter)( GLFWwindow *, int entered );
	typedef struct mouse_button_(*mouse_button)( GLFWwindow *, int button, int action, int mods );
	typedef struct scroll_(*scroll)( GLFWwindow *, double xoffset, double yoffset );
	typedef struct path_drop_(*path_drop)( GLFWwindow *, int count, char const * * paths );
}

void synapsify_glfw();

#endif
