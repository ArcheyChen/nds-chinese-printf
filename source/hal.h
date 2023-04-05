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
	TOP = 1,
	BUTTON = 0
}SCREEN_CHOOSE;

int halGetScreenWidth();

int halGetScreenHeight();

void halDrawPixel(SCREEN_CHOOSE isTop ,int x, int y, u16 color);

const char *halGetFontDataPtr();
void halClearPixel(SCREEN_CHOOSE isTop);
void vramInit();
void syncToScreen(SCREEN_CHOOSE isTop);
void syncEnable();
void syncDisable();
void syncToScreen(SCREEN_CHOOSE isTop);