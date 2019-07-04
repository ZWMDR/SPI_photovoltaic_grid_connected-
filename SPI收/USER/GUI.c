#include "GUI.h"

u16 zoom_in[5]={0,10,100,1000,10000};

void GUI_Init(void)
{
	LCD_Init();
	LCD_Fill(0,305,240,320,MAGENTA);
	POINT_COLOR=WHITE;
	BACK_COLOR=MAGENTA;
	LCD_ShowString(68,306,200,12,12,"Designed by ZWMDR");
}

void GUI_clear(void)
{
	LCD_Fill(0,0,240,305,WHITE);
	LCD_Fill(0,305,240,320,MAGENTA);
	POINT_COLOR=WHITE;
	BACK_COLOR=MAGENTA;
	LCD_ShowString(68,306,200,12,12,"Designed by ZWMDR");
}


void draw_consistant_FullLine(u16 start_xcoord,u16 end_xcoord,u16 ycoord)//横向实线
{
	LCD_DrawLine(start_xcoord,ycoord,end_xcoord,ycoord);
}

void draw_consistant_ImaginaryLine(u16 start_xcoord,u16 end_xcoord,u16 ycoord)//横向虚线
{
	u16 i;
	for(i=start_xcoord;i<end_xcoord;i+=4)
	{
		LCD_DrawLine(i,ycoord,i+2,ycoord);
	}
}

void draw_vconsistant_FullLine(u16 xcoord,u16 start_ycoord,u16 end_ycoord)//纵向实线
{
	LCD_DrawLine(xcoord,start_ycoord,xcoord,end_ycoord);
}

void draw_vconsistant_ImaginaryLine(u16 xcoord,u16 start_ycoord,u16 end_ycoord)//纵向虚线
{
	u16 i;
	for(i=start_ycoord;i<end_ycoord;i+=4)
	{
		LCD_DrawLine(xcoord,i,xcoord,i+2);
	}
}

void draw_arb_FullLine(u16 start_xcoord,u16 end_xcoord,u16 ycoord)//任意实线
{
	LCD_DrawLine(start_xcoord,ycoord,end_xcoord,ycoord);
}

void draw_arb_ImaginaryLine(u16 start_xcoord,u16 end_xcoord,u16 ycoord)//任意虚线
{
	
}

void GUI_WaveWindow_Init(GUI_WW_InitTypeDef *GUI_WW)//初始化波形视窗
{
	u16 i;
	u16 coord;
	
	if(GUI_WW->start_xcoord>=GUI_WW->end_xcoord || GUI_WW->start_ycoord>=GUI_WW->end_ycoord)
		return;
	
	//视窗范围填充
	LCD_Fill(GUI_WW->start_xcoord,GUI_WW->start_ycoord,GUI_WW->end_xcoord,GUI_WW->end_ycoord,GUI_WW->back_color);
	
	if(GUI_WW->set_axle_wire)//中轴线
	{
		POINT_COLOR=GUI_WW->axle_wire_color;
		draw_consistant_FullLine(0,240,GUI_WW->axle_wire_ycoord);
		draw_consistant_FullLine(0,240,GUI_WW->axle_wire_ycoord+1);
	}
	if(GUI_WW->vertical_gd->set_gd)//纵向参考线
	{
		POINT_COLOR=GUI_WW->vertical_gd->gd_color;
		if(GUI_WW->vertical_gd->wire_type==0)//实参考线
		{
			coord=GUI_WW->vertical_gd->gd_coord;
			for(i=0;i<GUI_WW->vertical_gd->gd_num;i++)
			{
				draw_consistant_FullLine(GUI_WW->start_xcoord,GUI_WW->end_xcoord,coord);
				coord+=GUI_WW->vertical_gd->gd_interval;
			}
		}
		else if(GUI_WW->vertical_gd->wire_type==2)//虚参考线
		{
			coord=GUI_WW->vertical_gd->gd_coord;
			for(i=0;i<GUI_WW->vertical_gd->gd_num;i++)
			{
				draw_consistant_ImaginaryLine(GUI_WW->start_xcoord,GUI_WW->end_xcoord,coord);
				coord+=GUI_WW->vertical_gd->gd_interval;
			}
		}
	}
	
	if(GUI_WW->horizontal_gd->set_gd)//纵向参考线
	{
		POINT_COLOR=GUI_WW->horizontal_gd->gd_color;
		if(GUI_WW->horizontal_gd->wire_type==0)//实参考线
		{
			coord=GUI_WW->horizontal_gd->gd_coord;
			for(i=0;i<GUI_WW->horizontal_gd->gd_num;i++)
			{
				draw_vconsistant_FullLine(coord,GUI_WW->start_ycoord,GUI_WW->end_ycoord);
				coord+=GUI_WW->horizontal_gd->gd_interval;
			}
		}
		else if(GUI_WW->horizontal_gd->wire_type==2)//虚参考线
		{
			coord=GUI_WW->horizontal_gd->gd_coord;
			for(i=0;i<GUI_WW->horizontal_gd->gd_num;i++)
			{
				draw_vconsistant_ImaginaryLine(coord,GUI_WW->start_ycoord,GUI_WW->end_ycoord);
				coord+=GUI_WW->horizontal_gd->gd_interval;
			}
		}
	}
	
	for(i=0;i<GUI_WW->num_lines;i++)
	{
		GUI_WW->lines[i].msg.lst_xcoord=GUI_WW->lines[i].msg.xcoord=GUI_WW->end_xcoord;
		GUI_WW->lines[i].msg.lst_ycoord=GUI_WW->lines[i].msg.ycoord=GUI_WW->end_ycoord;
	}
}

