// ListBoxWindow.h

#pragma once

#include <windows.h>
#include <commctrl.h>

#include "Ascii.h"
#include "Common.h"

#define LIST_BOX_WINDOW_CLASS_NAME												WC_LISTBOX

#define LIST_BOX_WINDOW_EXTENDED_STYLE											0
#define LIST_BOX_WINDOW_STYLE													( WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY )
#define LIST_BOX_WINDOW_TEXT													NULL

#define LIST_BOX_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING					"%s (%d files)"

BOOL IsListBoxWindow( HWND hWnd );

int ListBoxWindowAddString( LPCTSTR lpszString );

BOOL ListBoxWindowCreate( HWND hWndParent, HINSTANCE hInstance );

int ListBoxWindowGetCurrentSelection();

int ListBoxWindowGetItemText( int nWhichItem, LPTSTR lpszItemText );

BOOL ListBoxWindowGetRect( LPRECT lpRect );

BOOL ListBoxWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) );

BOOL ListBoxWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int ListBoxWindowPopulate( LPCTSTR lpszFolderPath, LPCTSTR lpszFileFilter, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusMessage ) );

HWND ListBoxWindowSetFocus();

void ListBoxWindowSetFont( HFONT hFont );

