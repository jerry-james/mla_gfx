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

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "virtual_screen.h"
#include "gfx/gfx_colors_x11.h"
#include "gfx/gfx_colors.h"
#include "driver/gfx/drv_gfx_display.h"
#include "gfx/gfx_primitive.h"
#include <unistd.h>
#include <gfx/gfx_types_resource.h>
#include "header.h"

struct TEST_SCREEN{
    uint16_t width;
    uint16_t height;
    char name[16];
};

static struct TEST_SCREEN screens[] = {
    {DISP_HOR_RESOLUTION, DISP_VER_RESOLUTION}
};

static struct TEST_SCREEN *screen = NULL;

bool renderString(char *test_name, char *text, const GFX_RESOURCE_HDR *ptr);

void drawBounds(const GFX_RESOURCE_HDR *ptr, int x, int y);

char * generateString();

/*******************************************************************************
 * Tests 
 ******************************************************************************/



void drawBounds(const GFX_RESOURCE_HDR *ptr, int x, int y) {
    uint16_t height = ptr->resource.font.header.height;
    GFX_LineStyleSet(GFX_LINE_STYLE_THIN_SOLID);
    GFX_ColorSet(GFX_X11_RED);
    GFX_LineDraw(x, y, x, y + height);
    int w = DISP_HOR_RESOLUTION;
    GFX_LineDraw(x, y, w, y);
    GFX_LineDraw(x, y + height, x + w, y + height);
    GFX_ColorSet(GFX_X11_BLACK);
}

bool renderString(char *test_name, char *text, const GFX_RESOURCE_HDR *ptr) {

    GFX_ColorSet(GFX_X11_WHITE);
    GFX_ScreenClear();
    GFX_ColorSet(GFX_X11_BLACK);

    GFX_FontSet((GFX_RESOURCE_HDR *) ptr);

    GFX_TextAreaTopSet(0);
    GFX_TextAreaBottomSet(GFX_MaxYGet());
    GFX_TextAreaLeftSet(0);
    GFX_TextAreaRightSet(GFX_MaxXGet());

    int x = 1;
    int y = 1;
    drawBounds(ptr, x, y);

    GFX_STATUS status = GFX_TextStringDraw(x, y, text, 0);

    if (status != GFX_STATUS_SUCCESS) { return false; }

    return VirtualScreen_ValidateScreen(test_name);
}



/* ===========================================================================*/
/* End of tests, Start of test framework                                      */
/* ===========================================================================*/        
typedef bool (*TEST_FUNCTION)(void);

/* Add test function here for it to be added to the test suite. */
extern TEST_FUNCTION tests[];
extern const uint32_t test_count;

static int RunTests(struct TEST_SCREEN *screen){
    int test_index;
    uint32_t passed_count;
    bool test_passed;

    passed_count = 0;
    for(test_index=0; test_index < test_count; test_index++){
        test_passed = tests[test_index]();
        
        if(test_passed == true){
            passed_count++;
        } else {
            printf("TEST FAILED(%i)\r\n", test_index);
        }
    }
    
    return passed_count;
}

void RunPrimitiveTests(void)
{
    int passed = 0;
    uint8_t i;
    
    for(i=0;i<sizeof(screens)/sizeof(struct TEST_SCREEN); i++){
        passed = RunTests(&screens[i]);

        printf( "Primitive tests, screen(%i,%i): run: %i, passed: %i, failed: %i\r\n", screens[i].width, screens[i].height, test_count, passed, test_count-passed);
    }
}


