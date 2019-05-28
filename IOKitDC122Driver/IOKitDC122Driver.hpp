/* add your code here */

#include <IOKit/usb/IOUSBDevice.h>

class com_ryan_test_IOKitDC122Driver : public IOService
{
    OSDeclareDefaultStructors(com_ryan_test_IOKitDC122Driver)
    
public:
    virtual bool init(OSDictionary *dictionary = 0) override;
    virtual void free() override;
    virtual IOService *probe(IOService *provider, SInt32 *score) override;
    
    virtual bool attach(IOService *provider) override;
    virtual void detach(IOService *provider) override;
    
    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;
    
    virtual bool terminate(IOOptionBits options = 0);
};
