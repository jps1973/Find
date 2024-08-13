// ListBoxWindow.cpp

#include "ListBoxWindow.h"

// Global variables
HWND g_hWndListBox;

BOOL IsListBoxWindow( HWND hWnd )
{
	// See if supplied window is list box window
	return( hWnd == g_hWndListBox );

} // End of function IsListBoxWindow

int ListBoxWindowAddString( LPCTSTR lpszString )
{
	// Add string to list box window
	return ::SendMessage( g_hWndListBox, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszString );

} // End of function ListBoxWindowAddString

BOOL ListBoxWindowCreate( HWND hWndParent, HINSTANCE hInstance )
{
	BOOL bResult = FALSE;

	// Create list box window
	g_hWndListBox = ::CreateWindowEx( LIST_BOX_WINDOW_EXTENDED_STYLE, LIST_BOX_WINDOW_CLASS_NAME, LIST_BOX_WINDOW_TEXT, LIST_BOX_WINDOW_STYLE, 0, 0, 0, 0, hWndParent, ( HMENU )NULL, hInstance, NULL );

	// Ensure that list box window was created
	if( g_hWndListBox )
	{
		// Successfully created list box window

		// Update return value
		bResult = TRUE;

	} // End of successfully created list box window
	return bResult;

} // End of function ListBoxWindowCreate

int ListBoxWindowGetCurrentSelection()
{
	// Get current selection item
	return ::SendMessage( g_hWndListBox, LB_GETCURSEL, ( WPARAM )NULL, ( LPARAM )NULL );

} // End of function ListBoxWindowGetCurrentSelection

BOOL ListBoxWindowGetInitialFileName( LPTSTR lpszInitialFileName )
{
	BOOL bResult = FALSE;

	// Allocate string memory
	LPTSTR lpszFolderPath = new char[ STRING_LENGTH ];

	// Get current folder path
	if( GetCurrentDirectory( STRING_LENGTH, lpszFolderPath ) )
	{
		// Succesfully got current folder path
		LPTSTR lpszLastBackSlash;
		SYSTEMTIME st;

		// Allocate string memory
		LPTSTR lpszFolderName = new char[ STRING_LENGTH ];

		// Remove any back-slash characters from end of folder path
		while( lpszFolderPath[ lstrlen( lpszFolderPath ) - sizeof( char ) ] == ASCII_BACK_SLASH_CHARACTER )
		{
			// Remove back-slash character from end of folder path
			lpszFolderPath[ lstrlen( lpszFolderPath ) - sizeof( char ) ] = ( char )NULL;

		}; // End of loop to remove any back-slash characters from end of folder path

		// Find last back-slash character in folder path
		lpszLastBackSlash = strrchr( lpszFolderPath, ASCII_BACK_SLASH_CHARACTER );

		// See if last back-slash character was found in folder path
		if( lpszLastBackSlash )
		{
			// Successfully found last back-slash character in folder path

			// Use text after last back-slash for folder name
			lstrcpy( lpszFolderName, ( lpszLastBackSlash + sizeof( char ) ) );

		} // End of successfully found last back-slash character in folder path
		else
		{
			// Unable to find last back-slash character in folder path

			// Format folder name (assume folder is a drive)
			wsprintf( lpszFolderName, LIST_BOX_WINDOW_DRIVE_FORMAT_STRING, lpszFolderPath[ 0 ] );

		} // End of unable to find last back-slash character in folder path

		// Get system time
		GetLocalTime( &st );

		// Format initial file name
		wsprintf( lpszInitialFileName, LIST_BOX_WINDOW_INITIAL_FILE_NAME_FORMAT_STRING, lpszFolderPath, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

		// Free string memory
		delete [] lpszFolderName;

	} // End of succesfully got current folder path

	// Free string memory
	delete [] lpszFolderPath;

	return bResult;

} // End of functionListBoxWindowGetInitialFileName

int ListBoxWindowGetItemText( int nWhichItem, LPTSTR lpszItemText )
{
	// Get item text
	return::SendMessage( g_hWndListBox, LB_GETTEXT, ( WPARAM )nWhichItem, ( LPARAM )lpszItemText );

} // End of function ListBoxWindowGetItemText

BOOL ListBoxWindowGetRect( LPRECT lpRect )
{
	// Get list box window rect
	return ::GetWindowRect( g_hWndListBox, lpRect );

} // End of function ListBoxWindowGetRect

BOOL ListBoxWindowHandleCommandMessage( WPARAM wParam, LPARAM, void( *lpDoubleClickFunction )( LPCTSTR lpszItemText ), void( *lpSelectionChangedFunction )( LPCTSTR lpszItemText ) )
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
			LPTSTR lpszSelectedItemText = new char[ STRING_LENGTH ];

			// Get selected item
			nSelectedItem = ListBoxWindowGetCurrentSelection();

			// Get selected item text
			if( ListBoxWindowGetItemText( nSelectedItem, lpszSelectedItemText ) )
			{
				// Successfully got selected item text

				// Call double click function
				( *lpDoubleClickFunction )( lpszSelectedItemText );

				// Update return value
				bResult = TRUE;

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelectedItemText;

			// Break out of switch
			break;

		} // End of a list box window double click notification code
		case LBN_SELCHANGE:
		{
			// A list box window selection change notification code
			int nSelectedItem;

			// Allocate string memory
			LPTSTR lpszSelectedItemText = new char[ STRING_LENGTH ];

			// Get selected item
			nSelectedItem = ListBoxWindowGetCurrentSelection();

			// Get selected item text
			if( ListBoxWindowGetItemText( nSelectedItem, lpszSelectedItemText ) )
			{
				// Successfully got selected item text

				// Call selection changed function
				( *lpSelectionChangedFunction )( lpszSelectedItemText );

				// Update return value
				bResult = TRUE;

			} // End of successfully got selected item text

			// Free string memory
			delete [] lpszSelectedItemText;

			// Break out of switch
			break;

		} // End of a list box window selection change notification code

	}; // End of selection for list box window notification code

	return bResult;

} // End of function ListBoxWindowHandleCommandMessage

