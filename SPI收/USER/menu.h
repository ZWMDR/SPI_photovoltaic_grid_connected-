#ifndef __MENU_H
#define __MENU_H
#include "lcd.h"
#include "GUI.h"

extern u8 last_menu_status;
extern u8 assign_flag;
extern u8 digits[4];

void LCD_Show_Menu(u8* key_flag,u8 status,u8 menu_status,u8* recv_flag,GUI_WW_InitTypeDef* GUI_WW);
void LCD_Show_Menu_1(u8 status);
void LCD_Show_InputBox(u8 status,u16 former,u16 latter,char* header,char* ending);
#endif
