// SplashScreenWindow.cpp

#include "SplashScreenWindow.h"

// Global variables
static DWORD g_dwMilliSeconds;
static LPTSTR g_lpszText;

SIZE GetTextSize( HDC hdcWindow, LPCTSTR lpszInputText )
{
	SIZE textSize;

	SIZE lineSize;
	LONG lTextWidth		= 0;
	LONG lTextHeight	= 0;
	DWORD dwLineLength;
	LPTSTR lpszStartOfLine = ( LPTSTR )lpszInputText;
	LPTSTR lpszEndOfLine;

	// Allocate string memory
	LPTSTR lpszLine = new char[ STRING_LENGTH + sizeof( char ) ];

	// Loop through all lines in text
	while( lpszStartOfLine )
	{
		// Find end of line
		lpszEndOfLine = strstr( lpszStartOfLine, NEW_LINE_TEXT );

		// See if end of line was found
		if( lpszEndOfLine )
		{
			// Successfully found end of line

			// Calculate line length
			dwLineLength = ( lpszEndOfLine - lpszStartOfLine );

			// See if line contains text
			if( dwLineLength > 0 )
			{
				// Line contains text

				// Store line text
				lstrcpyn( lpszLine, lpszStartOfLine, ( dwLineLength + sizeof( char ) ) );

			} // End of line contains text
			else
			{
				// Line is empty

				// Copy space into line
				lstrcpy( lpszLine, ASCII_SPACE_STRING );
				// The below code to get size of line won't work if the line is empty

			} // End of line is empty

			// Get size of line
			GetTextExtentPoint32( hdcWindow, lpszLine, lstrlen( lpszLine ), &lineSize );

			// See if line width is greater than text width
			if( lineSize.cx > lTextWidth )
			{
				// Line width is greater than text width

				// Update text width
				lTextWidth = lineSize.cx;

			} // End of line width is greater than text width

			// Update text height
			lTextHeight += lineSize.cy;

			// Advance to start of next line
			lpszStartOfLine = ( lpszEndOfLine + lstrlen( NEW_LINE_TEXT ) );

		} // End of successfully found end of line
		else
		{
			// Unable to find end of line

			// See if line contains text
			if( lpszStartOfLine[ 0 ] )
			{
				// Line contains text

				// Get size of line
				GetTextExtentPoint32( hdcWindow, lpszStartOfLine, lstrlen( lpszLine ), &lineSize );

				// See if line width is greater than text width
				if( lineSize.cx > lTextWidth )
				{
					// Line width is greater than text width

					// Update text width
					lTextWidth = lineSize.cx;

				} // End of line width is greater than text width

				// Update text height
				lTextHeight += lineSize.cy;

			} // End of line contains text

			// Force exit from loop
			lpszStartOfLine = NULL;

		} // End of unable to find end of line

	}; // End of loop through all lines in text

	// Update text size
	textSize.cx = lTextWidth;
	textSize.cy = lTextHeight;

	// Free string memory
	delete [] lpszLine;

	return textSize;

} // End of function GetTextSize

