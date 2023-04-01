#include <nds.h>


struct Cord{
	u16 x,y;
};

#define RGB(r,g,b)  ((r) | (g)<<5 | (b)<<10)
#define OFFSET(r,c,w) ((r)*(w)+(c))

#define SCREENWIDTH  (256)
#define SCREENHEIGHT (192)

#define REG_DISPCNT_MAIN  (*(vu32*)0x04000000)
#define REG_DISPCNT_SUB   (*(vu32*)0x04001000)  

void Clear(u16 x, u16 y, u16 w, u16 h, u16 c);
void ClearWithBG(u16* pbg,u16 x, u16 y, u16 w, u16 h);
void DrawPic(u16 *GFX, u16 x, u16 y, u16 w, u16 h, u8 isTrans, u16 tcolor);
void printf_zh(bool isTop,const char *format, ...);
void printf_zh_color(bool isTop,u16 color,const char *format, ...);