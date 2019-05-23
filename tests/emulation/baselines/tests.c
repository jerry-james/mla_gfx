//
// Created by C14648 on 5/13/2019.
//

#include <stdlib.h>
#include "test_function.h"
#include "header.h"

char* generateString() {
    char* chars = malloc(sizeof(char) * 256);

    for (int i = 32; i < 256; i++) {
        chars[i - 32] = i;
    }
    chars[255] = 0;
    return chars;
}

static bool DINv4_26_2BPP_test(void) {
    return renderString("DINv4_26_2BPP", generateString(), &DINv4_26_2BPP) ;
}


TEST_FUNCTION tests[] = {
    &DINv4_26_2BPP_test,
};

const uint32_t test_count = (sizeof(tests)/sizeof(TEST_FUNCTION));