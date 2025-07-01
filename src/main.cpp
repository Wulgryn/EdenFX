#include "PandoraDebug/console.hpp"
#include <windows.h>
#include <iostream>
using namespace std;
int main(int argc, char const *argv[])
{
    PandoraDebug::DC::log("Pandora Debug Console Initialized %d", 12);
    PandoraDebug::DC::logError("This is an error message with code %d", 404);
    PandoraDebug::DC::detach(true);
    system("pause");
    cout << endl;
    // cout << "Debug console detached. Press any key to continue..." << endl;
    PandoraDebug::DC::logInfo("This is an informational message with value %f", 3.14);
    PandoraDebug::DC::logWarning("This is a warning message with value %s", "caution");
    PandoraDebug::DC::logDebug("This is a debug message with value %d", 42);
    system("pause");
    PandoraDebug::DC::attach();
    PandoraDebug::DC::logTrace("This is a trace message with value %s", "trace info");
    PandoraDebug::DC::logFatal("This is a fatal error message with code %d", 500);
    PandoraDebug::DC::logSuccess("This is a success message with value %s", "operation completed");
    PandoraDebug::DC::logFailure("This is a failure message with value %s", "operation failed");
    PandoraDebug::DC::logCritical("This is a critical message with value %s", "critical error");
    system("pause");
    return 0;
}
