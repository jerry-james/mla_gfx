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

#include <stdbool.h>
#include <stdio.h>
#include "virtual_screen.h"
#include "gfx/gfx_colors_x11.h"
#include "gfx/gfx_colors.h"
#include "driver/gfx/drv_gfx_display.h"
#include "gfx/gfx_primitive.h"
#include <unistd.h>

#include "test_font_1.h"

struct TEST_SCREEN{
    uint16_t width;
    uint16_t height;
    char name[16];
};

static struct TEST_SCREEN screens[] = {
    {DISP_HOR_RESOLUTION, DISP_VER_RESOLUTION}
};

static struct TEST_SCREEN *screen = NULL;

/*******************************************************************************
 * Tests 
 ******************************************************************************/

static bool ClearScreen(void)
{    
    const char test_name[] = "ClearScreen";
    
    GFX_ColorSet(GFX_X11_WHITE);
    GFX_ScreenClear();
        
    return VirtualScreen_ValidateScreen(test_name);
}

static bool PutPixel(void)
{
    const char test_name[] = "PutPixel";
    
    GFX_ColorSet(GFX_X11_WHITE);
    GFX_ScreenClear();
    GFX_ColorSet(GFX_X11_RED);
    GFX_PixelPut(1,1);
    
    return VirtualScreen_ValidateScreen(test_name);
}

static bool DrawLine1(void)
{
    const char test_name[] = "DrawLine1";
    
    GFX_ColorSet(GFX_X11_WHITE);
    GFX_ScreenClear();
    GFX_ColorSet(GFX_X11_BLUE);
    GFX_LineDraw(0, 0, GFX_MaxXGet(), 0);
    GFX_ColorSet(GFX_X11_RED);
    GFX_LineDraw(0, GFX_MaxYGet(), GFX_MaxXGet(), GFX_MaxYGet());
    GFX_ColorSet(GFX_X11_YELLOW);
    GFX_LineDraw(0, 0, 0, GFX_MaxYGet());
    GFX_ColorSet(GFX_X11_GREEN);
    GFX_LineDraw(GFX_MaxXGet(), 0, GFX_MaxXGet(), GFX_MaxYGet());
    
    return VirtualScreen_ValidateScreen(test_name);
}

static bool GradientFilledRectangle(void)
{
    const char test_name[] = "GradientFilledRectangle";
    
    GFX_ColorSet(GFX_X11_WHITE);
    GFX_ScreenClear();
    
    GFX_FillStyleSet(GFX_FILL_STYLE_GRADIENT_DOWN);
    GFX_GradientColorSet(WHITE, RED);
    GFX_RectangleFillDraw(0, 0, GFX_MaxXGet(), GFX_MaxYGet());
    
    return VirtualScreen_ValidateScreen(test_name);
}

static bool Colors(void)
{
    const char test_name[] = "Colors";
    const GFX_COLOR colors[] = {
        BLACK,
        BRIGHTBLUE,
        BRIGHTGREEN,
        BRIGHTCYAN,
        BRIGHTRED,
        BRIGHTMAGENTA,
        YELLOW,
        BRIGHTYELLOW,
        LIGHTYELLOW,
        GOLD,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHTGRAY,
        DARKGRAY,
        LIGHTBLUE,
        LIGHTGREEN,
        LIGHTCYAN,
        LIGHTRED,
        LIGHTMAGENTA,
        WHITE,
        SADDLEBROWN,
        SIENNA,
        PERU,
        BURLYWOOD,
        WHEAT,
        TAN,
        ORANGE,
        DARKORANGE,
        LIGHTORANGE,
        GRAY242,
        GRAY229,
        GRAY224,
        GRAY204,
        GRAY192,
        GRAY160,
        GRAY128,
        GRAY096,
        GRAY032,
        GRAY010,
        GRAY95,
        GRAY90,
        GRAY0,
	GRAY80,	
        GRAY1,	
        GRAY2,	
	GRAY3,
	GRAY4,
        GRAY5,	
        GRAY6,
    };
    unsigned int i;
    
    GFX_ColorSet(WHITE);
    GFX_ScreenClear();
    
    for(i=0; i<sizeof(colors)/sizeof(GFX_COLOR); i++){
        GFX_ColorSet(colors[i]);
        GFX_LineDraw(i, 0, i, GFX_MaxYGet());
    }
    
    return VirtualScreen_ValidateScreen(test_name);    
}

