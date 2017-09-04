#pragma once


#ifdef SERVFUNC_EXPORTS
#define SERVFUNC_API __declspec(dllexport)
#else
#define SERVFUNC_API __declspec(dllimport)
#endif // SERV_FUNC_EXPORT

namespace servfunc
{
	class function1 {
	public:
		static SERVFUNC_API const char* echo(const char* message); //echo text back
		static SERVFUNC_API const char* reverse(const char* x); //reverse text
	};
}