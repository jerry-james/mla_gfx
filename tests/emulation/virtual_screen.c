/*******************************************************************************
Copyright 2018 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "system_config.h"
#include "gfx/gfx_primitive.h"

#if (GFX_CONFIG_COLOR_DEPTH == 8)
#define BUFFER_UNIT_SIZE sizeof(uint8_t)
static uint8_t screen_data[DISP_HOR_RESOLUTION][DISP_VER_RESOLUTION] = {0};
#error BMP output not currently supported for 8bpp
#elif (GFX_CONFIG_COLOR_DEPTH == 16)
#define BUFFER_UNIT_SIZE sizeof(uint16_t)
static uint16_t screen_data[DISP_HOR_RESOLUTION][DISP_VER_RESOLUTION] = {0};
#define RED_MASK    0x0000F800
#define GREEN_MASK  0x000007E0
#define BLUE_MASK   0x0000001F
#elif (GFX_CONFIG_COLOR_DEPTH == 24)
#define BUFFER_UNIT_SIZE sizeof(uint32_t)
static uint32_t screen_data[DISP_HOR_RESOLUTION][DISP_VER_RESOLUTION] = {0};
#define RED_MASK    0x00FF0000
#define GREEN_MASK  0x0000FF00
#define BLUE_MASK   0x000000FF
#endif

static uint16_t width = DISP_HOR_RESOLUTION;
static uint16_t height = DISP_VER_RESOLUTION;
static char* name = NULL;

static const uint8_t bytes_per_pixel = GFX_CONFIG_COLOR_DEPTH/8;

struct __attribute__((packed)) BMP_FILE_HEADER{
    uint16_t id;
    uint32_t size;
    uint16_t reserved0;
    uint16_t reserved1;
    uint32_t image_data_offset;
};

struct __attribute__((packed)) BITMAPINFOHEADER{
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t color_planes;
    uint16_t bits_per_pixel;
    uint32_t compression_method;
    uint32_t image_size;
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    uint32_t colors_in_pallet;
    uint32_t important_colors;
};

struct __attribute__((packed)) BITMAPV4HEADER{
    uint32_t header_size;
    uint32_t width;
    uint32_t height;
    uint16_t color_planes;
    uint16_t bits_per_pixel;
    uint32_t compression_method;
    uint32_t image_size;
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    uint32_t colors_in_pallet;
    uint32_t important_colors;
    uint32_t red_mask;
    uint32_t green_mask;
    uint32_t blue_mask;
    uint32_t alpha_mask;
    uint32_t cs_type;
    uint8_t endpoints[36];
    uint32_t red_gamma;
    uint32_t green_gamma;
    uint32_t blue_gamma;
};

static int bytesNeededTo32bitAlignRow(void){
    return (4-((bytes_per_pixel*width)%4))%4;
}

void VirtualScreen_Dump(const char* filename)
{
    int x, y;
    uint32_t pixel;
    
    if(screen_data == NULL) { return; }
 
    const uint32_t pixel_data_size = ((width*bytes_per_pixel)+bytesNeededTo32bitAlignRow())*height;
    
    const struct BITMAPV4HEADER bitmap_core_header = {
        .header_size = 108,
        .width = width,
        .height = height, 
        .color_planes = 1,
        .bits_per_pixel = GFX_CONFIG_COLOR_DEPTH,
        .vertical_resolution = 3780,
        .horizontal_resolution = 3780,
        .compression_method = 3,
        .red_mask = RED_MASK,
        .green_mask = GREEN_MASK,
        .blue_mask = BLUE_MASK,
        .cs_type = 0x206E6957,
    };
    
    const struct BMP_FILE_HEADER file_header = {
        .id = 0x4D42, 
        .size = sizeof(struct BITMAPV4HEADER)+pixel_data_size, 
        .image_data_offset = sizeof(struct BITMAPV4HEADER) + sizeof(struct BMP_FILE_HEADER),
        .reserved0 = 0,
        .reserved1 = 0
    };
    
    const uint32_t padding = 0x00000000;
    
    FILE *fp;
      
    fp = fopen(filename, "w+");
    
    if(fp == NULL) { printf("DUMP FAILED: Unable to open file (%s) for writing.  Check that folder is not read only.\r\n", filename); return;}
    
    fwrite(&file_header, 1, sizeof(file_header), fp);
    fwrite(&bitmap_core_header, 1, sizeof(bitmap_core_header), fp);
   
    for(y=height-1; y>=0; y--){
        for(x=0; x<width; x++){
            pixel = screen_data[x][y];
            fwrite(&pixel, 1, bytes_per_pixel, fp);
        }
        fwrite(&padding, 1, bytesNeededTo32bitAlignRow(), fp);
    }
    fclose(fp);
}

bool VirtualScreen_ValidateScreen(const char* test_name)
{
    char results_filename[80];
    char expected_filename[80];
    FILE *results_file;
    FILE *expected_file;
    
    sprintf(results_filename, "test_results\\%ix%i_%ibpp_%s.bmp\0", width, height, GFX_CONFIG_COLOR_DEPTH, test_name);
    sprintf(expected_filename, "expected_results\\%ix%i_%ibpp_%s.bmp\0", width, height, GFX_CONFIG_COLOR_DEPTH, test_name);

    VirtualScreen_Dump(results_filename);
        
    results_file = fopen(results_filename, "r");
    expected_file = fopen(expected_filename, "r");

    if(results_file == NULL){ printf("VALIDATION FAILED(%s): unable to create results file\r\n", test_name); return false; }
    if(expected_file == NULL){ printf("VALIDATION FAILED(%s): unable to open expected results file\r\n", test_name); return false; }
        
    while(!feof(results_file)){
        if(fgetc(results_file) != fgetc(expected_file)){
            fclose(results_file);
            fclose(expected_file);
            return false;
        }
    }
    
    if(!feof(expected_file)){
        return false;
    }
    
    return true;
}

void DRV_GFX_Initialize(void)
{
    
}

GFX_STATUS_BIT GFX_RenderStatusGet(void)
{
    return GFX_STATUS_READY_BIT;
}

GFX_STATUS GFX_PixelPut(uint16_t x, uint16_t y)
{
    if(screen_data == NULL) { return GFX_STATUS_FAILURE; }
    
    screen_data[x][y] = GFX_ColorGet();
    
    return (GFX_STATUS_SUCCESS);
}

GFX_COLOR GFX_PixelGet(uint16_t x, uint16_t y)
{
    if(screen_data == NULL) { return (GFX_COLOR)0xFFFFFF; }
    
    return ((GFX_COLOR) screen_data[x][y]);
}