void GUI_WaveWindow_refresh(GUI_WW_InitTypeDef *GUI_WW)
{
	u16 i;
	for(i=0;i<GUI_WW->num_lines;i++)
	{
		if(GUI_WW->lines[i].msg.show)
		{
			//超出视窗范围矫正
			if(GUI_WW->lines[i].msg.xcoord<GUI_WW->start_xcoord)
				GUI_WW->lines[i].msg.xcoord=GUI_WW->start_xcoord;
			else if(GUI_WW->lines[i].msg.xcoord>GUI_WW->end_xcoord)
				GUI_WW->lines[i].msg.xcoord=GUI_WW->end_xcoord;
			
			if(GUI_WW->lines[i].msg.ycoord<GUI_WW->start_ycoord)
				GUI_WW->lines[i].msg.ycoord=GUI_WW->start_ycoord;
			else if(GUI_WW->lines[i].msg.ycoord>GUI_WW->end_ycoord)
				GUI_WW->lines[i].msg.ycoord=GUI_WW->end_ycoord;
			
			POINT_COLOR=GUI_WW->lines[i].point_color;
			switch(GUI_WW->lines[i].wave_form)
			{
				case 0:
				{
					LCD_DrawLine(GUI_WW->lines[i].msg.lst_xcoord,GUI_WW->lines[i].msg.lst_ycoord,GUI_WW->lines[i].msg.xcoord,GUI_WW->lines[i].msg.ycoord);
					break;
				}
				case 1:
				{
					LCD_DrawLine(GUI_WW->lines[i].msg.lst_xcoord,GUI_WW->lines[i].msg.lst_ycoord,GUI_WW->lines[i].msg.xcoord,GUI_WW->lines[i].msg.ycoord);
					LCD_DrawLine(GUI_WW->lines[i].msg.lst_xcoord,GUI_WW->lines[i].msg.lst_ycoord+1,GUI_WW->lines[i].msg.xcoord,GUI_WW->lines[i].msg.ycoord+1);
					break;
				}
				case 2:
				{
					
					break;
				}
				case 3:
				{
					
					break;
				}
				case 4:
				{
					
					break;
				}
				default:
				{
					
				}
			}
			GUI_WW->lines[i].msg.lst_xcoord=GUI_WW->lines[i].msg.xcoord;
			GUI_WW->lines[i].msg.lst_ycoord=GUI_WW->lines[i].msg.ycoord;
			GUI_WW->lines[i].msg.show=0;
		}
	}
}