bool ColorsX11(void)
{
    const char test_name[] = "ColorsX11";
    const GFX_COLOR colors[] = {
        /*********************************************************************
         * X11: Pink Colors
         *********************************************************************/
        GFX_X11_PINK,
        GFX_X11_LIGHT_PINK,
        GFX_X11_HOT_PINK,
        GFX_X11_DEEP_PINK,
        GFX_X11_PALE_VIOLET_RED,
        GFX_X11_MEDIUM_VIOLET_RED,
        /*********************************************************************
         * X11: Red Colors
         *********************************************************************/
        GFX_X11_LIGHT_SALMON,
        GFX_X11_SALMON,
        GFX_X11_DARK_SALMON,
        GFX_X11_LIGHT_CAROL,
        GFX_X11_INDIAN_RED,
        GFX_X11_CRIMSON,
        GFX_X11_FIRE_BRICK,
        GFX_X11_DARK_RED,
        GFX_X11_RED,
        /*********************************************************************
         * X11: Orange Colors
         *********************************************************************/
        GFX_X11_ORANGE_RED,
        GFX_X11_TOMATO,
        GFX_X11_CORAL,
        GFX_X11_DARK_ORANGE,
        GFX_X11_ORANGE,
        GFX_X11_GOLD,
        /*********************************************************************
         * X11: Yellow Colors
         *********************************************************************/
        GFX_X11_YELLOW,
        GFX_X11_LIGHT_YELLOW,
        GFX_X11_LEMON_CHIFFON,
        GFX_X11_LIGHT_GOLDENROD_YELLOW,
        GFX_X11_PAPAYA_WHIP,
        GFX_X11_MOCCASIN,
        GFX_X11_PEACH_PUFF,
        GFX_X11_PALE_GOLDENROD,
        GFX_X11_KHAKI,
        GFX_X11_DARK_KHAKI,
        /*********************************************************************
         * X11: Brown Colors
         *********************************************************************/
        GFX_X11_CORNSILK,
        GFX_X11_BLANCHED_ALMOND,
        GFX_X11_BISQUE,
        GFX_X11_NAVAJO_WHITE,
        GFX_X11_WHEAT,
        GFX_X11_BURLY_WOOD,
        GFX_X11_TAN,
        GFX_X11_ROSY_BROWN,
        GFX_X11_SANDY_BROWN,
        GFX_X11_GOLDENROD,
        GFX_X11_DARK_GOLDENROD,
        GFX_X11_PERU,
        GFX_X11_CHOCOLATE,
        GFX_X11_SADDLE_BROWN,
        GFX_X11_SIENNA,
        GFX_X11_BROWN,
        GFX_X11_MARRON,
        /*********************************************************************
         * X11: Green Colors
         *********************************************************************/
        GFX_X11_DARK_OLIVE_GREEN,
        GFX_X11_OLIVE,
        GFX_X11_OLIVE_DRAB,
        GFX_X11_YELLOW_GREEN,
        GFX_X11_LIME_GREEN,
        GFX_X11_LIME,
        GFX_X11_LAWN_GREEN,
        GFX_X11_CHARTEUSE,
        GFX_X11_GREEN_YELLOW,
        GFX_X11_SPRING_GREEN,
        GFX_X11_MEDIUM_SPRING_GREEN,
        GFX_X11_LIGHT_GREEN,
        GFX_X11_PALE_GREEN,
        GFX_X11_DARK_SEA_GREEN,
        GFX_X11_MEDIUM_SEA_GREEN,
        GFX_X11_SEA_GREEN,
        GFX_X11_FOREST_GREEN,
        GFX_X11_GREEN,
        GFX_X11_DARK_GREEN,
        /*********************************************************************
         * X11: CYAN COLORS
         *********************************************************************/
        GFX_X11_MEDIUM_AQUAMARINE,
        GFX_X11_AQUA,
        GFX_X11_CYAN,
        GFX_X11_LIGHT_CYAN,
        GFX_X11_PALE_TURQUOISE,
        GFX_X11_AQUAMARINE,
        GFX_X11_TURQUOISE,
        GFX_X11_MEDIUM_TURQUOISE,
        GFX_X11_DARK_TURQUOISE,
        GFX_X11_LIGHT_SEA_GREEN,
        GFX_X11_CADEL_BLUE,
        GFX_X11_DARK_CYAN,
        GFX_X11_TEAL,
        /*********************************************************************
         * X11: BLUE COLORS
         *********************************************************************/
        GFX_X11_LIGHT_STEEL_BLUE,
        GFX_X11_POWDER_BLUE,
        GFX_X11_LIGHT_BLUE,
        GFX_X11_SKY_BLUE,
        GFX_X11_LIGHT_SKY_BLUE,
        GFX_X11_DEEP_SKY_BLUE,
        GFX_X11_DODGER_BLUE,
        GFX_X11_CORNFLOWER_BLUE,
        GFX_X11_STEEL_BLUE,
        GFX_X11_ROYAL_BLUE,
        GFX_X11_BLUE,
        GFX_X11_MEDIUM_BLUE,
        GFX_X11_DARK_BLUE,
        GFX_X11_NAVY,
        GFX_X11_MIDNIGHT_BLUE,
        /*********************************************************************
         * X11: PURPLE COLORS
         *********************************************************************/
        GFX_X11_LAVENDER,
        GFX_X11_THISTLE,
        GFX_X11_PLUM,
        GFX_X11_VIOLET,
        GFX_X11_ORCHID,
        GFX_X11_FUCHSIA,
        GFX_X11_MAGENTA,
        GFX_X11_MEDIUM_ORCHID,
        GFX_X11_MEDIUM_PURPLE,
        GFX_X11_BLUE_VIOLET,
        GFX_X11_DARK_VIOLET,
        GFX_X11_DARK_ORCHID,
        GFX_X11_DARK_MAGENTA,
        GFX_X11_PURPLE,
        GFX_X11_INDIGO,
        GFX_X11_DARK_SLATE_BLUE,
        GFX_X11_SLATE_BLUE,
        GFX_X11_MEDIUM_SLATE_BLUE,
        /*********************************************************************
         * X11: WHITE/GRAY/BLACK COLORS
         *********************************************************************/
        GFX_X11_WHITE,
        GFX_X11_SNOW,
        GFX_X11_HONEYDEW,
        GFX_X11_MINT_CREAM,
        GFX_X11_AZURE,
        GFX_X11_ALICE_BLUE,
        GFX_X11_GHOST_WHITE,
        GFX_X11_WHITE_SMOKE,
        GFX_X11_SEASHELL,
        GFX_X11_BEIGE,
        GFX_X11_OLD_LACE,
        GFX_X11_FLORAL_WHITE,
        GFX_X11_IVORY,
        GFX_X11_ANTIQUE_WHITE,
        GFX_X11_LINEN,
        GFX_X11_LAVENDOR_BLUSH,
        GFX_X11_MISTY_ROSE,
        GFX_X11_GAINSBORO,
        GFX_X11_LIGHT_GRAY,
        GFX_X11_SILVER,
        GFX_X11_DARK_GREY,
        GFX_X11_GREY,
        GFX_X11_DIM_GREY,
        GFX_X11_LIGHT_SLATE_GREY,
        GFX_X11_SLATE_GREY,
        GFX_X11_DARK_SLATE_GREY,
        GFX_X11_BLACK,
    };
    unsigned int i;
    
    GFX_ColorSet(WHITE);
    GFX_ScreenClear();
    
    for(i=0; i<sizeof(colors)/sizeof(GFX_COLOR); i++){
        GFX_ColorSet(colors[i]);
        GFX_LineDraw(i, 0, i, GFX_MaxYGet());
    }
    
    return VirtualScreen_ValidateScreen(test_name);   
}

