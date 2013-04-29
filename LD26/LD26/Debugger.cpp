#include "Debugger.h"

std::function<void (const char*, const char*, unsigned int)> Debugger::debugPrintFunction = [](const char*, const char*, unsigned int){};
