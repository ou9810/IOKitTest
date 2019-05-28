/* add your code here */

#include <IOKit/IOLib.h>
#include "IOKitDriver.hpp"

// This required macro defines the class's constructors, destructors,
// and several other methods I/O Kit requires.
OSDefineMetaClassAndStructors(com_ryan_test_IOKitDriver, IOService)

// Define the driver's superclass.
#define super IOService

bool com_ryan_test_IOKitDriver::init(OSDictionary *dict)
{
    bool result = super::init(dict);
    IOLog("Initializing\n");
    return result;
}

void com_ryan_test_IOKitDriver::free(void)
{
    IOLog("Freeing\n");
    super::free();
}

IOService *com_ryan_test_IOKitDriver::probe(IOService *provider,
                                                SInt32 *score)
{
    IOService *result = super::probe(provider, score);
    IOLog("Probing\n");
    return result;
}

bool com_ryan_test_IOKitDriver::start(IOService *provider)
{
    bool result = super::start(provider);
    IOLog("Starting\n");
    return result;
}

void com_ryan_test_IOKitDriver::stop(IOService *provider)
{
    IOLog("Stopping\n");
    super::stop(provider);
}
