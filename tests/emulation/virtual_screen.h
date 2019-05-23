#ifndef VIRTUAL_SCREEN_H
#define VIRTUAL_SCREEN_H

#include <stdint.h>
#include <stdbool.h>

bool VirtualScreen_ValidateScreen(const char* test_name);
void VirtualScreen_Dump(const char* filename);

#endif /* VIRTUAL_SCREEN_H */

