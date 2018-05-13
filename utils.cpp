#include "utils.h"

const char* strstr_caseinsensitive(const char* mainstring, const char* searchterm)
{
	int i, j, k;

	for (i = 0; mainstring[i]; i++)
	{
		for (j = i, k = 0; tolower(mainstring[j]) == tolower(searchterm[k]); j++, k++)
		{
			if (searchterm[k + 1] == NULL)
				return (mainstring + i);
		}
	}
	return NULL;
}

bool stringCompareCaseInsesitive(const char* string1, const char* string2) {
	if (string1 == nullptr || string2 == nullptr) return false;
	for (size_t i = 0; string1[i] != '\0'; i++) {
		if (string2[i] == '\0') return true;
		if (tolower(string1[i] != tolower(string2[i])))  return false;
	}
	return true;
}