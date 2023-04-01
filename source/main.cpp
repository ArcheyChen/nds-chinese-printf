/*---------------------------------------------------------------------------------

	$Id: main.cpp,v 1.13 2008-12-02 20:21:20 dovoto Exp $

	Simple console print demo
	-- dovoto


---------------------------------------------------------------------------------*/
#include <nds.h>
#include <nds/memory.h>
#include <nds/arm9/video.h>
#include <nds/arm9/background.h>
#include "wqy_bin.h"
#include <stdio.h>
#include <ft2build.h>
#include "draw.h"
#include FT_FREETYPE_H



void setPixel(int row, int col, u16 color);

#define BG_15BITCOLOR    (1<<7)
#define BG_CBB1          (1<<2)
//2-3   Character Base Block  (0-3, in units of 16 KBytes) (=BG Tile Data)
// 7     Colors/Palettes       (0=16/16, 1=256/1)
//14-15 Screen Size (0-3) 
int main(void) {
    videoSetMode(MODE_3_2D | DISPLAY_BG3_ACTIVE);
    videoSetModeSub(MODE_3_2D | DISPLAY_BG3_ACTIVE);

    REG_BG3CNT =  BgSize_R_256x256 | BG_15BITCOLOR | BG_CBB1;
    REG_BG3CNT_SUB = BgSize_R_256x256 | BG_15BITCOLOR | BG_CBB1;


    // Don't scale bg3 (set its affine transformation matrix to [[1,0],[0,1]])
    //在这个模式下不要缩放
    REG_BG3PA = 1 << 8;
    REG_BG3PD = 1 << 8;

    REG_BG3PA_SUB = 1 << 8;
    REG_BG3PD_SUB= 1 << 8;


    printf_zh(false,"test测试一下");
	while(1) {
        swiWaitForVBlank();
    }
	return 0;
}

