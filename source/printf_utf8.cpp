#include "printf_utf8.h"
#include <string>

Cord cordTop,cordButton;
FB_FONT fbFontCJK16;
FB_FONT *fbCurrentFont;

void setPos(SCREEN_CHOOSE screen, u16 x, u16 y){
  if(screen == TOP){
    cordTop = {x,y};
  }else{
    cordButton = {x,y};
  }
}
static int fbLoadFontData(FB_FONT *pFont, const char *fontData) {
  memset(pFont, 0, sizeof(FB_FONT));
  pFont->valid = 1;//跳过了对魔数的检查，就这样吧
  pFont->charWidth = *(u8 *)(fontData + 13);
  pFont->charHeight = *(u8 *)(fontData + 14);
  pFont->charDataSize = 1 + ((pFont->charWidth + 7) / 8) * pFont->charHeight;
  pFont->pageSize = pFont->charDataSize * 256;
  pFont->pData = (u8 *)fontData;
  pFont->pIndex = pFont->pData + 16;
  pFont->pCharData = pFont->pIndex + 256;
  return !pFont->valid;
}

void fbInit() {
  FB_FONT *pFont = &fbFontCJK16;
  const char *fontDataBuf = halGetFontDataPtr();
  fbLoadFontData(pFont, fontDataBuf);
  setPos(TOP,0,0);
  setPos(BUTTON,0,0);
  fbCurrentFont = pFont;
}

static int fbDrawUtf16(SCREEN_CHOOSE screen_choose ,u16 char_utf16,u16 color) {
  if (!fbCurrentFont) {
    return 0;
  }
  // int fontW = fbCurrentFont->charWidth;
  int fontH = fbCurrentFont->charHeight;
  int fontCharSize = fbCurrentFont->charDataSize;
  int fontPageSize = fbCurrentFont->pageSize;

  int screenW = halGetScreenWidth();
  int screenH = halGetScreenHeight();
  auto &currentCord = screen_choose == TOP ? cordTop:cordButton;

  if (char_utf16 == '\n') {
    currentCord.y += fontH + 1;
    currentCord.x = 0;
    return 0;
  }
  if(char_utf16 == '\t'){
    currentCord.x += fbCurrentFont->charWidth * 2 ;
    if (currentCord.x >= screenW) {
      currentCord.y += fontH + 1;
      currentCord.x = 0;
    }
    return 0;
  }
  u8 pgOffset = fbCurrentFont->pIndex[char_utf16 >> 8];
  if (pgOffset == 0xFF) {
    return 0;
  }
  u8 *ptr = fbCurrentFont->pCharData + fontPageSize * pgOffset +
            fontCharSize * (char_utf16 & 0xff);
  u8 width = *ptr;
  ptr++;

  if (currentCord.x + width >= screenW) {
    currentCord.y += fontH + 1;
    currentCord.x = 0;
  }
  if (currentCord.y + fontH >= screenH) {
    return 0;
  }
  for (u8 y = 0; y < fontH; y++) {
    for (u8 x = 0; x < width; x++) {
      u8 pix = ptr[y * 2 + x / 8] & (1 << (x % 8));
      if (pix) {
        halDrawPixel(screen_choose , currentCord.x + x, currentCord.y + y, color);
      }
    }
  }
  currentCord.x += width + 1;
  if (currentCord.x >= screenW) {
    currentCord.y += fontH + 1;
    currentCord.x = 0;
  }
  return width;
}
void utf8to16(SCREEN_CHOOSE screen_choose,const char * text,u16 color){
  int utf8_len = strlen(text);
	for(uint i = 0; i < utf8_len;i){
		u16 char_utf16;
		if(!(text[i] & 0x80)){
			char_utf16 = text[i++];
		}else if((text[i] & 0xE0) == 0xC0){
			char_utf16  = (text[i++] & 0x1F) << 6;
			char_utf16 |=  text[i++] & 0x3F;
		}else if((text[i] & 0xF0) == 0xE0){
			char_utf16  = (text[i++] & 0x0F) << 12;
			char_utf16 |= (text[i++] & 0x3F) << 6;
			char_utf16 |=  text[i++] & 0x3F;
		}else{
			i++; // out of range or something (This only does up to U+FFFF since it goes to a U16 anyways)
		}
    fbDrawUtf16(screen_choose ,char_utf16,color);
	}
  
  syncToScreen(screen_choose);
}
char printf_str_buffer[1024];//一个字最多3B，1K应该不会爆了吧
void printf_zh(SCREEN_CHOOSE screen_choose,const char *format, ...)
{
    va_list va;
    va_start(va, format);
    vsniprintf(printf_str_buffer,1024, format, va);
    va_end(va);
    utf8to16(screen_choose, printf_str_buffer,ARGB16(1,31,31,31));
}
void printf_zh_color(SCREEN_CHOOSE screen_choose, u16 color,const char *format, ...)
{
    va_list va;
    va_start(va, format);
    vsniprintf(printf_str_buffer,1024, format, va);
    va_end(va);
    utf8to16(screen_choose, printf_str_buffer,color);
}
void clearConsole(SCREEN_CHOOSE screen_choose){
  halClearPixel(screen_choose);
  setPos(screen_choose,0,0);  
}
//******************************************************************************