void GUI_MsgWindow_Init(GUI_MW_InitTypeDef* GUI_MW)//信息视窗初始化
{
	u16 i;
	u16 xcoord;
	u16 ycoord;
	u8 size;
	float temp;
	short adcx;
	if(GUI_MW->start_xcoord>=GUI_MW->end_xcoord || GUI_MW->start_ycoord>=GUI_MW->end_ycoord)
		return;
	
	//视窗范围填充
	BACK_COLOR=GUI_MW->back_color;
	LCD_Fill(GUI_MW->start_xcoord,GUI_MW->start_ycoord,GUI_MW->end_xcoord,GUI_MW->end_ycoord,GUI_MW->back_color);
	
	//数字内容
	if(GUI_MW->alignment==0)//靠左对齐
	{
		for(i=0;i<GUI_MW->num_num;i++)
		{
			size=GUI_MW->msgs_num[i].size;
			xcoord=GUI_MW->start_xcoord;
			ycoord=GUI_MW->msgs_num[i].ycoord;
			POINT_COLOR=GUI_MW->header_color;
			LCD_ShowString(xcoord,ycoord,180,size,size,(u8*)(GUI_MW->msgs_num[i].header));
			
			POINT_COLOR=GUI_MW->msg_color;
			xcoord+=size/2*GUI_MW->msgs_num[i].digits_header;
			temp=GUI_MW->msgs_num[i].number;
			adcx=temp;
			LCD_ShowxNum(xcoord,ycoord,adcx,GUI_MW->msgs_num[i].digits_former,size,0);
			temp-=adcx;
			temp*=zoom_in[GUI_MW->msgs_num[i].digits_latter];
			xcoord+=size/2*(GUI_MW->msgs_num[i].digits_former+1);
			LCD_ShowxNum(xcoord,ycoord,temp,GUI_MW->msgs_num[i].digits_latter,size,0);
		}
	}
	else if(GUI_MW->alignment==1)//居中对齐
	{
		for(i=0;i<GUI_MW->num_num;i++)
		{
			xcoord=GUI_MW->axle;
			ycoord=GUI_MW->msgs_num[i].ycoord;
			size=GUI_MW->msgs_num[i].size/2;
			POINT_COLOR=GUI_MW->header_color;
			LCD_ShowString(xcoord-GUI_MW->msgs_num[i].digits_header*size,ycoord,180,GUI_MW->msgs_num[i].size,GUI_MW->msgs_num[i].size,(u8*)GUI_MW->msgs_num[i].header);
			
			POINT_COLOR=GUI_MW->msg_color;
			temp=GUI_MW->msgs_num[i].number;
			adcx=temp;
			LCD_ShowxNum(xcoord,ycoord,adcx,GUI_MW->msgs_num[i].digits_former,GUI_MW->msgs_num[i].size,0);
			temp-=adcx;
			temp*=zoom_in[GUI_MW->msgs_num[i].digits_latter];
			xcoord+=size*(GUI_MW->msgs_num[i].digits_former+1);
			LCD_ShowxNum(xcoord,ycoord,temp,GUI_MW->msgs_num[i].digits_latter,GUI_MW->msgs_num[i].size,0);
		}
	}
	
	//字符内容
	if(GUI_MW->alignment==0)//靠左对齐
	{
		for(i=0;i<GUI_MW->str_num;i++)
		{
			size=GUI_MW->msgs_str[i].size;
			xcoord=GUI_MW->start_xcoord;
			ycoord=GUI_MW->msgs_str[i].ycoord;
			POINT_COLOR=GUI_MW->header_color;
			LCD_ShowString(xcoord,ycoord,180,size,size,(u8*)(GUI_MW->msgs_str[i].header));
			
			POINT_COLOR=GUI_MW->msg_color;
			xcoord+=size/2*GUI_MW->msgs_str[i].digits_header;
			LCD_ShowString(xcoord,ycoord,180,size,size,(u8*)(GUI_MW->msgs_str[i].content));
		}
	}
	else if(GUI_MW->alignment==1)//居中对齐
	{
		for(i=0;i<GUI_MW->str_num;i++)
		{
			xcoord=GUI_MW->axle;
			ycoord=GUI_MW->msgs_str[i].ycoord;
			size=GUI_MW->msgs_str[i].size/2;
			POINT_COLOR=GUI_MW->header_color;
			LCD_ShowString(xcoord-GUI_MW->msgs_str[i].digits_header*size,ycoord,180,GUI_MW->msgs_num[i].size,GUI_MW->msgs_num[i].size,(u8*)GUI_MW->msgs_str[i].header);
			
			POINT_COLOR=GUI_MW->msg_color;
			LCD_ShowString(xcoord,ycoord,180,GUI_MW->msgs_num[i].size,GUI_MW->msgs_num[i].size,
										(u8*)GUI_MW->msgs_str[i].content);
		}
	}
	
}

