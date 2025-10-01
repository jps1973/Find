// Template.cpp

#include "Template.h"

int ShowAboutMessage( HWND hWndParent )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWndParent;
	mbp.hInstance	= NULL; // Note that this must be null to use standard icons
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= MAIN_WINDOW_CLASS_ICON_NAME;

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

LRESULT CALLBACK MainWindowProcedure( HWND hWndMain, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMsg )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;
			HFONT hFont;

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Get font
			hFont = ( HFONT )GetStockObject( DEFAULT_GUI_FONT );

			// Create list box window
			if( ListBoxWindowCreate( hWndMain, hInstance ) )
			{
				// Successfully created list box window

				// Set list box window font
				ListBoxWindowSetFont( hFont );

				// Create status bar window
				if( StatusBarWindowCreate( hWndMain, hInstance ) )
				{
					// Successfully created status bar window

					// Set status bar window font
					StatusBarWindowSetFont( hFont );

				} // End of successfully created status bar window

			} // End of successfully created list box window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatus;
			int nStatusWindowHeight;
			int nListBoxWindowHeight;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			StatusBarWindowSize();

			// Get status window size
			StatusBarWindowGetRect( &rcStatus );

			// Calculate window sizes
			nStatusWindowHeight		= ( rcStatus.bottom - rcStatus.top );
			nListBoxWindowHeight	= ( nClientHeight - nStatusWindowHeight );

			// Move list box window
			ListBoxWindowMove( 0, 0, nClientWidth, nListBoxWindowHeight, TRUE );

			// Break out of switch
			break;

		} // End of a size message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on list box window
			ListBoxWindowSetFocus();

			// Break out of switch
			break;

		} // End of an activate message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MINMAXINFO FAR *lpMinMaxInfo;

			// Get min max info structure
			lpMinMaxInfo = ( MINMAXINFO FAR * )lParam;

			// Update min max info structure
			lpMinMaxInfo->ptMinTrackSize.x = MAIN_WINDOW_MINIMUM_WIDTH;
			lpMinMaxInfo->ptMinTrackSize.y = MAIN_WINDOW_MINIMUM_HEIGHT;

			// Break out of switch
			break;

		} // End of a get min max info message
		case WM_DROPFILES:
		{
			// A drop files message
			UINT uFileCount;
			HDROP hDrop;
			UINT uWhichFile;
			UINT uFileSize;

			// Allocate string memory
			LPTSTR lpszFilePath = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get drop handle
			hDrop = ( HDROP )wParam;

			// Count dropped files
			uFileCount = DragQueryFile( hDrop, ( UINT )-1, NULL, 0 );

			// Loop through dropped files
			for( uWhichFile = 0; uWhichFile < uFileCount; uWhichFile ++ )
			{
				// Get size of dropped file
				uFileSize = DragQueryFile( hDrop, uWhichFile, NULL, 0 );

				// Ensure that file size is valid
				if( uFileSize != 0 )
				{
					// File size is valid

					// Get file path
					if( DragQueryFile( hDrop, uWhichFile, lpszFilePath, ( uFileSize + sizeof( char ) ) ) )
					{
						// Successfully got file path

						// Add file path to list box window
						ListBoxWindowAddString( lpszFilePath );

					} // End of successfully got file path

				} // End of file size is valid

			}; // End of loop through dropped files

			// Free string memory
			delete [] lpszFilePath;

			// Break out of switch
			break;

		} // End of a drop files message
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy main window
					DestroyWindow( hWndMain );

					// Break out of switch
					break;

				} // End of a file exit command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default command

					// See if command message is from list box window
					if( IsListBoxWindow( ( HWND )lParam ) )
					{
						// Command message is from list box window

						// Handle command message from list box window
						if( !( ListBoxWindowHandleCommandMessage( wParam, lParam, &StatusBarWindowSetText ) ) )
						{
							// Command message was not handled from list box window

							// Call default procedure
							lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

						} // End of command message was not handled from list box window

					} // End of command message is from list box window
					else
					{
						// Command message is not from list box window

						// Call default procedure
						lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

					} // End of command message is not from list box window

					// Break out of switch
					break;

				} // End of default command

			}; // End of selection for command

			// Break out of switch
			break;

		} // End of a command message
		case WM_SYSCOMMAND:
		{
			// A system command message

			// Select system command
			switch( LOWORD( wParam ) )
			{
				case IDM_HELP_ABOUT:
				{
					// A help about system command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about system command
				default:
				{
					// Default system command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

					// Break out of switch
					break;

				} // End of default system command

			}; // End of selection for system command

			// Break out of switch
			break;

		} // End of a system command message
		case WM_NOTIFY:
		{
			// A notify message
			LPNMHDR lpNmHdr;

			// Get notify message handler
			lpNmHdr = ( LPNMHDR )lParam;

			// See if notify message is from list box window
			if( IsListBoxWindow( lpNmHdr->hwndFrom ) )
			{
				// Notify message is from list box window

				// Handle notify message from list box window
				if( !( ListBoxWindowHandleNotifyMessage( wParam, lParam, &StatusBarWindowSetText ) ) )
				{
					// Notify message was not handled from list box window

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

				} // End of notify message was not handled from list box window

			} // End of notify message is from list box window
			else
			{
				// Notify message is not from list box window

				// Call default procedure
				lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

			} // End of notify message is not from list box window


			// Break out of switch
			break;

		} // End of a notify message
		case WM_CONTEXTMENU:
		{
			// A context menu message
			HMENU hMenu;

			// Load context menu
			hMenu = LoadMenu( NULL, MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Show context menu
			TrackPopupMenu( GetSubMenu( hMenu, 0 ), ( TPM_LEFTALIGN | TPM_LEFTBUTTON ), LOWORD( lParam ), HIWORD( lParam ), 0, hWndMain, NULL );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A close message

			// Save file
			if( ListBoxWindowSave( TEMPLATE_FILE_NAME ) )
			{
				// Successfully saved file

				// Destroy main window
				DestroyWindow( hWndMain );

			} // End of successfully saved file
			else
			{
				// Unable to save file

				// Ensure that user is ok to close
				if( MessageBox( hWndMain, LIST_BOX_WINDOW_UNABLE_TO_SAVE_WARNING_MESSAGE, WARNING_MESSAGE_CAPTION, ( MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING ) ) == IDYES )
				{
					// User is ok to close

					// Destroy main window
					DestroyWindow( hWndMain );

				} // End of user is ok to close

			} // End of unable to save file

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A destroy message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call default window procedure
			lr = DefWindowProc( hWndMain, uMsg, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWindowProcedure

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	MSG msg;

	WNDCLASSEX wcMain;

	// Clear message structure
	ZeroMemory( &msg, sizeof( msg ) );

	// Clear window class structure
	ZeroMemory( &wcMain, sizeof( wcMain ) );

	// Initialise window class structure
	wcMain.cbSize			= sizeof( WNDCLASSEX );
	wcMain.lpfnWndProc		= MainWindowProcedure;
	wcMain.hInstance		= hInstance;
	wcMain.lpszClassName	= MAIN_WINDOW_CLASS_NAME;
	wcMain.style			= MAIN_WINDOW_CLASS_STYLE;
	wcMain.hIcon			= MAIN_WINDOW_CLASS_ICON;
	wcMain.hCursor			= MAIN_WINDOW_CLASS_CURSOR;
	wcMain.hbrBackground	= MAIN_WINDOW_CLASS_BACKGROUND;
	wcMain.lpszMenuName		= MAIN_WINDOW_CLASS_MENU_NAME;
	wcMain.hIconSm			= MAIN_WINDOW_CLASS_ICON_SMALL;

	// Register main window class
	if( RegisterClassEx( &wcMain ) )
	{
		// Successfully registered main window class
		HWND hWndMain;

		// Create main window
		hWndMain = CreateWindowEx( MAIN_WINDOW_EXTENDED_STYLE, MAIN_WINDOW_CLASS_NAME, MAIN_WINDOW_TEXT, MAIN_WINDOW_STYLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL );

		// Ensure that main window was created
		if( hWndMain )
		{
			// Successfully created main window
			HMENU hMenuSystem;
			LPWSTR *lpszArgumentList;
			int nArgumentCount;
			int nItemCount;

			// Allocate string memory
			LPTSTR lpszStatusMessage = new char[ STRING_LENGTH + sizeof( char ) ];

			// Get system menu
			hMenuSystem = GetSystemMenu( hWndMain, FALSE );

			// Add separator item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_SEPARATOR_ITEM_POSITION, ( MF_BYPOSITION | MF_SEPARATOR ), 0, NULL );

			// Add about item to system menu
			InsertMenu( hMenuSystem, SYSTEM_MENU_ABOUT_ITEM_POSITION, MF_BYPOSITION, IDM_HELP_ABOUT, SYSTEM_MENU_ABOUT_ITEM_TEXT );

			// Get argument list
			lpszArgumentList = CommandLineToArgvW( GetCommandLineW(), &nArgumentCount );

			// Ensure that argument list was got
			if( lpszArgumentList )
			{
				// Successfully got argument list
				int nWhichArgument;
				int nSizeNeeded;
				int nWideArgumentLength;

				// Allocate string memory
				LPTSTR lpszArgument = new char[ STRING_LENGTH + sizeof( char ) ];

				// Loop through arguments
				for( nWhichArgument = 1; nWhichArgument < nArgumentCount; nWhichArgument ++ )
				{
					// Get wide argument length
					nWideArgumentLength = lstrlenW( lpszArgumentList[ nWhichArgument ] );

					// Get size required for argument
					nSizeNeeded = WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ nWhichArgument ], nWideArgumentLength, NULL, 0, NULL, NULL );

					// Convert argument to ansi
					WideCharToMultiByte( CP_UTF8, 0, lpszArgumentList[ nWhichArgument ], nWideArgumentLength, lpszArgument, nSizeNeeded, NULL, NULL );

					// Terminate argument
					lpszArgument[ nSizeNeeded ] = ( char )NULL;

					// Add argument to list box window
					ListBoxWindowAddString( lpszArgument );

				}; // End of loop through arguments

				// Free string memory
				delete [] lpszArgument;

			} // End of successfully got argument list

			// Show main window
			ShowWindow( hWndMain, nCmdShow );

			// Update main window
			UpdateWindow( hWndMain );

			// Populate list box window
			nItemCount = ListBoxWindowPopulate( TEMPLATE_FILE_NAME );

			// Format status message
			wsprintf( lpszStatusMessage, LIST_BOX_WINDOW_POPULATE_STATUS_MESSAGE_FORMAT_STRING, TEMPLATE_FILE_NAME, nItemCount );

			// Show status message on status bar window
			StatusBarWindowSetText( lpszStatusMessage );

			// Message loop
			while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
			{
				// Translate message
				TranslateMessage( &msg );

				// Dispatch message
				DispatchMessage( &msg );

			}; // End of message loop

			// Free string memory
			delete [] lpszStatusMessage;

		} // End of successfully main created window
		else
		{
			// Unable to create main window

			// Display error message
			MessageBox( NULL, UNABLE_TO_CREATE_MAIN_WINDOW_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to create main window

	} // End of successfully registered main window class
	else
	{
		// Unable to register main window class

		// Display error message
		MessageBox( NULL, UNABLE_TO_REGISTER_MAIN_WINDOW_CLASS_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to register main window class

	return msg.wParam;

} // End of function WinMain
