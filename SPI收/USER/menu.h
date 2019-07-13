#ifndef __MENU_H
#define __MENU_H
#include "lcd.h"
#include "GUI.h"
#include "usart.h"

extern u8 last_menu_status;
extern u8 assign_flag;
extern u8 digits[4];
extern u8 digits_MPPT[4];
extern u16 Voltage;
extern u16 Current;
extern u16 Set_Voltage;

void LCD_Show_Menu(u8* key_flag,u8 status,u8 menu_status,u8* recv_flag,GUI_WW_InitTypeDef* GUI_WW);
void LCD_Show_Menu_1(u8 status);
void LCD_Show_InputBox(u8* digits,u8 status,u16 former,u16 latter,char* header,char* ending);

void LCD_Show_Wave_Init(GUI_WW_InitTypeDef* GUI_WW);
void LCD_Show_Wave_MPPT_Init(GUI_WW_InitTypeDef* GUI_WW);
void LCD_Show_Wave(u16 voltage_F,u16 target,u16 voltage_color,u16 target_color,GUI_WW_InitTypeDef* GUI_WW);

void LCD_Show_Msg(u16 Frequency,u16 Current);
#endif
