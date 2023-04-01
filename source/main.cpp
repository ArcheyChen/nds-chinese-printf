#include <nds.h>
#include <stdio.h>
#include "draw.h"

int main(void) {

    vramInit();

    printf_zh(TOP,"test测试一下");
    printf_zh(BUTTON,"test测试两下");
	while(1) {
        swiWaitForVBlank();
    }
	return 0;
}

