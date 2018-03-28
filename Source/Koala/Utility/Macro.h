#ifndef KOALA__UTILITY__MACRO
#define KOALA__UTILITY__MACRO

#include <string>
#include <map>

// Creating enums with strings
#define VALUE_TO_ENUM(value) value,
#define VALUE_TO_ENUM_ASSIGNMENT(value) value = ASSIGNMENT_##value,
#define VALUE_TO_STRING(value) {value, #value},

#define DECLARE_ENUM(name, values) \
	enum class name { values(VALUE_TO_ENUM, VALUE_TO_ENUM_ASSIGNMENT) }; \
	static const std::string& name##ToString(name v) \
	{ \
		static std::map<int, std::string> strings; \
		if(strings.size() == 0) \
		{ \
			enum { values(VALUE_TO_ENUM, VALUE_TO_ENUM_ASSIGNMENT) }; \
			strings = std::map<int, std::string>{ values(VALUE_TO_STRING, VALUE_TO_STRING) }; \
		} \
		return strings[int(v)]; \
	}

#endif // KOALA__UTILITY__MACRO
