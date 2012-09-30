#include "scriptlogger.hpp"
#include "log.hpp"

using namespace UScriptLogger;

ScriptLogger::ScriptLogger()
{
	Log::Initialize("ScriptLogger");

	NOTICE("ScriptLogger Initialized");
}

ScriptLogger::~ScriptLogger()
{
}

void ScriptLogger::Assert(const std::string &condition, const std::string &message, const std::string &channel, const std::string &source,
	const std::string &className, const std::string &stateName, const std::string &funcName)
{
}

void ScriptLogger::Debug(const std::string &message, const std::string &channel, const std::string &source,
	const std::string &className, const std::string &stateName, const std::string &funcName)
{
}

void ScriptLogger::Notice(const std::string &message, const std::string &channel, const std::string &source,
	const std::string &className, const std::string &stateName, const std::string &funcName)
{
	NOTICE(message);
}

void ScriptLogger::Warn(const std::string &message, const std::string &channel, const std::string &source,
	const std::string &className, const std::string &stateName, const std::string &funcName)
{
}

void ScriptLogger::Error(const std::string &message, const std::string &channel, const std::string &source,
	const std::string &className, const std::string &stateName, const std::string &funcName)
{
}

void ScriptLogger::Fatal(const std::string &message, const std::string &channel, const std::string &source,
	const std::string &className, const std::string &stateName, const std::string &funcName)
{
}

