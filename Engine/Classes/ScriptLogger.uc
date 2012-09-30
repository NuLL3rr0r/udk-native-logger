class GSScriptLogger extends Object
	DLLBind(script_logger);


dllimport final function bool Assert(string condition, string message, string channel, string source, string className, string stateName, string funcName);
dllimport final function bool Debug(string message, string channel, string source, string className, string stateName, string funcName);
dllimport final function bool Notice(string message, string channel, string source, string className, string stateName, string funcName);
dllimport final function bool Warn(string message, string channel, string source, string className, string stateName, string funcName);
dllimport final function bool Error(string message, string channel, string source, string className, string stateName, string funcName);
dllimport final function bool Fatal(string message, string channel, string source, string className, string stateName, string funcName);


DefaultProperties
{
}
