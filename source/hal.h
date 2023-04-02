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

extern u16 (*vramTop)[SCREEN_WIDTH];
extern u16 (*vramButton)[SCREEN_WIDTH];

int halGetScreenWidth();

int halGetScreenHeight();

void halDrawPixel(u16 (*vram)[SCREEN_WIDTH] ,int x, int y, u16 color);

const char *halGetFontDataPtr();
void halClearPixel(u16 (*vram)[SCREEN_WIDTH]);
void vramInit();