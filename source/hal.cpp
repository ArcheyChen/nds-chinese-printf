#include "hal.h"
u16 vramBuf[2][192][256];
u16 (*vram[2])[SCREEN_WIDTH] = {
  (u16 (*)[SCREEN_WIDTH])BG_GFX_SUB,
  (u16 (*)[SCREEN_WIDTH])BG_GFX
};
bool sync_enable = true;
auto VRAM_SIZE = sizeof(u16)*SCREEN_WIDTH*SCREEN_HEIGHT;
// u16 (*vramButton)[SCREEN_WIDTH] = (u16 (*)[SCREEN_WIDTH])BG_GFX_SUB;

int halGetScreenWidth() { return SCREEN_WIDTH; }

int halGetScreenHeight() { return SCREEN_HEIGHT; }

void halDrawPixel(SCREEN_CHOOSE isTop,int x, int y, u16 color) { vramBuf[isTop][y][x] = color; }

const char *halGetFontDataPtr() {
  return my_font;
}
void Vblank() {
  // dmaCopyWordsAsynch(0,vramBuf[0],vram[0],VRAM_SIZE);
  // dmaCopyWordsAsynch(1,vramBuf[1],vram[1],VRAM_SIZE);
}
void syncEnable(){
  sync_enable = true;
}
void syncDisable(){
  sync_enable = false;
}
void syncToScreen(SCREEN_CHOOSE isTop){
  if(sync_enable){
    dmaCopy(vramBuf[isTop],vram[isTop],VRAM_SIZE);
    // memcpy(vram[isTop],vramBuf[isTop],VRAM_SIZE);
  }
}
void halClearPixel(SCREEN_CHOOSE isTop){
  dmaFillWords(0,vramBuf[isTop],VRAM_SIZE);
  // memset(vramBuf[isTop],0,VRAM_SIZE);
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
  syncEnable();

	// irqSet(IRQ_VBLANK, Vblank);
  // irqEnable(IRQ_VBLANK);
  // REG_IME = 1;

  REG_BG3CNT =  BgSize_R_256x256 | BG_15BITCOLOR | BG_CBB1;
  REG_BG3CNT_SUB = BgSize_R_256x256 | BG_15BITCOLOR | BG_CBB1;


  // Don't scale bg3 (set its affine transformation matrix to [[1,0],[0,1]])
  //在这个模式下不要缩放
  REG_BG3PA = 1 << 8;
  REG_BG3PD = 1 << 8;

  REG_BG3PA_SUB = 1 << 8;
  REG_BG3PD_SUB= 1 << 8;
}