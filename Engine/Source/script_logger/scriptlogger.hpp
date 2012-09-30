#ifndef SCRIPTLOGGER_HPP
#define SCRIPTLOGGER_HPP


#include <string>

namespace UScriptLogger {
    class ScriptLogger;
}

class UScriptLogger::ScriptLogger
{
public:
	ScriptLogger();
    ~ScriptLogger();

public:
	void Assert(const std::string &condition, const std::string &message, const std::string &channel, const std::string &source,
		const std::string &className, const std::string &stateName, const std::string &funcName);

	void Debug(const std::string &message, const std::string &channel, const std::string &source,
		const std::string &className, const std::string &stateName, const std::string &funcName);

	void Notice(const std::string &message, const std::string &channel, const std::string &source,
		const std::string &className, const std::string &stateName, const std::string &funcName);

	void Warn(const std::string &message, const std::string &channel, const std::string &source,
		const std::string &className, const std::string &stateName, const std::string &funcName);

	void Error(const std::string &message, const std::string &channel, const std::string &source,
		const std::string &className, const std::string &stateName, const std::string &funcName);

	void Fatal(const std::string &message, const std::string &channel, const std::string &source,
		const std::string &className, const std::string &stateName, const std::string &funcName);
};


#endif /* SCRIPTLOGGER_HPP */

