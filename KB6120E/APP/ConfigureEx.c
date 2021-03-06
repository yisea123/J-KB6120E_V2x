/**************** (C) COPYRIGHT 2014 青岛金仕达电子科技有限公司 ****************
* 文 件 名: ConfigureEx.C
* 创 建 者: 董峰
* 描  述  : KB-6120E 配置程序（厂家配置部分）
* 最后修改: 2014年3月1日
*********************************** 修订记录 ***********************************
* 版  本:
* 修订人:
*******************************************************************************/
#include "AppDEF.H"
#include "stdio.h"
/********************************** 功能说明 ***********************************
*	厂家配置 -> 泵类型
*******************************************************************************/
/********************************** 功能说明 ***********************************
*	扩展配置 -> 泵累计运行时间
*******************************************************************************/
void	menu_Clean_SumTime( void )
{
	BOOL	Done = FALSE;

	do
	{
		cls();	//	Lputs( 0x0000u, "运行时间(小时)" );
		Lputs( 0x0102u, "泵类型" );
		Lputs( 0x0112u, "时间" );

		Lputs( 0x0402u, " A路:" );
		ShowFP32( 0x040Cu, PumpSumTimeLoad( SP_R24_A  ) / 60.0f, 0x0601u, "h" );
		Lputs( 0x0802u, " B路:" );
		ShowFP32( 0x080Cu, PumpSumTimeLoad( SP_R24_B  ) / 60.0f, 0x0601u, "h" );
		Lputs( 0x0C02u, " C路:" );
		ShowFP32( 0x0C0Cu, PumpSumTimeLoad( SP_SHI_C  ) / 60.0f, 0x0601u, "h" );
		Lputs( 0x1002u, " D路:" );
		ShowFP32( 0x100Cu, PumpSumTimeLoad( SP_SHI_D  ) / 60.0f, 0x0601u, "h" );
		Lputs( 0x1402u, " 粉尘:" );
		ShowFP32( 0x140Cu, PumpSumTimeLoad( SP_TSP    ) / 60.0f, 0x0601u, "h" );

		switch ( getKey() )
		{
			case	K_OK	:

				if ( ! releaseKey( K_OK, 100u ))
				{
					beep();

					if ( vbYes == MsgBox( "清除累计时间 ?", vbYesNo | vbDefaultButton2 ))
					{
						PumpSumTimeSave( SP_TSP,   0u );
						PumpSumTimeSave( SP_R24_A, 0u );
						PumpSumTimeSave( SP_R24_B, 0u );
						PumpSumTimeSave( SP_SHI_C, 0u );
						PumpSumTimeSave( SP_SHI_D, 0u );
					}
				}

				break;
			case	K_ESC	:
				Done = TRUE;
				break;
			default	:
				break;
		}
	}
	while( !Done );
}

/********************************** 功能说明 ***********************************
*  系统配置 -> 文件号
*******************************************************************************/
void	menu_Clean_FileNum( void )
{
	BOOL	Done = FALSE;

	do
	{
		cls();
		Lputs( 0x0102u, "类型" );
		Lputs( 0x010Cu, "文件号" );
		Lputs( 0x0802u, "粉 尘:" );
		ShowI16U( 0x080Cu, SampleSet[SP_TSP].FileNum, 	0x0300u, NULL );
		Lputs( 0x0C02u, "日均A:" );
		ShowI16U( 0x0C0Cu, SampleSet[SP_R24_A].FileNum, 0x0300u, NULL );
		Lputs( 0x1002u, "日均B:" );
		ShowI16U( 0x100Cu, SampleSet[SP_R24_B].FileNum, 0x0300u, NULL );
		Lputs( 0x1402u, "时均C:" );
		ShowI16U( 0x140Cu, SampleSet[SP_SHI_C].FileNum, 0x0300u, NULL );
		Lputs( 0x1802u, "时均D:" );
		ShowI16U( 0x180Cu, SampleSet[SP_SHI_D].FileNum, 0x0300u, NULL );

		switch ( getKey() )
		{
			case	K_OK	:

				if ( ! releaseKey( K_OK, 100u ))
				{
					beep();

					if ( vbYes == MsgBox( "清除所有文件 ?", vbYesNo | vbDefaultButton2 ) )
					{
						File_Clean();

						SampleSet[SP_TSP].FileNum =
						  SampleSet[SP_R24_A].FileNum =
						    SampleSet[SP_R24_B].FileNum =
						      SampleSet[SP_SHI_C].FileNum =
						        SampleSet[SP_SHI_D].FileNum = 0u;
						SampleSetSave();
					}
				}

				break;
			case	K_ESC	:
				Done = TRUE;
				break;
			default	:
				break;
		}
	}
	while( !Done );

}
void  menu_Sample_Sum( void )
{
	static	struct  uMenu  const  menu[] =
	{
		{ 0x0201u, "采样累计" },
		{ 0x0C02u, "文件编号" },
		{ 0x1502u, "泵累计" },

	};
	uint8_t item = 1u;

	do
	{
		cls();
		Menu_Redraw( menu );
		item = Menu_Select( menu, item, NULL );

		switch( item )
		{
			case 1:
				menu_Clean_FileNum();
				break;
			case 2:
				menu_Clean_SumTime();
				break;
			default:
				break;
		}
	}
	while( enumSelectESC != item );
}


