#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <functional>

#ifndef OVERRIDEDEBBUGGER
#define DEBUG(message) (Debugger::debug(__FILE__, message, __LINE__))
#else
#define DEBUG(messanger, message)     ((void)0)
#endif

struct Debugger
{
	static void debug(const char* messanger, const char* message, unsigned int line) 
	{
		Debugger::debugPrintFunction(messanger, message, line);
	};

	static void setPrintFunction(std::function<void (const char*, const char*, unsigned int)> func)
	{
		debugPrintFunction = func;
	};

private:
	static std::function<void (const char*, const char*, unsigned int)> debugPrintFunction;
};

#endif