#ifndef __MENU_H
#define __MENU_H
#include "lcd.h"
#include "GUI.h"

extern u8 last_menu_status;

void LCD_Show_Menu(u8* key_flag,u8 status,u8 menu_status);
void LCD_Show_Menu_1(u8 status);
#endif
