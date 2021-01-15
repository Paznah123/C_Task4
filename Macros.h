#pragma once

#define CHECK_RETURN_0(POINTER) if(!POINTER) return 0				// 1

#define CHECK_RETURN_NULL(POINTER) if(!POINTER) return NULL			// 2

#define CHECK_MSG_RETURN_0(POINTER, MESSAGE)\
if(!POINTER) {printf("%s", MESSAGE); return 0;}						// 3

#define CHECK_0_MSG_CLOSE_FILE(X, POINTER, MESSAGE)\
if(X == 0){ printf("%s", MESSAGE); fclose(POINTER); return 0;}		// 4

#define CHECK_NULL_MSG_CLOSE_FILE(POINTER, MESSAGE)\
if(!POINTER){printf("%s", MESSAGE); fclose(POINTER); return 0;}		// 5

#define MSG_CLOSE_RETURN_0(POINTER, MESSAGE)\
puts(MESSAGE); fclose(POINTER); return 0;							// 6