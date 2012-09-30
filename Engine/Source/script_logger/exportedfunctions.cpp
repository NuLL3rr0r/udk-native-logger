#include <windows.h>
#include <utils.h>
#include "exportedfunctions.hpp"
#include "scriptlogger.hpp"

using namespace UScriptLogger;

ScriptLogger scriptLoggerObject;

extern "C"
{
	__declspec(dllexport) void Assert(const wchar_t *condition, const wchar_t *message, const wchar_t *channel, const wchar_t *source,
		const wchar_t *className, const wchar_t *stateName, const wchar_t *funcName)
	{
		scriptLoggerObject.Assert(wstrtostr(condition), wstrtostr(message), wstrtostr(channel), wstrtostr(source),
			wstrtostr(className), wstrtostr(stateName), wstrtostr(funcName));
	}

	__declspec(dllexport) void Debug(const wchar_t *message, const wchar_t *channel, const wchar_t *source,
		const wchar_t *className, const wchar_t *stateName, const wchar_t *funcName)
	{
		scriptLoggerObject.Debug(wstrtostr(message), wstrtostr(channel), wstrtostr(source),
			wstrtostr(className), wstrtostr(stateName), wstrtostr(funcName));
	}

	__declspec(dllexport) void Notice(const wchar_t *message, const wchar_t *channel, const wchar_t *source,
		const wchar_t *className, const wchar_t *stateName, const wchar_t *funcName)
	{
		scriptLoggerObject.Notice(wstrtostr(message), wstrtostr(channel), wstrtostr(source),
			wstrtostr(className), wstrtostr(stateName), wstrtostr(funcName));
	}

	__declspec(dllexport) void Warn(const wchar_t *message, const wchar_t *channel, const wchar_t *source,
		const wchar_t *className, const wchar_t *stateName, const wchar_t *funcName)
	{
		scriptLoggerObject.Warn(wstrtostr(message), wstrtostr(channel), wstrtostr(source),
			wstrtostr(className), wstrtostr(stateName), wstrtostr(funcName));
	}

	__declspec(dllexport) void Error(const wchar_t *message, const wchar_t *channel, const wchar_t *source,
		const wchar_t *className, const wchar_t *stateName, const wchar_t *funcName)
	{
		scriptLoggerObject.Error(wstrtostr(message), wstrtostr(channel), wstrtostr(source),
			wstrtostr(className), wstrtostr(stateName), wstrtostr(funcName));
	}

	__declspec(dllexport) void Fatal(const wchar_t *message, const wchar_t *channel, const wchar_t *source,
		const wchar_t *className, const wchar_t *stateName, const wchar_t *funcName)
	{
		scriptLoggerObject.Fatal(wstrtostr(message), wstrtostr(channel), wstrtostr(source),
			wstrtostr(className), wstrtostr(stateName), wstrtostr(funcName));
	}
}

