//Copyright (c) 2015 Emil Dotchevski and Reverge Studios, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//This program demonstrates how Boost Synapse can be used to dispatch WindowProc
//messages using the HWND object as the emitter argument to synapse::emit. It also
//shows how boost::shared_ptr can be used to control the lifetime of Windows objects.

#include <boost/synapse/connect.hpp>
#include <boost/bind.hpp>
#define NOMINMAX
#include <windows.h>
#include <sstream>

namespace synapse=boost::synapse;

namespace
    {
    //Define a mouse_move signal that takes the mouse cursor coordinates.
    typedef struct mouse_move_(*mouse_move)( int x, int y );

    //Windows handles are pointer typedefs. The code below extracts the pointer type
    //from the typedef so that it can be used with shared_ptr. This technique works for
    //all Windows handle types.
    template <class T> struct h_type;
    template <class T> struct h_type<T *> { typedef T type; };
    typedef h_type<HWND>::type hwnd_t;

    //This is our WindowProc registered with Windows. Its translates Windows messages
    //into calls to synapse::emit<>, passing the hWnd as the emitter object.
    LRESULT CALLBACK
    WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
        {
        switch( uMsg )
            {
            default:
                return DefWindowProc(hWnd,uMsg,wParam,lParam);
            case WM_MOUSEMOVE:
                synapse::emit<mouse_move>(hWnd,lParam&0xFFFF,(lParam>>16)&0xFFFF);
                break;
            case WM_CLOSE:
                PostQuitMessage(0);
                break;
            }
        return 0;
        }

    //This function is connected to the mouse_move signal emitted by WindowProc.
    void
    print_mouse_position( HWND hWnd, int x, int y )
        {
        if( HDC dc=GetDC(hWnd) )
            {
            std::ostringstream str; str << "Mouse position: " << x << ", " << y;
            std::string s=str.str();
            RECT rect; GetClientRect(hWnd,&rect);
            FillRect(dc,&rect,(HBRUSH) GetStockObject(WHITE_BRUSH));
            TextOutA(dc,10,10,s.c_str(),s.size());
            ReleaseDC(hWnd,dc);
            }
        }
    }

int CALLBACK
WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int )
    {
    //Register the Windows class.
    char const name[ ]="win_WindowProc";
    WNDCLASSEXA wcx; 
    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wcx.lpfnWndProc = &WindowProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = hInstance;
    wcx.hIcon = 0;
    wcx.hCursor=LoadCursor(NULL,IDC_ARROW);
    wcx.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wcx.lpszMenuName =  0; 
    wcx.lpszClassName = name;
    wcx.hIconSm = 0;
    (void) RegisterClassExA(&wcx);

    //Create a window and hold on to it by shared_ptr.
    boost::shared_ptr<hwnd_t> hWnd;
    if( HWND w=CreateWindowA(name,name,WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_SIZEBOX,200,200,400,200,0,0,hInstance,0) )
        boost::shared_ptr<hwnd_t>(w,&DestroyWindow).swap(hWnd);
    else
        return 1; //Error

    //Connect the mouse_move signal from the hWnd emitter to the print_mouse_position function.
    boost::shared_ptr<synapse::connection> conn=synapse::connect<mouse_move>(hWnd,boost::bind(&print_mouse_position,hWnd.get(),_1,_2));

    //Show the window and call print_mouse_position once so it's not empty to begin with.
    ShowWindow(hWnd.get(),SW_SHOW);
    print_mouse_position(hWnd.get(),0,0);

    //Process messages -- this continues until the user closes the window.
    for( MSG msg; GetMessageA(&msg,0,0,0); )
        {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        }
    return 0;
    }