void	menu_ConfigureDisplay( void )
{
	static	struct  uMenu  const  menu[] =
	{
		{ 0x0301u, "配置 显示" },
		{ 0x0802u, "灰度" },
		{ 0x1002u, "亮度" },
		{ 0x1802u, "定时" }
	};
	uint8_t item = 1u;

	uint16_t gray  = Configure.DisplayGray;
	uint16_t light = Configure.DisplayLight;
	uint8_t  ltime = Configure.TimeoutLight;
	BOOL	changed = FALSE;
	cls();
	Menu_Redraw( menu );

	do
	{
		ShowI16U( 0x080Du, gray,  0x0401u, "% " );
		ShowI16U( 0x100Cu, light, 0x0300u, " % " );

		switch ( ltime )
		{
			case 0:
				Lputs( 0x180Cu, "[关闭] " );
				break;
			case 1:
				Lputs( 0x180Cu, "[15秒] " );
				break;
			case 2:
				Lputs( 0x180Cu, "[30秒] " );
				break;
			case 3:
				Lputs( 0x180Cu, "[60秒] " );
				break;
			default:
			case 4:
				Lputs( 0x180Cu, "[常亮] " );
				break;
		}

		item = Menu_Select( menu, item, NULL );

		switch( item )
		{
			case 1:

				if ( EditI16U( 0x080Du, &gray, 0x0401u ))
				{
					if ( gray > 1000u )
					{
						gray = 1000u;
					}

					if ( gray <  1u )
					{
						gray =  1u;
					}

					DisplaySetGrayVolt( gray * 0.022f );
					changed = TRUE;
				}

				break;
			case 2:

				if ( EditI16U( 0x100Cu, &light, 0x0300u ))
				{
					if ( light > 100u )
					{
						light = 100u;
					}

					DisplaySetLight( light );
					changed = TRUE;
				}

				break;
			case 3:

				if ( ++ltime > 4 )
				{
					ltime = 0u;
				}

				DisplaySetTimeout( ltime );
				changed = TRUE;
				break;
		}

	}
	while ( enumSelectESC != item );

	if ( changed )
	{
		Configure.DisplayGray  = gray;
		Configure.DisplayLight = light;
		Configure.TimeoutLight = ltime;
		ConfigureSave();
	}
}
extern	void	Configure_Instrument( void );
static	menu_Instr_Config( void )
{
	Configure_Instrument();
}



/********************************** 功能说明 ***********************************
*	扩展配置（只能厂家操作的部分）

厂家调试
程序版本
泵累计运行时间
泵（流量计）型号选择，仪器显示名称选择，选择供电电源类型
计前压力保护限制值设置，防止泵堵塞
显示屏幕调整
文件编号、文件存储内容清零
*******************************************************************************/
void	menu_ConfigureEx( void )
{
	static	struct  uMenu  const  menu[] =
	{
		{ 0x0302u, "厂家配置" 	},
		{ 0x0804u,   "调 试"	}, 	{ 0x0814u, "采样累计" 	},
		{ 0x1002u, "型号配置" 	}, 	{ 0x1016u,   "显 示" 	},	//	{ 0x0408u, "压力保护" },
		{ 0x1802u, "名称配置"	}, 	{ 0x1814u, "程序版本" 	},

	};
	uint8_t	item = 1u;

	do
	{
		cls();
		Menu_Redraw( menu );
		item = Menu_Select( menu, item, NULL );

		switch( item )
		{
			case 1:
				menu_FactoryDebug();
				break;
			case 3:
				menu_Instr_Config();
				break;
			case 5:
				menu_ExName();
				break;
			case 2:
				menu_Sample_Sum();
				break;
			case 4:
				menu_ConfigureDisplay();
				break;
			case 6:
				ShowEdition_Inner();
				getKey();
				break;	//	HCBoxPIDParament();	break;
			default:
				break;
		}
	}
	while( enumSelectESC != item );
}

/********  (C) COPYRIGHT 2014 青岛金仕达电子科技有限公司  **** End Of File ****/