static bool LineStyleSetVertical(void)
{
    const char test_name[] = "LineStyleSetVertical";
    const GFX_LINE_STYLE line_styles[] = {
        GFX_LINE_STYLE_THIN_SOLID,
        GFX_LINE_STYLE_THIN_DOTTED,
        GFX_LINE_STYLE_THIN_DASHED,
        GFX_LINE_STYLE_THICK_SOLID,
        GFX_LINE_STYLE_THICK_DOTTED,
        GFX_LINE_STYLE_THICK_DASHED
    };
    unsigned int i;
    
    GFX_ColorSet(GFX_X11_WHITE);
    GFX_ScreenClear();
    GFX_ColorSet(GFX_X11_BLACK);
    
    for(i=0; i<sizeof(line_styles)/sizeof(GFX_LINE_STYLE); i++){
        GFX_LineStyleSet(line_styles[i]);
        GFX_LineDraw(i*4, 0, i*4, GFX_MaxYGet());
    }

    return VirtualScreen_ValidateScreen(test_name);
}

static bool LineStyleSetHorizontal(void)
{
    const char test_name[] = "LineStyleSetHorizontal";
    const GFX_LINE_STYLE line_styles[] = {
        GFX_LINE_STYLE_THIN_SOLID,
        GFX_LINE_STYLE_THIN_DOTTED,
        GFX_LINE_STYLE_THIN_DASHED,
        GFX_LINE_STYLE_THICK_SOLID,
        GFX_LINE_STYLE_THICK_DOTTED,
        GFX_LINE_STYLE_THICK_DASHED
    };
    unsigned int i;
    
    GFX_ColorSet(GFX_X11_WHITE);
    GFX_ScreenClear();
    GFX_ColorSet(GFX_X11_BLACK);
    
    for(i=0; i<sizeof(line_styles)/sizeof(GFX_LINE_STYLE); i++){
        GFX_LineStyleSet(line_styles[i]);
        GFX_LineDraw(0, i*4,GFX_MaxXGet(),  i*4);
    }

    return VirtualScreen_ValidateScreen(test_name);
}

