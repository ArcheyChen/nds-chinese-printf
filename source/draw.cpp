#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <nds.h>
#include <nds/ndstypes.h>
#include <nds/arm9/video.h>
#include <nds/memory.h>
#include <nds/arm9/background.h>


#include "hzk12.h"
#include "asc126.h"
#include "draw.h"

#define MAX_pReadCache_size 0x20000
#define PIXEL_ENABLE (1<<15)
//高位为0是透明的（不显示）
u8 pReadCache [MAX_pReadCache_size];

//vram[y][x] vram[SCREEN_WIDTH][SCREEN_HEIGHT]
u16 (*vramTop)[SCREEN_WIDTH] = (u16 (*)[SCREEN_WIDTH])BG_GFX;
u16 (*vramButton)[SCREEN_WIDTH] = (u16 (*)[SCREEN_WIDTH])BG_GFX_SUB;
void  setPixel3_main ( int  row ,  int  col ,  u16  color )  { 
    BG_GFX [ OFFSET ( row ,  col ,  SCREEN_WIDTH )]  =  color  |  PIXEL_ENABLE ; 
}

void  setPixel3_sub ( int  row ,  int  col ,  u16  color )  { 
    BG_GFX_SUB [ OFFSET ( row ,  col ,  SCREEN_WIDTH )]  =  color  |  PIXEL_ENABLE ; 
}
//******************************************************************************
void Clear(u16 x, u16 y, u16 w, u16 h, u16 c)
{
	u16 *p = (u16*)VRAM;
	u16 yi,ww,hh;
    

    hh = (y+h>SCREEN_HEIGHT)?SCREEN_HEIGHT:(y+h);
    ww  = (x+w>SCREEN_WIDTH)?(SCREEN_WIDTH-x):w;

	//u16 tmp[SCREEN_WIDTH];
	for(u32 i=0;i<SCREEN_WIDTH;i++)
		((u16*)pReadCache)[i] = c;

	for(yi=y; yi < hh; yi++)
		dmaCopy(pReadCache,p+yi*SCREEN_WIDTH+x,ww*2);         
}
//******************************************************************************
void  ClearWithBG(u16* pbg,u16 x, u16 y, u16 w, u16 h)
{
	u16 *p = (u16*)VRAM;;
	u16 yi,ww,hh;
    
    hh = (y+h>SCREEN_HEIGHT)?SCREEN_HEIGHT:(y+h);
    ww  = (x+w>SCREEN_WIDTH)?(SCREEN_WIDTH-x):w;

	for(yi=y; yi < hh; yi++)
		dmaCopy(pbg+yi*SCREEN_WIDTH+x,p+yi*SCREEN_WIDTH+x,ww*2);       
}
//******************************************************************************
void  DrawPic(u16 *GFX, u16 x, u16 y, u16 w, u16 h, u8 isTrans, u16 tcolor)
{
	u16 *p = (u16*)VRAM;
	u16 c;
	u16 xi,yi,ww,hh;

  hh = (y+h>SCREEN_HEIGHT)?SCREEN_HEIGHT:(y+h);
  ww  = (x+w>SCREEN_WIDTH)?(SCREEN_WIDTH-x):w;	
	
	if(isTrans)
	{
		for(yi=y; yi < hh; yi++)
			for(xi=x;xi<x+ww;xi++)
			{
				c = GFX[(yi-y)*w+(xi-x)];
				if(c!=tcolor)
					p[yi*SCREEN_WIDTH+xi] = c;
			}
	}
	else
	{
		for(yi=y; yi < hh; yi++)
			dmaCopy(GFX+(yi-y)*w,p+yi*SCREEN_WIDTH+x,w*2); 
	}
}
//---------------------------------------------------------------------------------

Cord DrawHZText12(char *str, u16 len, u16 x, u16 y, u16 color,bool top)
{
	u32 i,l,hi=0;
	u32 location;
	u8 cc,c1,c2;
	u16 (*vram)[SCREEN_WIDTH] = vramButton;
	if(top){
		vram = vramTop;
	}
	if(len==0)
		l=strlen(str);
	else
		if(len>strlen(str))
			l=strlen(str);
		else
			l=len;

	if((u16)(len*6)>(u16)(SCREEN_WIDTH-x))
		len=(SCREEN_WIDTH-x)/6;
    while(hi<l)
    {
		c1 = str[hi];
    	hi++;
		if(c1=='\n'){
			x=0;
			y+=12;
		}
    	else if(c1<0x80)  //ASCII
    	{
			if(x>=SCREEN_WIDTH-6){
				x = 0;
				y += 12;
			}
    		location = c1*12;
    		for(i=0;i<12;i++)//字符的高是12，所以画12次
			{
				cc = ASC_DATA[location+i];
				auto vram_line = &vram[y+i][x];
				for(int j=0;j<8;j++){//如果这个字符在第i位是1，那么把对应内存的位置填上color
					if(cc & (1<<j))
						vram_line[7-j] = color;
				}
			}		
    		x+=6;
    	}
		else	//Double-byte
		{	
			if(x>=SCREEN_WIDTH-12){
				x = 0;
				y += 12;
			}
    		c2 = str[hi];
    		hi++;
    		if(c1<0xb0){
    			location = ((c1-0xa1)*94+(c2-0xa1))*24;
			}else{
				location = (9*94+(c1-0xb0)*94+(c2-0xa1))*24;
			}

			
			for(i=0;i<12;i++)
			{				
				cc = acHZK12[location+i*2];
				auto vram_line = &vram[y+i][x];
				for(int j=0;j<8;j++){//第一部分
					if(cc & (1<<j))
						vram_line[7-j] = color;
				}

				cc = acHZK12[location+i*2+1];
				for(int j=0;j<8;j++){//第二部分
					if(cc & (1<<j))
						vram_line[15-j] = color;
				}
			}
			x+=12;
		}

	}
	return Cord{x,y};
}
//---------------------------------------------------------------------------------

static char str[128];
static Cord current_cord_Top={0,0};
static Cord current_cord_Button={0,0};
void printf_zh(bool isTop,const char *format, ...)
{
    va_list va;
    va_start(va, format);
    vsprintf(str, format, va);
    va_end(va);
	auto cord = isTop ? current_cord_Top : current_cord_Button;
    cord = DrawHZText12(str,0,cord.x,cord.y, ARGB16(1,31,31,31),isTop);
	if(isTop){
		current_cord_Top = cord;
	}else{
		current_cord_Button = cord;
	}
	//英文字库结构  12*8 ，高12，宽6 ，前6bit，中文则是宽12
}
void printf_zh_color(bool isTop,u16 color,const char *format, ...)
{
    va_list va;
    va_start(va, format);
    vsprintf(str, format, va);
    va_end(va);
    auto cord = isTop ? current_cord_Top : current_cord_Button;
    cord = DrawHZText12(str,0,cord.x,cord.y, color,isTop);
	if(isTop){
		current_cord_Top = cord;
	}else{
		current_cord_Button = cord;
	}
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
//---------------------------------------------------------------------------------