//
//  IOKitTestKext.c
//  IOKitTestKext
//
//  Created by Ryan on 2019/5/22.
//  Copyright © 2019 ryan.test. All rights reserved.
//

#include <mach/mach_types.h>

kern_return_t IOKitTestKext_start(kmod_info_t * ki, void *d);
kern_return_t IOKitTestKext_stop(kmod_info_t *ki, void *d);

kern_return_t IOKitTestKext_start(kmod_info_t * ki, void *d)
{
    printf("Hello IOKitTestKext\n");
    return KERN_SUCCESS;
}

kern_return_t IOKitTestKext_stop(kmod_info_t *ki, void *d)
{
    printf("Goodbye IOKitTestKext\n");
    return KERN_SUCCESS;
}
