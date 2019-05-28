/* add your code here */

#include <IOKit/IOLib.h>
#include <IOKit/usb/IOUSBInterface.h>
#include "IOKitDC122Driver.hpp"

// This required macro defines the class's constructors, destructors,
// and several other methods I/O Kit requires.
OSDefineMetaClassAndStructors(com_ryan_test_IOKitDC122Driver, IOService)

// Define the driver's superclass.
#define super IOService

void logEndpoint(IOUSBPipe *pipe) {
    IOLog("Endpoint #%d ", pipe->GetEndpointNumber());
    IOLog("--> Type: ");
    
    switch (pipe->GetType()) {
        case kUSBControl:
            IOLog("kUSBControl ");
            break;
        case kUSBBulk:
            IOLog("kUSBBulk ");
            break;
        case kUSBInterrupt:
            IOLog("kUSBInterrupt ");
            break;
        case kUSBIsoc:
            IOLog("kUSBIsoc ");
            break;
    }
    
    IOLog("--> Direction: ");
    
    switch (pipe->GetDirection()) {
        case kUSBIn:
            IOLog("IN (kUSBIn) ");
            break;
        case kUSBOut:
            IOLog("OUT (kUSBOut) ");
            break;
        case kUSBAnyDirn:
            IOLog("ANY (kUSBAnyDirn) ");
            break;
    }
    
    IOLog("maxPacketSize: %d interval: %d\n", pipe->GetMaxPacketSize(), pipe->GetInterval());
}

bool com_ryan_test_IOKitDC122Driver::init(OSDictionary *dict)
{
    IOLog("com_ryan_test_IOKitDC122Driver::init(%p)\n", this);
    bool result = super::init(dict);
    return result;
}

void com_ryan_test_IOKitDC122Driver::free()
{
    IOLog("com_ryan_test_IOKitDC122Driver::free(%p)\n", this);
    super::free();
}

IOService *com_ryan_test_IOKitDC122Driver::probe(IOService *provider, SInt32 *score)
{
    IOLog("%s(%p)::probe\n", getName(), this);
    IOService *result = super::probe(provider, score);
    return result;
}

bool com_ryan_test_IOKitDC122Driver::attach(IOService *provider)
{
    IOLog("%s(%p)::attach\n", getName(), this);
    bool result = super::attach(provider);
    return result;
}

void com_ryan_test_IOKitDC122Driver::detach(IOService *provider)
{
    IOLog("%s(%p)::detach\n", getName(), this);
    super::detach(provider);
}

bool com_ryan_test_IOKitDC122Driver::start(IOService *provider)
{
    IOLog("%s(%p)::start\n", getName(), this);
    bool result = super::start(provider);
    
    do {
        if (!result) {
            IOLog("%s(%p)::start -> super return false", getName(), this);
            break;
        }
        
        IOUSBInterface *interface = OSDynamicCast(IOUSBInterface, provider);
        IOUSBFindEndpointRequest request = {};
        IOUSBPipe *pipe = NULL;
        
        if (interface == NULL) {
            IOLog("%s(%p)::start -> provider not a IOUSBInterface\n", getName(), this);
            result = false;
            break;
        }
        
        IOLog("%s(%p)::start -> finding endpoints", getName(), this);
        
        request.type = kUSBBulk;
        request.direction = kUSBIn; // DEVICE->HOST
        pipe = interface->FindNextPipe(pipe, &request);
        if (pipe) {
            logEndpoint(pipe);
            pipe->release();
        }
        
        request.type = kUSBBulk;
        request.direction = kUSBOut; // HOST->DEVICE
        pipe = interface->FindNextPipe(pipe, &request);
        if (pipe) {
            logEndpoint(pipe);
            pipe->release();
        }
        
        IOLog("%s(%p)::start -> end finding endpoints", getName(), this);
    } while (false);
    
    return result;
}

void com_ryan_test_IOKitDC122Driver::stop(IOService *provider)
{
    IOLog("%s(%p)::stop\n", getName(), this);
    super::stop(provider);
}

bool com_ryan_test_IOKitDC122Driver::terminate(IOOptionBits options)
{
    IOLog("%s(%p)::terminate\n", getName(), this);
    bool result = super::terminate(options);
    return result;
}
