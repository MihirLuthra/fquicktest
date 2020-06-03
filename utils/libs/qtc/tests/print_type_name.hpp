#ifndef __PRINT_TYPE_NAME_HPP__
#define __PRINT_TYPE_NAME_HPP__

#include <string_view>

/*
 * Thanks 康桓瑋for posting this on
 * https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/56766138#56766138
 * under CC0 license
 */
template <typename T>
constexpr std::string_view
type_name()
{
	std::string_view name, prefix, suffix;
#ifdef __clang__
	name = __PRETTY_FUNCTION__;
	prefix = "std::string_view type_name() [T = ";
	suffix = "]";
#elif defined(__GNUC__)
	name = __PRETTY_FUNCTION__;
	prefix = "constexpr std::string_view type_name() [with T = ";
	suffix = "; std::string_view = std::basic_string_view<char>]";
#elif defined(_MSC_VER)
	name = __FUNCSIG__;
	prefix = "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl type_name<";
	suffix = ">(void)";
#endif
	name.remove_prefix(prefix.size());
	name.remove_suffix(suffix.size());
	return name;
}

#endif /* __PRINT_TYPE_NAME_HPP__ */
