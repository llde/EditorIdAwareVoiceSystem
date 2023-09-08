#pragma once

template <typename C>
struct reverse_wrapper {

    C& c_;
    reverse_wrapper(C& c) : c_(c) {}

    typename C::reverse_iterator begin() { return c_.rbegin(); }
    typename C::reverse_iterator end() { return c_.rend(); }
};

template <typename C, size_t N>
struct reverse_wrapper< C[N] > {

    C(&c_)[N];
    reverse_wrapper(C(&c)[N]) : c_(c) {}

    typename std::reverse_iterator<const C*> begin() { return std::rbegin(c_); }
    typename std::reverse_iterator<const C*> end() { return std::rend(c_); }
};

const char* strstr_caseinsensitive(const char* mainstring, const char* searchterm);


bool stringCompareCaseInsesitive(const char* string1 , const char* string2);


void stringToLower(char* string);

#ifdef _DEBUG
#define MESSAGE_DEBUG _MESSAGE
#else
inline void _MESSAGESTUB(const char* fmt, ...) {}
#define MESSAGE_DEBUG(x , ...) _MESSAGESTUB(x, __VA_ARGS__)
#endif