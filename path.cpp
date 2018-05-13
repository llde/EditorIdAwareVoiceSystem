#include "path.h"
/*
* Return the NULL terminated path component in an HEAP allocated way
* nullptr if invalid component
*/
char* getSingleComponent(Component path_component, const char* input) {
	size_t component = 0;
	size_t len = strlen(input);
	const char* new_ptr = nullptr;
	size_t comp_len = 0;
	for (size_t i = 0; i < len; i++) {
		if (input[i] == '\\') {
			component++;
			continue;
		}
		if (path_component == Component::Race && component > 4) break;
		if (path_component == Component::Race && component == 4) {
			if (new_ptr == NULL) new_ptr = input + i;
			comp_len++;
		}
	}
	if (new_ptr == nullptr || comp_len == 0) return nullptr;
	char* ttt = new char[comp_len + 1];
	memcpy(ttt, new_ptr, comp_len);
	ttt[comp_len] = '\0';
	//_MESSAGE("SIZE %zu", comp_len);
	return ttt;
}


bool replacePathComponent(Component path_component, const char* input, const char* repl, char* output) {
	size_t component = 0;
	size_t len = strlen(input);
	const char* new_ptr = nullptr;
	size_t size_until_component = 0;
	size_t remain = 0;
	for (size_t i = 0; i < len; i++) {
		if (input[i] == '\\') {
			component++;
			if (path_component == Component::Race && component == 4) {
				size_until_component = i;
			}
			if (path_component == Component::Race && component > 4) {
				new_ptr = input + i;
				remain = len - i;
				break;
			};
		}
	}
	if (new_ptr == nullptr) return false;
	memset(output,0, len);
 	memcpy(output, input , size_until_component);
	memcpy(output + size_until_component, repl, strlen(repl) -1);
	_MESSAGE("%s" , output);
	memcpy(output + size_until_component + strlen(repl),  new_ptr, remain);
	_MESSAGE("%s", new_ptr);
	return true;
}