BOOL ListBoxWindowMove( int nX, int nY, int nWidth, int nHeight, BOOL bRepaint )
{
	// Move list box window
	return ::MoveWindow( g_hWndListBox, nX, nY, nWidth, nHeight, bRepaint );

} // End of function ListBoxWindowMove

int ListBoxWindowPopulate( LPCTSTR lpszFolderPath, LPCTSTR lpszFileFilter, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusMessage ) )
{
	int nResult = 0;

	WIN32_FIND_DATA wfd;
	HANDLE hFind;

	// Allocate string memory
	LPTSTR lpszParentFolderPath		= new char[ STRING_LENGTH ];
	LPTSTR lpszFullSearchPattern	= new char[ STRING_LENGTH ];

	// Store parent folder path
	lstrcpy( lpszParentFolderPath, lpszFolderPath );

	// Ensure that parent folder path ends with a back-slash
	if( lpszParentFolderPath[ lstrlen( lpszParentFolderPath ) - sizeof( char ) ] != ASCII_BACK_SLASH_CHARACTER )
	{
		// Parent folder path does not end with a back-slash

		// Append a back-slash onto parent folder path
		lstrcat( lpszParentFolderPath, ASCII_BACK_SLASH_STRING );

	} // End of parent folder path does not end with a back-slash

	// First, add files in the current folder

	// Copy parent folder path into full search pattern
	lstrcpy( lpszFullSearchPattern, lpszParentFolderPath );

	// Append file filter onto full search pattern
	lstrcat( lpszFullSearchPattern, lpszFileFilter );

	// Show full search pattern as status
	( *lpStatusFunction )( lpszParentFolderPath );

	// Find first item
	hFind = FindFirstFile( lpszFullSearchPattern, &wfd );

	// Ensure that first item was found
	if( hFind != INVALID_HANDLE_VALUE )
	{
		// Successfully found first item

		// Allocate string memory
		LPTSTR lpszFoundItemPath = new char[ STRING_LENGTH ];

		// Loop through all items
		do
		{
			// See if found item is a file
			if( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				// Found item is a file

				// Copy parent folder path into found item path
				lstrcpy( lpszFoundItemPath, lpszParentFolderPath );

				// Append found item name onto found item path
				lstrcat( lpszFoundItemPath, wfd.cFileName );

				// Add found item path onto list box window
				SendMessage( g_hWndListBox, LB_ADDSTRING, ( WPARAM )NULL, ( LPARAM )lpszFoundItemPath );

				// Update return value
				nResult ++;

			} // End of found item is a file

		} while( FindNextFile( hFind, &wfd )!=0 ); // End of loop through all items

		// Update list box window
		UpdateWindow( g_hWndListBox );

		// Close file find
		FindClose( hFind );

		// Free string memory
		delete [] lpszFoundItemPath;

	} // End of successfully found first item

	// Now that files in the currend folder have been added, scan for sub-folders

	// Copy parent folder path into full search pattern
	lstrcpy( lpszFullSearchPattern, lpszParentFolderPath );

	// Append all files filter onto full search pattern
	lstrcat( lpszFullSearchPattern, ALL_FILES_FILTER );

	// Find first item
	hFind = FindFirstFile( lpszFullSearchPattern, &wfd );

	// Ensure that first item was found
	if( hFind != INVALID_HANDLE_VALUE )
	{
		// Successfully found first item

		// Allocate string memory
		LPTSTR lpszFoundItemPath = new char[ STRING_LENGTH ];

		// Loop through all items
		do
		{
			// See if found item is a folder
			if( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				// Found item is a folder

				// Ensure that found item is not dots
				if( wfd.cFileName[ 0 ] != ASCII_FULL_STOP_CHARACTER )
				{
					// Found item is not dots

					// Copy parent folder path into found item path
					lstrcpy( lpszFoundItemPath, lpszParentFolderPath );

					// Append found item name onto found item path
					lstrcat( lpszFoundItemPath, wfd.cFileName );

					// Populate from the found folder
					nResult += ListBoxWindowPopulate( lpszFoundItemPath, lpszFileFilter, lpStatusFunction );

				} // End of found item is not dots

			} // End of found item is a folder

		} while( FindNextFile( hFind, &wfd )!=0 ); // End of loop through all items

		// Close file find
		FindClose( hFind );

		// Free string memory
		delete [] lpszFoundItemPath;

	} // End of successfully found first item

	return nResult;

} // End of function ListBoxWindowPopulate