void GUI_MsgWindow_refresh(GUI_MW_InitTypeDef *GUI_MW)
{
	u16 i;
	u16 xcoord;
	u16 ycoord;
	u8 size;
	float temp;
	short adcx;
	
	//数字内容
	if(GUI_MW->alignment==0)//靠左对齐
	{
		for(i=0;i<GUI_MW->num_num;i++)
		{
			size=GUI_MW->msgs_num[i].size;
			xcoord=GUI_MW->start_xcoord;
			ycoord=GUI_MW->msgs_num[i].ycoord;
			
			POINT_COLOR=GUI_MW->msg_color;
			xcoord+=size/2*GUI_MW->msgs_num[i].digits_header;
			temp=GUI_MW->msgs_num[i].number;
			adcx=temp;
			LCD_ShowxNum(xcoord,ycoord,adcx,GUI_MW->msgs_num[i].digits_former,size,0);
			temp-=adcx;
			temp*=zoom_in[GUI_MW->msgs_num[i].digits_latter];
			xcoord+=size/2*(GUI_MW->msgs_num[i].digits_former+1);
			LCD_ShowxNum(xcoord,ycoord,temp,GUI_MW->msgs_num[i].digits_latter,size,0);
		}
	}
	else if(GUI_MW->alignment==1)//居中对齐
	{
		for(i=0;i<GUI_MW->num_num;i++)
		{
			xcoord=GUI_MW->axle;
			ycoord=GUI_MW->msgs_num[i].ycoord;
			size=GUI_MW->msgs_num[i].size/2;
			
			POINT_COLOR=GUI_MW->msg_color;
			temp=GUI_MW->msgs_num[i].number;
			adcx=temp;
			LCD_ShowxNum(xcoord,ycoord,adcx,GUI_MW->msgs_num[i].digits_former,GUI_MW->msgs_num[i].size,0);
			temp-=adcx;
			temp*=zoom_in[GUI_MW->msgs_num[i].digits_latter];
			xcoord+=size*(GUI_MW->msgs_num[i].digits_former+1);
			LCD_ShowxNum(xcoord,ycoord,temp,GUI_MW->msgs_num[i].digits_latter,GUI_MW->msgs_num[i].size,0);
		}
	}
	
	//字符内容
	if(GUI_MW->alignment==0)//靠左对齐
	{
		for(i=0;i<GUI_MW->str_num;i++)
		{
			size=GUI_MW->msgs_str[i].size;
			xcoord=GUI_MW->start_xcoord;
			ycoord=GUI_MW->msgs_str[i].ycoord;
			
			POINT_COLOR=GUI_MW->msg_color;
			xcoord+=size/2*GUI_MW->msgs_str[i].digits_header;
			LCD_ShowString(xcoord,ycoord,180,size,size,(u8*)(GUI_MW->msgs_str[i].content));
		}
	}
	else if(GUI_MW->alignment==1)//居中对齐
	{
		for(i=0;i<GUI_MW->str_num;i++)
		{
			xcoord=GUI_MW->axle;
			ycoord=GUI_MW->msgs_str[i].ycoord;
			size=GUI_MW->msgs_str[i].size/2;
			
			POINT_COLOR=GUI_MW->msg_color;
			LCD_ShowString(xcoord,ycoord,180,GUI_MW->msgs_num[i].size,GUI_MW->msgs_num[i].size,
										(u8*)GUI_MW->msgs_str[i].content);
		}
	}
}

