//
//  main.cpp
//  IOKitProbe
//
//  Created by Ryan on 2019/5/22.
//  Copyright © 2019 ryan.test. All rights reserved.
//

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOMessage.h>

typedef struct {
    io_service_t service;
    io_object_t notification;
} DeviceData;

void writeMatchingDictionary(CFMutableDictionaryRef *dict);
void findUSBDevices(CFDictionaryRef dict);
void registerMatchingNotification(CFDictionaryRef dict);

void DeviceAdded(void *refCon, io_iterator_t iter);
void DeviceRemoved(void *refCon, io_service_t service, natural_t messageType, void* messageArgument);

IONotificationPortRef   gNotificationPort = NULL;

int main(int argc, const char * argv[]) {
    CFMutableDictionaryRef matchingDict = IOServiceMatching("IOUSBDevice");
    writeMatchingDictionary(&matchingDict);
    
    registerMatchingNotification(matchingDict);
    //findUSBDevices(matchingDict);
    
    return 0;
}

void writeMatchingDictionary(CFMutableDictionaryRef *dict) {
    int vendorId = 0x1d4d;
    int productId = 0x6003;
    
    CFNumberRef vendorIdRef = CFNumberCreate(NULL, kCFNumberIntType, &vendorId);
    CFNumberRef productIdRef = CFNumberCreate(NULL, kCFNumberIntType, &productId);
    
    CFDictionaryAddValue(*dict, CFSTR("idVendor"), vendorIdRef);
    CFDictionaryAddValue(*dict, CFSTR("idProduct"), productIdRef);
    
    CFRelease(vendorIdRef);
    CFRelease(productIdRef);
}

void findUSBDevices(CFDictionaryRef dict) {
    io_iterator_t iter;
    io_service_t service;
    
    kern_return_t kr = IOServiceGetMatchingServices(kIOMasterPortDefault, dict, &iter);
    
    if (kr != KERN_SUCCESS)
        return;

    
    while ((service = IOIteratorNext(iter)) != 0) {
        CFStringRef className;
        io_name_t name;
        
        className = IOObjectCopyClass(service);
        if (CFEqual(className, CFSTR("IOUSBDevice"))) {
            CFTypeRef venderName = IORegistryEntryCreateCFProperty(service, CFSTR("USB Vendor Name"), NULL, 0);
            CFShow(venderName);
            
            IORegistryEntryGetName(service, name);
            printf("Found device: %s\n", name);
        }
        
        CFRelease(className);
        
        DeviceData *data = (DeviceData *)malloc(sizeof(DeviceData));
        data->service = service;
        IOServiceAddInterestNotification(gNotificationPort, service, kIOGeneralInterest, DeviceRemoved, data, &data->notification);
        
        IOObjectRelease(service);
    }
}

void registerMatchingNotification(CFDictionaryRef matchingDict) {
    io_iterator_t iter;
    CFRunLoopSourceRef runLoopSource;
    kern_return_t kr;
    
    gNotificationPort = IONotificationPortCreate(kIOMasterPortDefault);
    runLoopSource = IONotificationPortGetRunLoopSource(gNotificationPort);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopDefaultMode);
    
    kr = IOServiceAddMatchingNotification(gNotificationPort, kIOFirstMatchNotification, matchingDict, DeviceAdded, NULL, &iter);
    DeviceAdded(NULL, iter);
    
//    CFRunLoopRun();
    
    IONotificationPortDestroy(gNotificationPort);
    IOObjectRelease(iter);
}

void DeviceAdded(void *refCon, io_iterator_t iter) {
    task_port_t owningTask = mach_task_self();
    uint32_t type = 0;
    io_connect_t driverConnection;
    kern_return_t kr = IOServiceOpen(iter, owningTask, type, &driverConnection);
    
    if (kr != KERN_SUCCESS)
        return;
        
        
}

void DeviceRemoved(void *refCon, io_service_t service, natural_t messageType, void* messageArgument) {
    DeviceData *data = (DeviceData *)refCon;
    
    if (messageType == kIOMessageServiceIsTerminated) {
        io_name_t name;
        IORegistryEntryGetName(service, name);
        printf("Device removed: %s\n", name);
        
        IOObjectRelease(data->notification);
        IOObjectRelease(data->service);
        
        free(data);
    }
}
