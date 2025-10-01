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

#define LIST_BOX_WINDOW_FIND_FILES_STATUS_MESSAGE_FORMAT_STRING					"%s (%s) - %d files"
#define LIST_BOX_WINDOW_SAVED_FILE_STATUS_MESSAGE_FORMAT_STRING					"Saved to %s"

#define LIST_BOX_WINDOW_UNABLE_TO_SAVE_WARNING_MESSAGE							"Unable to save.\r\n\r\nDo you want to close?"

BOOL IsListBoxWindow( HWND hWnd );

int ListBoxWindowAddString( LPCTSTR lpszString );

BOOL ListBoxWindowCreate( HWND hWndParent, HINSTANCE hInstance );

int ListBoxWindowFindFiles( LPCTSTR lpszFolderPath, LPCTSTR lpszFileFilter = ALL_FILES_FILTER );

BOOL ListBoxWindowGetRect( LPRECT lpRect );

BOOL ListBoxWindowHandleCommandMessage( WPARAM wParam, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) );

BOOL ListBoxWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint = TRUE );

int ListBoxWindowLoad( LPCTSTR lpszFileName );

int ListBoxWindowPopulate( LPCTSTR lpszFileName );

int ListBoxWindowSave( HWND hWndParent, LPTSTR lpszFileName );

int ListBoxWindowSave( LPCTSTR lpszFileName );

HWND ListBoxWindowSetFocus();

void ListBoxWindowSetFont( HFONT hFont );