static bool LineStyleSetDiagonal(void)
{
    const char test_name[] = "LineStyleSetDiagonal";
    const GFX_LINE_STYLE line_styles[] = {
        GFX_LINE_STYLE_THIN_SOLID,
        GFX_LINE_STYLE_THIN_DOTTED,
        GFX_LINE_STYLE_THIN_DASHED,
        GFX_LINE_STYLE_THICK_SOLID,
        GFX_LINE_STYLE_THICK_DOTTED,
        GFX_LINE_STYLE_THICK_DASHED
    };
    unsigned int i;
    
    GFX_ColorSet(GFX_X11_WHITE);
    GFX_ScreenClear();
    GFX_ColorSet(GFX_X11_BLACK);
    
    for(i=0; i<sizeof(line_styles)/sizeof(GFX_LINE_STYLE); i++){
        GFX_LineStyleSet(line_styles[i]);
        GFX_LineDraw(i*6, 0, GFX_MaxXGet(), GFX_MaxYGet() - (i*6));
    }

    return VirtualScreen_ValidateScreen(test_name);
}

static bool TextCharDraw1(void)
{
    const char test_name[] = "TextCharDraw1";
    
    GFX_ColorSet(GFX_X11_WHITE);
    GFX_ScreenClear();
    GFX_ColorSet(GFX_X11_BLACK);
    
    GFX_FontSet((GFX_RESOURCE_HDR*)&test_font_1);
    GFX_TextCursorPositionSet(1, 1);
    
    GFX_TextAreaTopSet(0);
    GFX_TextAreaBottomSet(GFX_MaxYGet());
    GFX_TextAreaLeftSet(0);
    GFX_TextAreaRightSet(GFX_MaxXGet());
                
    if(GFX_TextCharDraw( '1' ) != GFX_STATUS_SUCCESS) { return false; }

    return VirtualScreen_ValidateScreen(test_name);
}

static bool TextCharDraw2(void)
{
    const char test_name[] = "TextCharDraw2";
    
    GFX_ColorSet(GFX_X11_WHITE);
    GFX_ScreenClear();
    GFX_ColorSet(GFX_X11_BLACK);
    
    GFX_FontSet((GFX_RESOURCE_HDR*)&test_font_1);
    GFX_TextCursorPositionSet(1, 1);
    
    GFX_TextAreaTopSet(0);
    GFX_TextAreaBottomSet(GFX_MaxYGet());
    GFX_TextAreaLeftSet(0);
    GFX_TextAreaRightSet(GFX_MaxXGet());
                
    /* '~' should be an out of bounds character. */
    if(GFX_TextCharDraw( '~' ) != GFX_STATUS_ERROR) { return false; }

    return VirtualScreen_ValidateScreen(test_name);
}

static bool RectangleRoundFillDraw1(void)
{
    const char test_name[] = "RectangleRoundFillDraw1";
    
    GFX_ColorSet(GFX_X11_WHITE);
    GFX_ScreenClear();
    
    GFX_FillStyleSet(GFX_FILL_STYLE_GRADIENT_DOWN);
    GFX_GradientColorSet(GFX_X11_RED, GFX_X11_BLUE);
    GFX_RectangleRoundFillDraw(     0, //left
                                    0, //top
                                    GFX_MaxXGet(), //right
                                    GFX_MaxYGet(), //bottom
                                    0 //radius
                              ); 

    return VirtualScreen_ValidateScreen(test_name);
}

static bool RectangleRoundFillDraw2(void)
{
    const char test_name[] = "RectangleRoundFillDraw2";
    uint16_t radius = (GFX_MaxXGet() > GFX_MaxYGet()) ? (GFX_MaxYGet()/8) : (GFX_MaxXGet()/8);  //radius - 1/8 of the smaller side of the screen.
    
    GFX_ColorSet(GFX_X11_WHITE);
    GFX_ScreenClear();
    
    GFX_FillStyleSet(GFX_FILL_STYLE_GRADIENT_RIGHT);
    GFX_GradientColorSet(GFX_X11_RED, GFX_X11_BLUE);
    GFX_RectangleRoundFillDraw(     0 + radius, //left
                                    0 + radius, //top
                                    GFX_MaxXGet() - radius, //right
                                    GFX_MaxYGet() - radius, //bottom
                                    radius
                              ); 

    return VirtualScreen_ValidateScreen(test_name);
}


GFX_STATUS GFX_ExternalResourceCallback( GFX_RESOURCE_HDR *pResource, uint32_t offset, uint16_t nCount, void *pBuffer)
{
    FILE *resource_file;
        
    if(pResource == &test_font_1)
    {
        resource_file = fopen("test_font_1.bin", "r+b");
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

/* ===========================================================================*/
/* End of tests, Start of test framework                                      */
/* ===========================================================================*/        
typedef bool (*TEST_FUNCTION)(void);

/* Add test function here for it to be added to the test suite. */
static TEST_FUNCTION tests[]={
    &ClearScreen,
    &PutPixel,
    &DrawLine1,
    &Colors,
    &ColorsX11,
    &LineStyleSetVertical,
    &LineStyleSetHorizontal,
    &LineStyleSetDiagonal,
    &GradientFilledRectangle,
    &TextCharDraw1,
    &TextCharDraw2,
    &RectangleRoundFillDraw1,
    &RectangleRoundFillDraw2
};

static const uint32_t test_count = (sizeof(tests)/sizeof(TEST_FUNCTION));

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