void GUI_show_frame(u8 frame_mode,u16 frame_color,u16 start_xcoord,u16 start_ycoord,u16 end_xcoord,u16 end_ycoord)
{
	POINT_COLOR=frame_color;
	if(frame_mode==1)
	{
		draw_consistant_FullLine(start_xcoord,end_xcoord,start_ycoord);
		draw_consistant_FullLine(start_xcoord,end_xcoord,end_ycoord);
		draw_vconsistant_FullLine(start_xcoord,start_ycoord,end_ycoord);
		draw_vconsistant_FullLine(end_xcoord,start_ycoord,end_ycoord);
	}
	else if(frame_mode==11)
	{
		draw_consistant_ImaginaryLine(start_xcoord,end_xcoord,start_ycoord);
		draw_consistant_ImaginaryLine(start_xcoord,end_xcoord,end_ycoord);
		draw_vconsistant_ImaginaryLine(start_xcoord,start_ycoord,end_ycoord);
		draw_vconsistant_ImaginaryLine(end_xcoord,start_ycoord,end_ycoord);
	}
	else if(frame_mode==2)
	{
		draw_consistant_FullLine(start_xcoord,end_xcoord,start_ycoord);
		draw_consistant_FullLine(start_xcoord,end_xcoord,start_ycoord+1);
		draw_consistant_FullLine(start_xcoord,end_xcoord,end_ycoord);
		draw_consistant_FullLine(start_xcoord,end_xcoord,end_ycoord-1);
		draw_vconsistant_FullLine(start_xcoord,start_ycoord,end_ycoord);
		draw_vconsistant_FullLine(start_xcoord+1,start_ycoord,end_ycoord);
		draw_vconsistant_FullLine(end_xcoord,start_ycoord,end_ycoord);
		draw_vconsistant_FullLine(end_xcoord-1,start_ycoord,end_ycoord);
	}
	else if(frame_mode==12)
	{
		draw_consistant_ImaginaryLine(start_xcoord,end_xcoord,start_ycoord);
		draw_consistant_ImaginaryLine(start_xcoord,end_xcoord,start_ycoord+1);
		draw_consistant_ImaginaryLine(start_xcoord,end_xcoord,end_ycoord);
		draw_consistant_ImaginaryLine(start_xcoord,end_xcoord,end_ycoord-1);
		draw_vconsistant_ImaginaryLine(start_xcoord,start_ycoord,end_ycoord);
		draw_vconsistant_ImaginaryLine(start_xcoord+1,start_ycoord,end_ycoord);
		draw_vconsistant_ImaginaryLine(end_xcoord,start_ycoord,end_ycoord);
		draw_vconsistant_ImaginaryLine(end_xcoord-1,start_ycoord,end_ycoord);
	}
	else if(frame_mode==3)
	{
		draw_consistant_FullLine(start_xcoord,end_xcoord,start_ycoord);
		draw_consistant_FullLine(start_xcoord,end_xcoord,start_ycoord+1);
		draw_consistant_FullLine(start_xcoord,end_xcoord,start_ycoord+2);
		draw_consistant_FullLine(start_xcoord,end_xcoord,end_ycoord);
		draw_consistant_FullLine(start_xcoord,end_xcoord,end_ycoord-1);
		draw_consistant_FullLine(start_xcoord,end_xcoord,end_ycoord-2);
		draw_vconsistant_FullLine(start_xcoord,start_ycoord,end_ycoord);
		draw_vconsistant_FullLine(start_xcoord+1,start_ycoord,end_ycoord);
		draw_vconsistant_FullLine(start_xcoord+2,start_ycoord,end_ycoord);
		draw_vconsistant_FullLine(end_xcoord,start_ycoord,end_ycoord);
		draw_vconsistant_FullLine(end_xcoord-1,start_ycoord,end_ycoord);
		draw_vconsistant_FullLine(end_xcoord-2,start_ycoord,end_ycoord);
	}
	else if(frame_mode==13)
	{
		draw_consistant_ImaginaryLine(start_xcoord,end_xcoord,start_ycoord);
		draw_consistant_ImaginaryLine(start_xcoord,end_xcoord,start_ycoord+1);
		draw_consistant_ImaginaryLine(start_xcoord,end_xcoord,start_ycoord+2);
		draw_consistant_ImaginaryLine(start_xcoord,end_xcoord,end_ycoord);
		draw_consistant_ImaginaryLine(start_xcoord,end_xcoord,end_ycoord-1);
		draw_consistant_ImaginaryLine(start_xcoord,end_xcoord,end_ycoord-2);
		draw_vconsistant_ImaginaryLine(start_xcoord,start_ycoord,end_ycoord);
		draw_vconsistant_ImaginaryLine(start_xcoord+1,start_ycoord,end_ycoord);
		draw_vconsistant_ImaginaryLine(start_xcoord+2,start_ycoord,end_ycoord);
		draw_vconsistant_ImaginaryLine(end_xcoord,start_ycoord,end_ycoord);
		draw_vconsistant_ImaginaryLine(end_xcoord-1,start_ycoord,end_ycoord);
		draw_vconsistant_ImaginaryLine(end_xcoord-2,start_ycoord,end_ycoord);
	}
}

