/**
 * @file ColoredChar_Constants.h
 * @author Amin Karic
 * @brief File containing constants for ColoredChar struct
 * @version 0.1
 * @date 2025-11-17
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef COLORED_CHAR_CONSTANTS_H
#define COLORED_CHAR_CONSTANTS_H

//* Basic color constants
#define CCHAR_BLACK 0x000000FF
#define CCHAR_WHITE 0xFFFFFFFF
#define CCHAR_RED 0xFF0000FF
#define CCHAR_GREEN 0x00FF00FF
#define CCHAR_BLUE 0x0000FFFF
#define CCHAR_YELLOW 0xFFFF00FF
#define CCHAR_CYAN 0x00FFFFFF
#define CCHAR_MAGENTA 0xFF00FFFF

//* Black character constant
#define BLANK_CHARACTER ColoredChar(' ', CCHAR_BLACK)

//* Unicode characters
#define ANSI_RESET "\x1b[0m"

#endif