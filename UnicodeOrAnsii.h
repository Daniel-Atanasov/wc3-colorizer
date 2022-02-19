#include <iostream>
#include <string>

#if defined(UNICODE) && defined(_UNICODE)
#define TChar wchar_t
#define TString std::wstring
#define IStringStream std::wistringstream
#define IFStream std::wifstream
#define ToString std::to_wstring
#define STR(quote) L##quote
#else
#define TString std::string
#define TChar char
#define IStringStream std::istringstream
#define IFStream std::ifstream
#define ToString std::to_string
#define STR(quote) quote
#endif