LRESULT CALLBACK SplashScreenWindowProcedure( HWND hWndSplashScreen, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMessage )
	{
		case WM_CREATE:
		{
			// A create message

			// Initialise timer
			SetTimer( hWndSplashScreen, 0, g_dwMilliSeconds, NULL );

			// Break out of switch
			break;

		} // End of a create message
		case WM_PAINT:
		{
			// A paint message
			PAINTSTRUCT ps;
			HDC hdcWindow;

			// Begin painting
			hdcWindow = BeginPaint( hWndSplashScreen, &ps );

			// Ensure that painting was begun
			if( hdcWindow )
			{
				// Successfully begun painting
				RECT rcClient;
				int nWindowLeft;
				int nWindowTop;
				int nWindowWidth;
				int nWindowHeight;
				SIZE textSize;

				// Get text size
				textSize = GetTextSize( hdcWindow, g_lpszText );

				// Calculate new window size
				nWindowWidth	= ( SPLASH_SCREEN_WINDOW_BORDER_SIZE + textSize.cx + SPLASH_SCREEN_WINDOW_BORDER_SIZE );
				nWindowHeight	= ( SPLASH_SCREEN_WINDOW_BORDER_SIZE + textSize.cy + SPLASH_SCREEN_WINDOW_BORDER_SIZE );

				// Calculate new window position
				nWindowLeft		= ( ( GetSystemMetrics( SM_CXSCREEN ) - nWindowWidth ) / 2 );
				nWindowTop		= ( ( GetSystemMetrics( SM_CYSCREEN ) - nWindowHeight ) / 2 );

				// Move window
				MoveWindow( hWndSplashScreen, nWindowLeft, nWindowTop, nWindowWidth, nWindowHeight, TRUE );

				// Update client rect for text
				rcClient.left	= SPLASH_SCREEN_WINDOW_BORDER_SIZE;
				rcClient.top	= SPLASH_SCREEN_WINDOW_BORDER_SIZE;
				rcClient.right	= ( textSize.cx + SPLASH_SCREEN_WINDOW_BORDER_SIZE );
				rcClient.bottom	= ( textSize.cy + SPLASH_SCREEN_WINDOW_BORDER_SIZE );

				// Set text color
				SetTextColor( hdcWindow, SPLASH_SCREEN_WINDOW_CLASS_TEXT_COLOR );

				// Set text background mode
				SetBkMode( hdcWindow, TRANSPARENT );

				// Draw text
				DrawText( hdcWindow, g_lpszText, lstrlen( g_lpszText ), &rcClient, DT_CENTER );

				// End painting
				EndPaint( hWndSplashScreen, &ps );

			} // End of successfully begun painting

			// Break out of switch
			break;

		} // End of a paint message
		case WM_TIMER:
		{
			// A timer message

			// Close splash screen window
			SendMessage( hWndSplashScreen, WM_CLOSE, ( WPARAM )NULL, ( LPARAM )NULL );

			// Break out of switch
			break;

		} // End of a timer message
		case WM_CLOSE:
		{
			// A close message

			// Destroy splash screen window
			DestroyWindow( hWndSplashScreen );

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A close message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a close message
		default:
		{
			// A close message

			// Call default window procedure
			lr = DefWindowProc( hWndSplashScreen, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of a close message

	}; // End of selection for message

	return lr;

} // End of function SplashScreenWindowProcedure

BOOL SplashScreenWindowShow( HINSTANCE hInstance, LPCTSTR lpszText, DWORD dwMilliSeconds )
{
	BOOL bResult = FALSE;

	WNDCLASSEX wcSplashScreen;
	DWORD dwTextLength;

	// Get text length
	dwTextLength = lstrlen( lpszText );

	// Allocate string memory
	g_lpszText = new char[ dwTextLength + sizeof( char ) ];

	// Store global text
	lstrcpy( g_lpszText, lpszText );

	// Update global milliseconds
	g_dwMilliSeconds = dwMilliSeconds;

	// Clear splash screen window class structure
	ZeroMemory( &wcSplashScreen, sizeof( wcSplashScreen ) );

	// Initialise splash screen window class structure
	wcSplashScreen.cbSize			= sizeof( WNDCLASSEX );
	wcSplashScreen.lpfnWndProc		= SplashScreenWindowProcedure;
	wcSplashScreen.hInstance		= hInstance;
	wcSplashScreen.lpszClassName	= SPLASH_SCREEN_WINDOW_CLASS_NAME;
	wcSplashScreen.style			= SPLASH_SCREEN_WINDOW_CLASS_STYLE;
	wcSplashScreen.hbrBackground	= SPLASH_SCREEN_WINDOW_CLASS_BACKGROUND;

	// Register splash screen window class
	if( RegisterClassEx( &wcSplashScreen ) )
	{
		// Successfully registered splash screen window class
		HWND hWndSplashScreen;
		int nLeft;
		int nTop;

		// Calculate window position
		nLeft	= ( ( GetSystemMetrics( SM_CXSCREEN ) - SPLASH_SCREEN_WINDOW_INITIAL_WIDTH ) / 2 );
		nTop	= ( ( GetSystemMetrics( SM_CYSCREEN ) - SPLASH_SCREEN_WINDOW_INITIAL_HEIGHT ) / 2 );

		// Create splash screen window
		hWndSplashScreen = CreateWindowEx( SPLASH_SCREEN_WINDOW_EXTENDED_STYLE, SPLASH_SCREEN_WINDOW_CLASS_NAME, SPLASH_SCREEN_WINDOW_TEXT, SPLASH_SCREEN_WINDOW_STYLE, nLeft, nTop, SPLASH_SCREEN_WINDOW_INITIAL_WIDTH, SPLASH_SCREEN_WINDOW_INITIAL_HEIGHT, NULL, NULL, hInstance, NULL );

		// Ensure that splash screen window was created
		if( hWndSplashScreen )
		{
			// Successfully created splash screen window
			MSG msg;

			// Show splash screen window
			ShowWindow( hWndSplashScreen, SW_SHOW );

			// Update splash screen window
			UpdateWindow( hWndSplashScreen );

			// Main message loop
			while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
			{
				// Translate message
				TranslateMessage( &msg );

				// Dispatch message
				DispatchMessage(&msg);

			}; // End of main message loop

			// Update return value
			bResult = TRUE;

		} // End of successfully created splash screen window

	} // End of successfully registered splash screen window class

	// Free string memory
	delete [] g_lpszText;

	return bResult;

} // End of function SplashScreenWindowShow