u8 GUI_Menu_Init(GUI_Menu_InitTypeDef* Menu)//菜单显示初始化
{
	u8 i;
	
	LCD_Fill(Menu->start_xcoord,Menu->start_ycoord,Menu->end_xcoord,Menu->end_ycoord,Menu->back_color);
	
	if(Menu->show_frame)
		GUI_show_frame(Menu->frame_mode,Menu->frame_color,Menu->start_xcoord,Menu->start_ycoord,Menu->end_xcoord,Menu->end_ycoord);
	
	for(i=0;i<Menu->table_num;i++)
	{
		if(Menu->catalog_tables[i].xcoord==0)
		{
			Menu->catalog_tables[i].xcoord=(Menu->end_xcoord-Menu->start_xcoord-Menu->catalog_tables[i].size/2*Menu->catalog_tables[i].length)/2+Menu->start_xcoord;
		}
		if(Menu->catalog_tables[i].ycoord==0)
		{
			Menu->catalog_tables[i].ycoord=(Menu->end_ycoord-Menu->start_ycoord)/Menu->table_num*i+Menu->start_ycoord;
		}
	}
	
	BACK_COLOR=Menu->back_color;
	POINT_COLOR=Menu->point_color;
	for(i=0;i<Menu->table_num;i++)
	{
		LCD_ShowString(Menu->catalog_tables[i].xcoord,Menu->catalog_tables[i].ycoord,180,Menu->catalog_tables[i].size,Menu->catalog_tables[i].size,(u8*)(Menu->catalog_tables[i].content));
	}
	return 0;
}

u8 GUI_Menu_Select(GUI_Menu_InitTypeDef *Menu,u8 select_status)
{
	u8 i;
	if(select_status>Menu->table_num)
		return 0;
	
	if(select_status>0)
	{
		POINT_COLOR=Menu->select_point_color;
		BACK_COLOR=Menu->select_back_color;
		LCD_ShowString(Menu->catalog_tables[select_status-1].xcoord,Menu->catalog_tables[select_status-1].ycoord,
										180,Menu->catalog_tables[select_status-1].size,Menu->catalog_tables[select_status-1].size,
										(u8*)(Menu->catalog_tables[select_status-1].content));
	}
	POINT_COLOR=Menu->point_color;
	BACK_COLOR=Menu->back_color;
	for(i=0;i<Menu->table_num;i++)
	{
		if(i!=select_status-1)
		{
			LCD_ShowString(Menu->catalog_tables[i].xcoord,Menu->catalog_tables[i].ycoord,180,
										Menu->catalog_tables[i].size,Menu->catalog_tables[i].size,
										(u8*)(Menu->catalog_tables[i].content));
		}
	}
	return 1;
}


void GUI_InputBox_Init(GUI_IB_InitTypeDef* GUI_IB,u8 select_status)
{
	u8 i;
	u8 num=GUI_IB->digit_fromer+GUI_IB->digit_later;
	u16 xcoord=GUI_IB->xcoord;
	LCD_Fill(GUI_IB->start_xcoord,GUI_IB->start_ycoord,GUI_IB->end_xcoord,GUI_IB->end_ycoord,GUI_IB->back_color);
	if(GUI_IB->show_frame)
		GUI_show_frame(GUI_IB->frame_mode,GUI_IB->back_color,GUI_IB->start_xcoord,GUI_IB->start_ycoord,GUI_IB->end_xcoord,GUI_IB->end_ycoord);
	
	POINT_COLOR=GUI_IB->header_color;
	LCD_ShowString(xcoord,GUI_IB->ycoord,180,GUI_IB->size,GUI_IB->size,(u8*)(GUI_IB->header));
	xcoord+=GUI_IB->digit_header*GUI_IB->size/2;
	for(i=0;i<GUI_IB->digit_fromer;i++)
	{
		if(select_status==i)
		{
			POINT_COLOR=GUI_IB->select_point_color;
			BACK_COLOR=GUI_IB->select_back_color;
		}
		
		LCD_ShowNum(xcoord,GUI_IB->ycoord,GUI_IB->digits[i],1,GUI_IB->size);
		
		if(select_status==i)
		{
			POINT_COLOR=GUI_IB->digits_color;
			BACK_COLOR=GUI_IB->back_color;
		}
	}
}

