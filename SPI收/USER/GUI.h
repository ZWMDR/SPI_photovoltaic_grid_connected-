#ifndef __GUI_H
#define __GUI_H
#include "lcd.h"
#include "led.h"

/*--------------------波形显示视窗------------------------*/

typedef struct GUI_WaveWindow_msg//波形参数
{
	u16 lst_xcoord;
	u16 lst_ycoord;
	u16 xcoord;
	u16 ycoord;
	u8 show;//是否需要显示（更新）
}WW_msg;

typedef struct GUI_waveform_InitTypeDef//波形显示参数
{
	u16 point_color;//波形颜色
	u8 wave_form;//波形
	u8 refresh_type;//刷新方式
	
	WW_msg msg;
	
}GUI_wf_InitTypeDef;
/*--------------------
     wave_form
   0: 普通单层实线
   1: 双层实线
   2: 单层虚线
   3: 双层虚线
   4: 覆盖式线型

    refresh_type
    0: 屏满刷新
    1: 每次刷新
--------------------*/

typedef struct GUI_WaveWindow_guide_line_InitTypeDef//等间距参考线设置参数
{
	u8 set_gd;//是否设置参考线
	u8 gd_num;//参考线数量
	u8 wire_type;//参考线线型
	/*---------------
	wave_form
   0: 单层实线
   2: 单层虚线
	----------------*/
	
	u16 gd_color;//参考线颜色
	u16 gd_coord;//参考线起始位置
	u16 gd_interval;//参考线间隔
	
}GUI_WW_gd_InitTypeDef;

typedef struct GUI_WaveWindow_InitTypeDef//波形窗口参数
{
	//视窗大小
	u16 start_xcoord;
	u16 start_ycoord;
	u16 end_xcoord;
	u16 end_ycoord;
	u16 back_color;
	
	//视窗线型参数
	u16 num_lines;//视窗线型种类
	GUI_wf_InitTypeDef *lines;//视窗线型参数
	
	//中轴线
	u8 set_axle_wire;//是否设置中轴线
	u16 axle_wire_ycoord;//中轴线位置
	u16 axle_wire_color;//中轴线颜色
	
	//纵向参考线
	u8 set_vertical_gd;
	GUI_WW_gd_InitTypeDef *vertical_gd;
	
	//横向参考线
	u8 set_hroizontal_gd;
	GUI_WW_gd_InitTypeDef *horizontal_gd;
	
}GUI_WW_InitTypeDef;

/*--------------------信息显示视窗------------------------*/
typedef struct GUI_msg_num_TypeDef
{
	char* header;
	float number;
	u8 size;
	
	u8 digits_header;//标题长度
	u8 digits_latter;//小数点前位数
	u8 digits_former;//小数点后位数
	u16 ycoord;
	
}GUI_msg_num;

typedef struct GUI_msg_str_TypeDef
{
	char* header;
	char* content;
	u8 size;
	
	u8 digits_header;//标题长度
	u8 digits_content;//内容长度
	u16 ycoord;
	
}GUI_msg_str;

typedef struct GUI_MsgWindow_InitTypeDef
{
	//视窗大小
	u16 start_xcoord;
	u16 start_ycoord;
	u16 end_xcoord;
	u16 end_ycoord;
	u16 back_color;
	
	u8 alignment;
	u16 axle;
	/*---------------
	对齐方式
	0：左对齐
	1: 中对齐
	----------------*/
	
	u16 num_num;
	u16 str_num;
	u16 header_color;
	u16 msg_color;
	GUI_msg_num* msgs_num;
	GUI_msg_str* msgs_str;
	
}GUI_MW_InitTypeDef;

/*---------------------菜单栏------------------------*/

typedef struct GUI_Menu_catalog_table//菜单内容
{
	char* content;
	u8 size;//字号
	u8 length;//字符长度
	u16 xcoord;
	u16 ycoord;
	
}GUI_catalog_table;

typedef struct GUI_Menu_InitTypeDef
{
	//菜单窗口参数
	u16 start_xcoord;
	u16 start_ycoord;
	u16 end_xcoord;
	u16 end_ycoord;
	
	//背景颜色、选中颜色
	u16 back_color;
	u16 point_color;
	u16 select_back_color;
	u16 select_point_color;
	
	//显示外框
	u8 show_frame;
	u8 frame_mode;
	/*--------------
	0: 不显示外框
	1: 单层外框
	2: 双层外框
	3: 三层外框
	11:单层虚线外框
	12:双层虚线外框
	13:三层虚线外框
	--------------*/
	u16 frame_color;
	
	//菜单内容
	u8 table_num;//菜单数目
	GUI_catalog_table* catalog_tables;//具体内容
	
}GUI_Menu_InitTypeDef;


/*---------------------输入框------------------------*/

typedef struct GUI_InputBox_InitTypeDef
{
	u16 start_xcoord;
	u16 start_ycoord;
	u16 end_xcoord;
	u16 end_ycoord;
	
	u16 back_color;
	u16 point_color;
	u16 select_back_color;
	u16 select_point_color;
	
	u8 show_frame;
	u8 frame_mode;
	u16 frame_color;
	
	char* header;
	u8 digit_header;
	u8 digit_fromer;
	u8 digit_later;
	u8* digits;
	u8 size;
	u16 header_color;
	u16 digits_color;
	u16 xcoord;
	u16 ycoord;
	float number;
	
}GUI_IB_InitTypeDef;

void GUI_Init(void);//初始化
void GUI_clear(void);//清屏

void GUI_show_frame(u8 frame_mode,u16 frame_color,u16 start_xcoord,u16 start_ycoord,u16 end_xcoord,u16 end_ycoord);


void GUI_WaveWindow_Init(GUI_WW_InitTypeDef *GUI_WW);//GUI波形视窗初始化
void GUI_WaveWindow_refresh(GUI_WW_InitTypeDef *GUI_WW);//GUI波形视窗刷新


void GUI_MsgWindow_Init(GUI_MW_InitTypeDef* GUI_MW); //GUI信息视窗初始化
void GUI_MsgWindow_refresh(GUI_MW_InitTypeDef *GUI_MW);//GUI信息视窗刷新


//菜单


//选择菜单
u8 GUI_Menu_Init(GUI_Menu_InitTypeDef* Menu);//菜单初始化
u8 GUI_Menu_Select(GUI_Menu_InitTypeDef *Menu,u8 select_status);//菜单选择

//输入框
void GUI_InputBox_Init(GUI_IB_InitTypeDef* GUI_IB,u8 select_status);


#endif
