#pragma once

enum class Component {
	Master,
	Race,
	Sex,
};

//TODO only Race will be imlemented for now

/*
* Return the NULL terminated path component in an HEAP allocated way
* nullptr if invalid component
*/
char* getSingleComponent(Component path_component, const char* input);

/*
* Replace the Component of the path input with repl
@param path_component: The component of the path
@param input : the path
@param repl : the string that replace the component
@param output: the sustituted path. The caller is responsible to make sure there is enough space
*/
bool replacePathComponent(Component path_component, const char* input, const char* repl, char* output);