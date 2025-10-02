// SplashScreenWindow.h

#pragma once

#include <windows.h>

#include "Ascii.h"
#include "Common.h"

#define SPLASH_SCREEN_WINDOW_CLASS_NAME											"Main Splash Screen Window Class"

#define SPLASH_SCREEN_WINDOW_CLASS_STYLE										0
#define SPLASH_SCREEN_WINDOW_CLASS_BACKGROUND									( HBRUSH )( COLOR_WINDOW + 1 )
#define SPLASH_SCREEN_WINDOW_CLASS_TEXT_COLOR									COLOR_WINDOWTEXT

#define SPLASH_SCREEN_WINDOW_EXTENDED_STYLE										( WS_EX_TOOLWINDOW | WS_EX_TOPMOST )
#define SPLASH_SCREEN_WINDOW_STYLE												( WS_POPUP | WS_BORDER )
#define SPLASH_SCREEN_WINDOW_TEXT												"Splash Screen"

#define SPLASH_SCREEN_WINDOW_INITIAL_WIDTH										1
#define SPLASH_SCREEN_WINDOW_INITIAL_HEIGHT										1
#define SPLASH_SCREEN_WINDOW_BORDER_SIZE										10

#define SPLASH_SCREEN_WINDOW_DEFAULT_MILLISTCONDS								2000

BOOL SplashScreenWindowShow( HINSTANCE hInstance, LPCTSTR lpszText, DWORD dwMilliSeconds  = SPLASH_SCREEN_WINDOW_DEFAULT_MILLISTCONDS );
