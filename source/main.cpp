#include <nds.h>
#include <stdio.h>
#include "draw.h"

int main(void) {

    vramInit();

    printf_zh(TOP,"test����һ��");
    printf_zh(BUTTON,"test��������");
	while(1) {
        swiWaitForVBlank();
    }
	return 0;
}

