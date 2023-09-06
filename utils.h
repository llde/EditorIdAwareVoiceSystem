#pragma once
const char* strstr_caseinsensitive(const char* mainstring, const char* searchterm);


bool stringCompareCaseInsesitive(const char* string1 , const char* string2);


void stringToLower(char* string);

#ifdef _DEBUG
#define MESSAGE_DEBUG _MESSAGE
#else
inline void _MESSAGESTUB(const char* fmt, ...) {}
#define MESSAGE_DEBUG(x , ...) _MESSAGESTUB(x, __VA_ARGS__)
#endif