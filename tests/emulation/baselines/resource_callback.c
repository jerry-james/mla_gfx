#include <gfx/gfx_types_resource.h>
#include <stdio.h>
#include <test_font_1.h>
#include "header.h"

GFX_STATUS GFX_ExternalResourceCallback(GFX_RESOURCE_HDR *pResource, uint32_t offset, uint16_t nCount, void *pBuffer)
{
    FILE *resource_file;
        
    if(false) {
        
    }
        else if(pResource == &DINv4_26_2BPP) {
        resource_file = fopen("./baselines/DINv4_26_2BPP.bin", "r");
    }
    else
    {
        return GFX_STATUS_FAILURE;
    }
     
    if(resource_file == NULL){ return GFX_STATUS_FAILURE; }
    if(fseek(resource_file, offset, SEEK_SET) != 0) { return GFX_STATUS_FAILURE; }
    if(fread(pBuffer, 1, nCount, resource_file) != nCount ) { return GFX_STATUS_FAILURE; } 

    fclose(resource_file);
    return GFX_STATUS_SUCCESS;
}
