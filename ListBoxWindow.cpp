// ListBoxWindow.cpp

#include "ListBoxWindow.h"

// Global variables
static HWND g_hWndListBox;

BOOL IsListBoxWindow( HWND hWnd )
{
	// See if supplied window is list box window
	return( hWnd == g_hWndListBox );

} // End of function IsListBoxWindow

int ListBoxWindowAddString( LPCTSTR lpszString )
{
	// Add string to list box window
	return SendMessage( g_hWndListBox, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszString );

} // End of function ListBoxWindowAddString

BOOL ListBoxWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list box window
	g_hWndListBox = CreateWindowEx( LIST_BOX_WINDOW_EXTENDED_STYLE, LIST_BOX_WINDOW_CLASS_NAME, LIST_BOX_WINDOW_TEXT, LIST_BOX_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list box window was created
	if( g_hWndListBox )
	{
		// Successfully created list box window

		// Update return value
		bResult = TRUE;

	} // End of successfully created list box window
	return bResult;

} // End of function ListBoxWindowCreate

BOOL ListBoxWindowGetRect( LPRECT lpRect )
{
	// Get list box window rect
	return GetWindowRect( g_hWndListBox, lpRect );

} // End of function ListBoxWindowGetRect

BOOL ListBoxWindowHandleCommandMessage( WPARAM wParam, LPARAM, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	// Select list box window notification code
	switch( HIWORD( wParam ) )
	{
		case LBN_DBLCLK:
		{
			// A list box window double click notification code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelected = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get selected item
			nSelectedItem = SendMessage( g_hWndListBox, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Get selected item text
			if( SendMessage( g_hWndListBox, LB_GETTEXT, ( WPARAM )nSelectedItem, ( LPARAM )lpszSelected ) )
			{
				// Successfully got selected item text

				// Display selected item text
				MessageBox( NULL, lpszSelected, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelected;

			// Break out of switch
			break;

		} // End of a list box window double click notification code
		case LBN_SELCHANGE:
		{
			// A list box window selection change notification code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelected = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get selected item
			nSelectedItem = SendMessage( g_hWndListBox, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

			// Get selected item text
			if( SendMessage( g_hWndListBox, LB_GETTEXT, ( WPARAM )nSelectedItem, ( LPARAM )lpszSelected ) )
			{
				// Successfully got selected item text

				// Show selected item text on status bar window
				( *lpStatusFunction )( lpszSelected );

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelected;

			// Break out of switch
			break;

		} // End of a list box window selection change notification code
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for list box window notification code

	return bResult;

} // End of function ListBoxWindowHandleCommandMessage

BOOL ListBoxWindowHandleNotifyMessage( WPARAM, LPARAM lParam, BOOL( *lpStatusFunction )( LPCTSTR lpszItemText ) )
{
	BOOL bResult = FALSE;

	LPNMHDR lpNmhdr;

	// Get notify message handler
	lpNmhdr = ( ( LPNMHDR )lParam );

	// Select list box window notification code
	switch( lpNmhdr->code )
	{
		default:
		{
			// Default notification code

			// No need to do anything here, just continue with default result

			// Break out of switch
			break;

		} // End of default notification code

	}; // End of selection for list box window notification code

	return bResult;

} // End of function ListBoxWindowHandleNotifyMessage

BOOL ListBoxWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list box window
	return MoveWindow( g_hWndListBox, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListBoxWindowMove

int ListBoxWindowLoad( LPCTSTR lpszFileName )
{
	int nResult = 0;

	HANDLE hFile;

	// Open file
	hFile = CreateFile( lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );

	// Ensure that file was opened
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully opened file
		DWORD dwFileSize;

		// Get file size
		dwFileSize = GetFileSize( hFile, NULL );

		// Ensure that file size was got
		if( dwFileSize != INVALID_FILE_SIZE )
		{
			// Successfully got file size

			// Allocate string memory
			LPTSTR lpszFileText = new char[ dwFileSize + sizeof( char ) ];

			// Read file text
			if( ReadFile( hFile, lpszFileText, dwFileSize, NULL, NULL ) )
			{
				// Successfully read file text
				LPTSTR lpszLine;

				// Terminate file text
				lpszFileText[ dwFileSize ] = ( char )NULL;

				// Get first line in file text
				lpszLine = strtok( lpszFileText, NEW_LINE_TEXT );

				// Loop through all lines in file text
				while( lpszLine )
				{
					// Add line to list box window
					if( SendMessage( g_hWndListBox, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszLine ) >= 0 )
					{
						// Successfully added line to list box window

						// Update return value
						nResult ++;

						// Get next line in file text
						lpszLine = strtok( NULL, NEW_LINE_TEXT );

					} // End of successfully added line to list box window
					else
					{
						// Unable to add line to list box window

						// Force exit from loop
						lpszLine = NULL;

					} // End of unable to add line to list box window

				}; // End of loop through all lines in file text

			} // End of successfully read file text

			// Free string memory
			delete [] lpszFileText;

		} // End of successfully got file size

		// Close file
		CloseHandle( hFile );

	} // End of successfully opened file

	return nResult;

} // End of function ListBoxWindowLoad

int ListBoxWindowPopulate( LPCTSTR lpszFileName )
{
	int nResult = 0;

	// Clear list box window
	SendMessage( g_hWndListBox, LB_RESETCONTENT, ( WPARAM )NULL, ( LPARAM )NULL );

	// Load file
	nResult = ListBoxWindowLoad( lpszFileName );

	return nResult;

} // End of function ListBoxWindowPopulate

int ListBoxWindowSave( LPCTSTR lpszFileName )
{
	int nResult = 0;

	HANDLE hFile;

	// Create file
	hFile = CreateFile( lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	// Ensure that file was created
	if( hFile != INVALID_HANDLE_VALUE )
	{
		// Successfully created file
		int nItemCount;
		int nWhichItem;

		// Allocate string memory
		LPTSTR lpszItemText = new char[ STRING_LENGTH + sizeof( char ) ];

		// Count items on list box window
		nItemCount = SendMessage( g_hWndListBox, LB_GETCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Loop through items on list box window
		for( nWhichItem = 0; nWhichItem < nItemCount; nWhichItem ++ )
		{
			// Get item text
			if( SendMessage( g_hWndListBox, LB_GETTEXT, ( WPARAM )nWhichItem, ( LPARAM )lpszItemText) != LB_ERR )
			{
				// Successfully got item text

				// Write item text to file
				if( WriteFile( hFile, lpszItemText, lstrlen( lpszItemText ), NULL, NULL ) )
				{
					// Successfully wrote item text to file

					// Write new line text to file
					WriteFile( hFile, NEW_LINE_TEXT, lstrlen( NEW_LINE_TEXT ), NULL, NULL );

					// Update return value
					nResult ++;

				} // End of successfully wrote item text to file
				else
				{
					// Unable to write item text to file

					// Force exit from loop
					nWhichItem = nItemCount;

				} // End of unable to write item text to file

			} // End of successfully got item text
			else
			{
				// Unable to get item text

				// Force exit from loop
				nWhichItem = nItemCount;

			} // End of unable to get item text

		}; // End of loop through items on list box window

		// Free string memory
		delete [] lpszItemText;

		// Close file
		CloseHandle( hFile );

	} // End of successfully created file

	return nResult;

} // End of function ListBoxWindowSave

HWND ListBoxWindowSetFocus()
{
	// Focus on list box window
	return SetFocus( g_hWndListBox );

} // End of function ListBoxWindowSetFocus

void ListBoxWindowSetFont( HFONT hFont )
{
	// Set list box window font
	SendMessage( g_hWndListBox, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListBoxWindowSetFont
