//
// Created by C14648 on 5/13/2019.
//

#ifndef MLA_GFX_TEST_FUNCTION_H
#define MLA_GFX_TEST_FUNCTION_H
#include <stdbool.h>
#include <gfx/gfx_types_resource.h>

typedef bool (*TEST_FUNCTION)(void);
bool renderString(char *test_name, char *text, const GFX_RESOURCE_HDR *ptr);
#endif //MLA_GFX_TEST_FUNCTION_H
