//Copyright (c) Reverge Studios, Inc.
//ALL RIGHTS RESERVED

#include "synapsify_glfw.hpp"
#include "boost/synapse/connect.hpp"
#include "boost/synapse/connection.hpp"
#include "GLFW/glfw3.h"

namespace synapse=boost::synapse;

namespace
{
    void (*prev_framebuffer_size_)( GLFWwindow * w, int width, int height );
    void framebuffer_size_fwd( GLFWwindow * w, int width, int height )
    {
        try
        {
            synapse::emit<glfw_signals::framebuffer_size>(w,w,width,height);
        }
        catch(...)
        {
            synapse::emit<glfw_signals::exception_caught>(w,w);
        }
        if( prev_framebuffer_size_ )
            prev_framebuffer_size_(w,width,height);
    }

    void (*prev_key_)( GLFWwindow * w, int key, int scancode, int action, int mods );
    void key_fwd( GLFWwindow * w, int key, int scancode, int action, int mods )
    {
        try
        {
            synapse::emit<glfw_signals::key>(w,w,key,scancode,action,mods);
        }
        catch(...)
        {
            synapse::emit<glfw_signals::exception_caught>(w,w);
        }
        if( prev_key_ )
            prev_key_(w,key,scancode,action,mods);
        }

    void (*prev_text_)( GLFWwindow * w, unsigned int codepoint );
    void text_fwd( GLFWwindow * w, unsigned int codepoint )
    {
        try
        {
            synapse::emit<glfw_signals::text>(w,w,codepoint);
        }
        catch(...)
        {
            synapse::emit<glfw_signals::exception_caught>(w,w);
        }
        if( prev_text_ )
            prev_text_(w,codepoint);
    }

    void (*prev_cursor_position_)( GLFWwindow * w, double xpos, double ypos );
    void cursor_position_fwd( GLFWwindow * w, double xpos, double ypos )
    {
        try
        {
            synapse::emit<glfw_signals::cursor_position>(w,w,xpos,ypos);
        }
        catch(...)
        {
            synapse::emit<glfw_signals::exception_caught>(w,w);
        }
        if( prev_cursor_position_ )
            prev_cursor_position_(w,xpos,ypos);
    }

    void (*prev_cursor_enter_)( GLFWwindow * w, int entered );
    void cursor_enter_fwd( GLFWwindow * w, int entered )
    {
        try
        {
            synapse::emit<glfw_signals::cursor_enter>(w,w,entered);
        }
        catch(...)
        {
            synapse::emit<glfw_signals::exception_caught>(w,w);
        }
        if( prev_cursor_enter_ )
            prev_cursor_enter_(w,entered);
    }

    void (*prev_mouse_button_)( GLFWwindow * w, int button, int action, int mods );
    void mouse_button_fwd( GLFWwindow * w, int button, int action, int mods )
    {
        try
        {
            synapse::emit<glfw_signals::mouse_button>(w,w,button,action,mods);
        }
        catch(...)
        {
            synapse::emit<glfw_signals::exception_caught>(w,w);
        }
        if( prev_mouse_button_ )
            prev_mouse_button_(w,button,action,mods);
    }

    void (*prev_scroll_)( GLFWwindow * w, double xoffset, double yoffset );
    void scroll_fwd( GLFWwindow * w, double xoffset, double yoffset )
    {
        try
        {
            synapse::emit<glfw_signals::scroll>(w,w,xoffset,yoffset);
        }
        catch(...)
        {
            synapse::   emit<glfw_signals::exception_caught>(w,w);
        }
        if( prev_scroll_ )
            prev_scroll_(w,xoffset,yoffset);
    }

    void (*prev_path_drop_)( GLFWwindow * w, int count, char const * * paths );
    void path_drop_fwd( GLFWwindow * w, int count, char const * * paths )
    {
        try
        {
            synapse::emit<glfw_signals::path_drop>(w,w,count,paths);
        }
        catch(...)
        {
            synapse::emit<glfw_signals::exception_caught>(w,w);
        }
        if( prev_path_drop_ )
            prev_path_drop_(w,count,paths);
    }

    template <class F, F * Handler, F * * PrevHandler, F * (*Setter)(GLFWwindow *,F *)>
    void meta_handler( synapse::connection & c, unsigned flags )
    {
        boost::shared_ptr<GLFWwindow> w=c.emitter<GLFWwindow>();
        if( flags&synapse::meta::connect_flags::connecting )
        {
            if( flags&synapse::meta::connect_flags::first_for_this_emitter )
            {
                assert(!*PrevHandler);
                *PrevHandler=Setter(w.get(),Handler);
            }
        }
        else
        {
            if( flags&synapse::meta::connect_flags::last_for_this_emitter )
            {
                F * p=Setter(w.get(),*PrevHandler);
                assert(p==Handler);
            }
        }
    }
}

void synapsify_glfw()
{
    synapse::connect<synapse::meta::connected<glfw_signals::framebuffer_size> >( synapse::meta::emitter(),
        &meta_handler<void(GLFWwindow *,int,int),&framebuffer_size_fwd,&prev_framebuffer_size_,&glfwSetFramebufferSizeCallback> );

    synapse::connect<synapse::meta::connected<glfw_signals::key> >( synapse::meta::emitter(),
        &meta_handler<void(GLFWwindow *,int,int,int,int),&key_fwd,&prev_key_,&glfwSetKeyCallback> );

    synapse::connect<synapse::meta::connected<glfw_signals::text> >( synapse::meta::emitter(),
        &meta_handler<void(GLFWwindow *,unsigned),&text_fwd,&prev_text_,&glfwSetCharCallback> );

    synapse::connect<synapse::meta::connected<glfw_signals::cursor_position> >( synapse::meta::emitter(),
        &meta_handler<void(GLFWwindow *,double,double),&cursor_position_fwd,&prev_cursor_position_,&glfwSetCursorPosCallback> );

    synapse::connect<synapse::meta::connected<glfw_signals::cursor_enter> >( synapse::meta::emitter(),
        &meta_handler<void(GLFWwindow *,int),&cursor_enter_fwd,&prev_cursor_enter_,&glfwSetCursorEnterCallback> );

    synapse::connect<synapse::meta::connected<glfw_signals::mouse_button> >( synapse::meta::emitter(),
        &meta_handler<void(GLFWwindow *,int,int,int),&mouse_button_fwd,&prev_mouse_button_,&glfwSetMouseButtonCallback> );

    synapse::connect<synapse::meta::connected<glfw_signals::scroll> >( synapse::meta::emitter(),
        &meta_handler<void(GLFWwindow *,double,double),&scroll_fwd,&prev_scroll_,&glfwSetScrollCallback> );

    synapse::connect<synapse::meta::connected<glfw_signals::path_drop> >( synapse::meta::emitter(),
        &meta_handler<void(GLFWwindow *,int,char const * *),&path_drop_fwd,&prev_path_drop_,&glfwSetDropCallback> );
}
