// StatusBarWindow.cpp

#include "StatusBarWindow.h"

// Global variables
HWND g_hWndStatusBar;

BOOL StatusBarWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create status bar window
	g_hWndStatusBar = ::CreateWindowEx( STATUS_BAR_WINDOW_EXTENDED_STYLE, STATUS_BAR_WINDOW_CLASS_NAME, STATUS_BAR_WINDOW_TEXT, STATUS_BAR_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that status bar window was created
	if( g_hWndStatusBar )
	{
		// Successfully created status bar window

		// Update return value
		bResult = TRUE;

	} // End of successfully created status bar window
	return bResult;

} // End of function StatusBarWindowCreate

BOOL StatusBarWindowGetRect( LPRECT lpRect )
{
	// Get status bar window rect
	return ::GetWindowRect( g_hWndStatusBar, lpRect );

} // End of function StatusBarWindowGetRect

void StatusBarWindowSetFont( HFONT hFont )
{
	// Set status bar window font
	::SendMessage( g_hWndStatusBar, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function StatusBarWindowSetFont

BOOL StatusBarWindowSetText( LPCTSTR lpszStatusText )
{
	// Set status bar window text
	return ::SendMessage( g_hWndStatusBar, SB_SETTEXT, ( WPARAM )NULL, ( LPARAM )lpszStatusText );

} // End of function StatusBarWindowSetText

LRESULT StatusBarWindowSize()
{
	// Size status bar window
	return ::SendMessage( g_hWndStatusBar, WM_SIZE, ( WPARAM )NULL, ( LPARAM )NULL );

} // End of function StatusBarWindowSize
