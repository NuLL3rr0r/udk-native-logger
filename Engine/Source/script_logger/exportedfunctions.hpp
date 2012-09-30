#ifndef EXPORTEDFUNCTIONS_HPP
#define EXPORTEDFUNCTIONS_HPP


extern "C"
{
	__declspec(dllexport) void Assert(const wchar_t *condition, const wchar_t *message, const wchar_t *channel, const wchar_t *source,
		const wchar_t *className, const wchar_t *stateName, const wchar_t *funcName);

	__declspec(dllexport) void Debug(const wchar_t *message, const wchar_t *channel, const wchar_t *source,
		const wchar_t *className, const wchar_t *stateName, const wchar_t *funcName);

	__declspec(dllexport) void Notice(const wchar_t *message, const wchar_t *channel, const wchar_t *source,
		const wchar_t *className, const wchar_t *stateName, const wchar_t *funcName);

	__declspec(dllexport) void Warn(const wchar_t *message, const wchar_t *channel, const wchar_t *source,
		const wchar_t *className, const wchar_t *stateName, const wchar_t *funcName);

	__declspec(dllexport) void Error(const wchar_t *message, const wchar_t *channel, const wchar_t *source,
		const wchar_t *className, const wchar_t *stateName, const wchar_t *funcName);

	__declspec(dllexport) void Fatal(const wchar_t *message, const wchar_t *channel, const wchar_t *source,
		const wchar_t *className, const wchar_t *stateName, const wchar_t *funcName);
}


#endif /* EXPORTEDFUNCTIONS_HPP */


