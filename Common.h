// Common.h

#pragma once

#include <windows.h>

#define STRING_LENGTH															2048

#define ALL_FILES_FILTER														"*.*"
#define NEW_LINE_TEXT															"\r\n"

#define WINDOW_BORDER_WIDTH														::GetSystemMetrics( SM_CXBORDER )
#define WINDOW_BORDER_HEIGHT													::GetSystemMetrics( SM_CYBORDER )

#define SAVE_FILE_FILTER														"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0"
#define SAVE_FILE_DEFAULT_EXTENSION												"txt"

#define ABOUT_MESSAGE_CAPTION													"About"
#define ERROR_MESSAGE_CAPTION													"Error"
#define INFORMATION_MESSAGE_CAPTION												"Information"
#define WARNING_MESSAGE_CAPTION													"Warning"
