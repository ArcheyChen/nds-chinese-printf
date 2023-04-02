#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nds.h>
#include <nds/arm9/video.h>
#include <nds/arm9/background.h>
#include "my_font.h"
typedef enum{
	TOP = true,
	BUTTON = false
}SCREEN_CHOOSE;

u16 (*vramTop)[SCREEN_WIDTH] = (u16 (*)[SCREEN_WIDTH])BG_GFX;
u16 (*vramButton)[SCREEN_WIDTH] = (u16 (*)[SCREEN_WIDTH])BG_GFX_SUB;

inline int halGetScreenWidth() { return 240; }

inline int halGetScreenHeight() { return 160; }

void halDrawPixel(u16 (*vram)[SCREEN_WIDTH] ,int x, int y, u16 color) { vram[y][x] = color; }

const char *halGetFontDataPtr() {
  return my_font;
}
void halClearPixel(u16 (*vram)[SCREEN_WIDTH]){
  memset(vram,0,sizeof(u16)*SCREEN_WIDTH*SCREEN_HEIGHT);
}
void vramInit(){
	#define BG_15BITCOLOR    (1<<7)
	#define BG_CBB1          (1<<2)
	//2-3   Character Base Block  (0-3, in units of 16 KBytes) (=BG Tile Data)
	// 7     Colors/Palettes       (0=16/16, 1=SCREEN_WIDTH/1)
	//14-15 Screen Size (0-3) 
	//https://www.pmallory.com/bitmap-graphics-on-the-nintendo-ds-part-2-mode-3.html
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
}