int ListBoxWindowSave( BOOL( *lpStatusFunction )( LPCTSTR lpszStatusMessage ) )
{
	int nResult = 0;

	OPENFILENAME ofn;

	// Allocate string memory
	LPTSTR lpszFileName = new char[ STRING_LENGTH ];

	// Initialise file name
	ListBoxWindowGetInitialFileName( lpszFileName );

	// Clear open file name structure
	ZeroMemory( &ofn, sizeof( ofn ) );

	// Initialise open file name structure
	ofn.lStructSize		= sizeof( ofn );
	ofn.hwndOwner		= NULL;
	ofn.lpstrFilter		= SAVE_FILE_FILTER;
	ofn.lpstrFile		= lpszFileName;
	ofn.nMaxFile		= STRING_LENGTH;
	ofn.Flags			= ( OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT );
	ofn.lpstrDefExt		= SAVE_FILE_DEFAULT_EXTENSION;

	// Get save file name
	if( GetSaveFileName( &ofn ) )
	{
		// Successfully got save file name

		// Allocate string memory
		LPTSTR lpszStatusMessage = new char[ STRING_LENGTH ];

		// Save file
		nResult = ListBoxWindowSave( lpszFileName, lpStatusFunction );

		// Format status message
		wsprintf( lpszStatusMessage, LIST_BOX_WINDOW_SAVE_STATUS_MESSAGE_FORMAT_STRING, lpszFileName, nResult );

		// Show status message
		( *lpStatusFunction )( lpszStatusMessage );

		// Free string memory
		delete [] lpszStatusMessage;

	} // End of successfully got save file name
	
	// Free string memory
	delete [] lpszFileName;

	return nResult;

} // End of function ListBoxWindowSave

int ListBoxWindowSave( LPCTSTR lpszFileName, BOOL( *lpStatusFunction )( LPCTSTR lpszStatusMessage ) )
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
		LPTSTR lpszItemText = new char[ STRING_LENGTH ];

		// Count items on list box window
		nItemCount = SendMessage( g_hWndListBox, LB_GETCOUNT, ( WPARAM )NULL, ( LPARAM )NULL );

		// Loop through on list box window
		for( nWhichItem = 0; nWhichItem < nItemCount; nWhichItem ++ )
		{
			// Get item text
			if( SendMessage( g_hWndListBox, LB_GETTEXT, ( WPARAM )nWhichItem, ( LPARAM )lpszItemText ) != LB_ERR )
			{
				// Successfully got item text

				// Write item text to file
				if( WriteFile( hFile, lpszItemText, lstrlen( lpszItemText ), NULL, NULL ) )
				{
					// Successfully wrote item text to file

					// Use item text as status
					( *lpStatusFunction )( lpszItemText );

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

		}; // End of loop through on list box window

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
	return ::SetFocus( g_hWndListBox );

} // End of function ListBoxWindowSetFocus

void ListBoxWindowSetFont( HFONT hFont )
{
	// Set list box window font
	::SendMessage( g_hWndListBox, WM_SETFONT, ( WPARAM )hFont, ( LPARAM )TRUE );

} // End of function ListBoxWindowSetFont
