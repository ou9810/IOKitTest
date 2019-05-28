/* add your code here */

#include <IOKit/IOService.h>
class com_ryan_test_IOKitDriver : public IOService
{
    OSDeclareDefaultStructors(com_ryan_test_IOKitDriver)
public:
    virtual bool init(OSDictionary *dictionary = 0) override;
    virtual void free(void) override;
    virtual IOService *probe(IOService *provider, SInt32 *score) override;
    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;
};
