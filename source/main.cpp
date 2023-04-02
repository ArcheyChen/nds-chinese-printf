#include <nds.h>
#include <stdio.h>
#include "printf_utf8.h"

int main(void) {

    vramInit();

	fbInit();
    char txt[] = "陋室铭\n"
"唐·刘禹锡\n"
"山不在注1高，有仙则名注2。水不在深，有龙则灵注3。斯是陋室注4，惟吾德馨注5。苔痕上阶绿，草色入帘青。谈笑有鸿儒注6，往来无白丁注7。可以调素琴注8，阅金经注9。无丝竹注10之注11乱耳，无案牍注12之劳形注13。南阳注14诸葛庐，西蜀子云亭。孔子云注15：何陋之有注16？";
    printf_zh(TOP,"测试一下%d",123);
    printf_zh(BUTTON,"让我们读一下诗：%s\n",txt);
    clearConsole(TOP);
    printf_zh(TOP,"\n情况如何呢");
	while(1) {
        swiWaitForVBlank();
    }
	return 0;
}

