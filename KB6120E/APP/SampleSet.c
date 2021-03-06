/**************** (C) COPYRIGHT 2014 青岛金仕达电子科技有限公司 ****************
* 文 件 名: Setup.C
* 创 建 者: Dean
* 描  述  : KB-6120E 采样设置程序
* 最后修改: 2013年12月18日
*********************************** 修订记录 ***********************************
* 版  本:
* 修订人:
*******************************************************************************/
#include "AppDEF.H"
#include "BMP.H"
/********************************** 数据定义 **********************************/
struct	uSampleSet	SampleSet[SP_Max];	//	采样设置
extern  void	Show_std_clock( void );
/********************************** 功能说明 ***********************************
*  设置采样任务的控制参数，原则上设置好的参数，每次采样不需要变动。
*******************************************************************************/
static	BOOL	setup_x( enum	enumSamplerSelect SamplerSelect, struct uMenu const menu[] )	//SHI\R24
{
	uint8_t	item = 0u;
	BOOL	changed = FALSE;
	static	struct uMenu  const  menuL[] =
	{
		{ 0x0401u, "设 置  " },
		{ 0x0606u, "设置" },	//	设置默认采样参数，原则上不需要每次都修改
		{ 0x0C06u, "采样" },	//	调整每次采样都需要确认的参数，并启动采样
		{ 0x1206u, "查询" },	//	查看对应采样器的采样记录文件，及打印功能
		{ 0x1806u, "维护" }
	};
	cls();
	Menu_Redraw( menuL );
	WBMP( 0x1818, menuL[1].yx-0x04, SAMPLESET );
	WBMP( 0x1818, menuL[2].yx-0x04, SAMPLE );
	WBMP( 0x1818, menuL[3].yx-0x04, QUERY );
	WBMP( 0x1818, menuL[4].yx-0x04, MAINTAIN );

	do
	{

		Part_cls();
		Menu_Redraw( menu );
		SamplerTypeShow( 0x010Eu );
		ShowTIME( 0x061Cu, SampleSet[SamplerSelect].sample_time );
		ShowTIME( 0x0C1Cu, SampleSet[SamplerSelect].suspend_time );
		ShowI16U( 0x121Cu, SampleSet[SamplerSelect].set_loops, 0x0200u, NULL );
		ShowI16U( 0x181Cu, Configure.SetFlow[SamplerSelect], 0x0301u, "L/m" );

		item = Menu_Select2( menu, item + 1u, FALSE, Show_std_clock );

		switch( item )
		{
			case 1:

				if ( EditTIME( 0x061Cu, &( SampleSet[SamplerSelect].sample_time )))
				{
					changed = TRUE;
				}

				break;
			case 2:

				if ( EditTIME( 0x0C1Cu, &( SampleSet[SamplerSelect].suspend_time )))
				{
					changed = TRUE;
				}

				break;
			case 3:

				if ( EditI16U( 0x121Cu, &( SampleSet[SamplerSelect].set_loops ), 0x0200u ))
				{
					changed = TRUE;
				}

				break;
			case 4:

				if ( EditI16U( 0x181Cu, &( Configure.SetFlow[SamplerSelect] ), 0x0301u ))
				{
					switch( SamplerSelect )
					{
						default:
							break;
						case SP_R24_A:
						case SP_R24_B:

							if( Configure.SetFlow[SamplerSelect] < 1 )
								Configure.SetFlow[SamplerSelect] = 1;

							if( Configure.SetFlow[SamplerSelect] > 10 )
								Configure.SetFlow[SamplerSelect] = 10;

							break;
						case SP_SHI_C:
						case SP_SHI_D:

							if(  Configure.PumpType[SamplerSelect] == enumOrifice_1 )
							{
								if( Configure.SetFlow[SamplerSelect] < 1 )
									Configure.SetFlow[SamplerSelect] = 1;

								if( Configure.SetFlow[SamplerSelect] > 10 )
									Configure.SetFlow[SamplerSelect] = 10;
							}
							else
							{
								if( Configure.SetFlow[SamplerSelect] < 5 )
									Configure.SetFlow[SamplerSelect] = 5;

								if( Configure.SetFlow[SamplerSelect] > 20 )
									Configure.SetFlow[SamplerSelect] = 20;
							}
					}

					ConfigureSave();
				}

				break;

			case enumSelectXCH:
				SamplerTypeSwitch();
				return	FALSE;	//	返回到上级菜单，切换到其他采样器
		}

		if ( changed )
		{
			SampleSetSave();
		}

	}
	while( enumSelectESC != item );

	return	TRUE;
}

static	BOOL	setup_TSP( enum	enumSamplerSelect SamplerSelect, struct uMenu const menu[] )
{
	uint8_t	item = 0u;
	BOOL	changed = FALSE;
// 	BOOL	need_redraw = TRUE;
	static	struct uMenu  const  menuL[] =
	{
		{ 0x0401u, "设 置  " },
		{ 0x0606u, "设置" },	//	设置默认采样参数，原则上不需要每次都修改
		{ 0x0C06u, "采样" },	//	调整每次采样都需要确认的参数，并启动采样
		{ 0x1206u, "查询" },	//	查看对应采样器的采样记录文件，及打印功能
		{ 0x1806u, "维护" }
	};
	cls();
	Menu_Redraw( menuL );
	WBMP( 0x1818, menuL[1].yx-0x04, SAMPLESET );
	WBMP( 0x1818, menuL[2].yx-0x04, SAMPLE );
	WBMP( 0x1818, menuL[3].yx-0x04, QUERY );
	WBMP( 0x1818, menuL[4].yx-0x04, MAINTAIN );

	do
	{

		SamplerTypeShow( 0x010Eu );
		Part_cls();
		Menu_Redraw( menu );

		ShowTIME( 0x081Cu, SampleSet[SamplerSelect].sample_time );
		ShowTIME( 0x101Cu, SampleSet[SamplerSelect].suspend_time );
		ShowI16U( 0x181Cu, SampleSet[SamplerSelect].set_loops, 0x0200u, NULL );

		item = Menu_Select2( menu, item + 1u, FALSE, Show_std_clock );

		switch( item )
		{
			case 1:

				if ( EditTIME( 0x081Cu, &( SampleSet[SamplerSelect].sample_time )))
				{
					changed = TRUE;
				}

				break;
			case 2:

				if ( EditTIME( 0x101Cu, &( SampleSet[SamplerSelect].suspend_time )))
				{
					changed = TRUE;
				}

				break;
			case 3:

				if ( EditI16U( 0x181Cu, &( SampleSet[SamplerSelect].set_loops ), 0x0200u ))
				{
					changed = TRUE;
				}

				break;

			case enumSelectXCH:
				SamplerTypeSwitch();
				return	FALSE;	//	返回到上级菜单，切换到其他采样器
		}

		if ( changed )
		{
			SampleSetSave();
		}
	}
	while ( enumSelectESC != item );

	return	TRUE;
}




void	menu_SampleSetup( void )
{
	static	struct uMenu  const  menu_x[] =
	{
		{ 0x0401u, "设 置  " },
		{ 0x060Eu, "采样时间" },
		{ 0x0C0Eu, "间隔时间" },
		{ 0x120Eu, "采样次数" },
		{ 0x180Eu, "采样流量" },
	};

	static	struct uMenu  const  menu_TSP[] =
	{
		{	0x0301u, "设 置  " },
		{ 0x080Eu, "采样时间" },
		{ 0x100Eu, "间隔时间" },
		{ 0x180Eu, "采样次数" },
	};

	BOOL	done = FALSE;

	do
	{
		monitor();

		switch ( SamplerSelect )
		{
			case SP_TSP:
				done = setup_TSP( SamplerSelect, menu_TSP );
				break;
			case SP_R24_A:
			case SP_R24_B:
			case SP_SHI_C:
			case SP_SHI_D:
				done = setup_x( SamplerSelect, menu_x );
				break;
			default:
				break;
		}
	}
	while( ! done );
}

/********************************** 功能说明 ***********************************
*  设置启动采样的控制参数，原则上每次采样前需要重新设置的参数，并启动采样
*******************************************************************************/
static	BOOL	SampleStart( enum enumSamplerSelect SamplerSelect )
{
	static	struct uMenu  const  menu_Accurate[] =
	{
		{ 0x0301u, "采 样  " },
		{ 0x100Eu, "开始时间" },
		{ 0x1814u, "调零" },
		{ 0x181Cu, "启动" },
	};

	static	struct uMenu  const  menu_Delay[] =
	{
		{ 0x0301u, "采 样  " },
		{ 0x100Eu, "延时时间" },
		{ 0x1814u, "调零" },
		{ 0x181Cu, "启动" },
	};
	static	struct uMenu  const  menuL[] =
	{
		{ 0x0401u, "采 样  " },
		{ 0x0606u, "设置" },	//	设置默认采样参数，原则上不需要每次都修改
		{ 0x0C06u, "采样" },	//	调整每次采样都需要确认的参数，并启动采样
		{ 0x1206u, "查询" },	//	查看对应采样器的采样记录文件，及打印功能
		{ 0x1806u, "维护" }
	};

	static	struct uMenu const * menu;
	uint8_t	item = 1u;
	BOOL	need_redraw = TRUE;

	BOOL	changed_Delay = FALSE;
	BOOL	changed = FALSE;

	cls();
	Menu_Redraw( menuL );
	SamplerTypeShow( 0x010Eu );
	WBMP( 0x1818, menuL[1].yx-0x04, SAMPLESET );
	WBMP( 0x1818, menuL[2].yx-0x04, SAMPLE );
	WBMP( 0x1818, menuL[3].yx-0x04, QUERY );
	WBMP( 0x1818, menuL[4].yx-0x04, MAINTAIN );

	do
	{
		switch( Configure.Mothed_Delay )
		{
			case enumByAccurate:
				menu = menu_Accurate;
				break;
			case enumByDelay:
				menu = menu_Delay;
				break;
			default:
				break;
		}

		if ( need_redraw )
		{
			Part_cls();
			SamplerTypeShow( 0x010Eu );
			Menu_Redraw( menu );
			need_redraw = FALSE;
		}

		Menu_Item_Mask( menu, item );

		do
		{
			uint16_t	now_hour_min = (( get_Now() / 60 ) % 1440 );

			Show_std_clock();
			Lputs( 0x080Eu, "当前时间=" );
			ShowTIME( 0x081Cu, now_hour_min );	//	显示当前时间

			if ( enumByAccurate == Configure.Mothed_Delay )
			{
				if ( ! changed_Delay )
				{
					SampleSet[SamplerSelect].delayt = now_hour_min + 1u;
				}
			}

			ShowTIME( 0x101Cu, SampleSet[SamplerSelect].delayt );
		}
		while ( ! hitKey( 100u ));

		Menu_Item_Mask( menu, item );

		item = Menu_Select2( menu, item, FALSE, Show_std_clock );

		switch( item )
		{
			case 1:

				if ( EditTIME( 0x101Cu, &( SampleSet[SamplerSelect].delayt )))
				{
					changed_Delay = TRUE;
					changed = TRUE;
					SampleSetSave();
				}

				++item;
				break;
			case 2:
				need_redraw = TRUE;
				CalibrateZero_x( SamplerSelect );
				++item;
				//lint -fallthrough
				break;
			case 3:	//	启动采样

				if ( changed )
				{
					SampleSetSave();
				}

				Sampler_Start ( SamplerSelect );

				return	FALSE;	//	返回显示采样状态

			case enumSelectXCH:
				SamplerTypeSwitch();
				return	FALSE;
			case	enumSelectESC:
				break;
		}
	}
	while ( enumSelectESC != item );

	return	TRUE;	//	返回，显示上级菜单
}

// void	State_Finish( enum enumSamplerSelect SamplerSelect )
// {

// 	if( (	Q_Sampler[SamplerSelect].state	== state_FINISH ) && SampleShowState[SamplerSelect] )
// 	{
// 		SampleShowState[SamplerSelect] = FALSE;
// 		cls();
// 	// 	SamplerTypeShow( 0x0102u );
// 		WBMP( 0x0290,0x0502, STROCK);
// 		WBMP( 0x0290,0x0514, STROCK);

// 		switch( SamplerSelect )
// 		{
// 		case	SP_TSP: Lputs( 0x0102,  "TSP采样");	break;
// 		case	SP_R24ASP_R24B: Lputs( 0x0102, "日均采样");	break;
// 		case	SP_SHI_CSP_SHI_D: Lputs( 0x0102, "时均采样");	break;
// 		case	Q_AIR: Lputs( 0x0102, "大气采样");	break;
// 		}
// 		Lputs( 0x0A0C,	"采样完成!");
// 		Lputs( 0x0F03,  "按确认键查询采样结果!");
// 		do
// 		{
// 			Show_std_clock();
// 		}while( !hitKey( 50 ) );

// 		switch( getKey() )
// 		{
// 		case K_OK:	menu_SampleQuery();	break;
// 		default:	break;
// 		}
// 	}
// }
/********************************** 功能说明 ***********************************
*  主菜单访问设置程序的接口
*******************************************************************************/
void	menu_SampleStart( void )
{
	BOOL	done = FALSE;

	do
	{
		monitor();
// 		State_Finish(	SamplerSelect );
		done = SampleStart( SamplerSelect );
	}
	while ( ! done );
}

/********  (C) COPYRIGHT 2014 青岛金仕达电子科技有限公司  **** End Of File ****/
// void	SetupFlow_SHI( void )
// {
// 	static	struct uMenu  const  menu[] =
// 	{
// 		{ 0x0201u, "流量设置" },
// 		{ 0x0C0Eu, "流量C" },
// 		{ 0x140Eu, "流量D" },
// 	};
// 	static	struct uMenu  const  menuL[] =
// 	{
// 		{ 0x0401u, "      " },
// 		{ 0x0606u, "设置" },	//	设置默认采样参数，原则上不需要每次都修改
// 		{ 0x0C06u, "采样" },	//	调整每次采样都需要确认的参数，并启动采样
// 		{ 0x1206u, "查询" },	//	查看对应采样器的采样记录文件，及打印功能
// 		{ 0x1806u, "维护" }
// 	};
// 	uint8_t item = 0u;
//
// 	uint16_t	cflow = Configure.SetFlow[SP_SHI_C];
// 	uint16_t	dflow = Configure.SetFlow[SP_SHI_D];
// 	BOOL	changed = FALSE;
// 	BOOL	need_redraw = TRUE;
//
// 	do {
// 		if ( need_redraw )
// 		{
// 			cls();
// 			Menu_Redraw( menuL );
// 			SamplerTypeShow( 0x010Eu );
// 			WBMP( 0x1818, menuL[1].yx-0x04, SAMPLESET );
// 			WBMP( 0x1818, menuL[2].yx-0x04, SAMPLE );
// 			WBMP( 0x1818, menuL[3].yx-0x04, QUERY );
// 			WBMP( 0x1818, menuL[4].yx-0x04, MAINTAIN );
// 			Part_cls();
// 			Menu_Redraw( menu );
// 			need_redraw = FALSE;
// 		}
// 		ShowI16U( 0x0C1Au, cflow, 0x0301u, "L/m" );
// 		ShowI16U( 0x141Au, dflow, 0x0301u, "L/m" );

// 		item = Menu_Select2( menu, item + 1u, FALSE, Show_std_clock );
// 		switch ( item )
// 		{
// 		case 1:
// 			if ( EditI16U( 0x0C1Au, &cflow, 0x0301u ))
// 			{
// 				switch ( Configure.PumpType[SP_SHI_C] )
// 				{
// 				case enumOrifice_1:
// 					if ( cflow <  1 )  { cflow =  1u; }
// 					if ( cflow > 10u ) { cflow = 10u; }
// 					break;
// 				case enumOrifice_2:
// 					if ( cflow <  5u ) { cflow =  5u; }
// 					if ( cflow > 20u ) { cflow = 20u; }
//
// 					break;
// 				default:
// 					break;
// 				}
// 				changed = TRUE;
// 			}
// 			break;
// 		case 2:
// 			if ( EditI16U( 0x141Au, &dflow, 0x0301u ))
// 			{
// 				switch ( Configure.PumpType[SP_SHI_D] )
// 				{
// 				case enumOrifice_1:
// 					if ( dflow <  1u ) { dflow =  1u; }
// 					if ( dflow > 10u ) { dflow = 10u; }
// 					break;
// 				case enumOrifice_2:
// 					if ( dflow <  5u ) { dflow =  5u; }
// 					if ( dflow > 20u ) { dflow = 20u; }

// 					break;
// 				default:
// 					break;
// 				}
// 				changed = TRUE;
// 			}
// 			break;
// 		case enumSelectESC:
// 			if ( changed )
// 			{
// 				Configure.SetFlow[SP_SHI_C] = cflow;
// 				Configure.SetFlow[SP_SHI_D] = dflow;
// 				switch( MsgBox( "保存流量设置?", vbYesNoCancel + vbDefaultButton3 ))
// 				{
// 				case vbYes:
// 						ConfigureSave();
// 						break;
// 				case vbNo:
// 						ConfigureLoad();
// 						break;
// 				case vbCancel:
// 						item = 1u;
// 						break;
// 				default:
// 						break;
// 				}
// 			}
// 			need_redraw = TRUE;
// 			break;
// 		default:
// 			break;
// 		}
// 	} while ( enumSelectESC != item );
//
// }

#define	LOBYTE(__w)		((uint8_t)((__w)&0x0FF))
#define	HIBYTE(__w)		((uint8_t)(((__w)>>8)&0x0FF))

// static	BOOL	setup_R24( struct uMenu  const  menu[], const CHAR * szPrompt )
// {
// 	uint8_t	item = 0u;
// 	BOOL	changed = FALSE;
// 	BOOL	need_redraw = TRUE;
// 	static	struct uMenu  const  menuL[] =
// 	{
// 		{ 0x0401u, "      " },
// 		{ 0x0606u, "设置" },	//	设置默认采样参数，原则上不需要每次都修改
// 		{ 0x0C06u, "采样" },	//	调整每次采样都需要确认的参数，并启动采样
// 		{ 0x1206u, "查询" },	//	查看对应采样器的采样记录文件，及打印功能
// 		{ 0x1806u, "维护" }
// 	};

// 	do {
// 		if ( need_redraw )
// 		{
// 			cls();
// 			Menu_Redraw( menuL );
// 			SamplerTypeShow( 0x010Eu );
// 			WBMP( 0x1818, menuL[1].yx-0x04, SAMPLESET );
// 			WBMP( 0x1818, menuL[2].yx-0x04, SAMPLE );
// 			WBMP( 0x1818, menuL[3].yx-0x04, QUERY );
// 			WBMP( 0x1818, menuL[4].yx-0x04, MAINTAIN );
// 			Part_cls();
// 			Menu_Redraw( menu );
// 			need_redraw = FALSE;
// 		}
//
// 		ShowTIME( 0x061Cu, SampleSet[SamplerSelect].sample_time );

// 		ShowTIME( 0x0F1Cu, SampleSet[SamplerSelect].suspend_time );
// 		Lputs(0x140Eu, szPrompt );
//
// 		ShowI16U( 0x1816u, SampleSet[SamplerSelect].set_loops, 0x0200u, NULL );

// 		item = Menu_Select2( menu, item + 1u, FALSE, Show_std_clock );

// 		switch ( item )
// 		{
// 		case 1:
// 			if ( EditTIME( 0x061Cu, &SampleSet[SamplerSelect].sample_time ))
// 			{
// 				if ( SampleSet[SamplerSelect].sample_time < SampleSet[SamplerSelect].sample_time )
// 				{
// 					SampleSet[SamplerSelect].sample_time = SampleSet[SamplerSelect].sample_time;
// 					beep();
// 				}
// 				else
// 				{
// 					changed = TRUE;
// 				}
// 			}
// 			break;
// 		case 2:
// 			if ( EditTIME( 0x0A1Cu, &SampleSet[SamplerSelect].sample_time ))
// 			{
// 				if ( SampleSet[SamplerSelect].sample_time < SampleSet[SamplerSelect].sample_time )
// 				{
// 					SampleSet[SamplerSelect].sample_time = SampleSet[SamplerSelect].sample_time;
// 					beep();
// 				}
// 				else
// 				{
// 					changed = TRUE;
// 				}
// 			}
// 			break;
// 		case 3:
// 			if ( EditTIME( 0x0F1Cu, &SampleSet[SamplerSelect].suspend_time ))
// 			{
// 				changed = TRUE;
// 			}
// 			break;

// 		case 4:
// 			if ( EditI16U( 0x1816u, &( SampleSet[SamplerSelect].set_loops ), 0x0200u ))
// 			{
// 				changed = TRUE;
// 			}
// 			break;
//
// 		case 5:
// 		//	对于 4路小流量采样器：设置C/D 时均 采样流量（A/B采样流量是相对固定的）
// 		//	对于 2路小流量采样器：设置A/B 采样流量
// 			SetupFlow_SHI();
// 			need_redraw = TRUE;
// 			break;
//
// 		case enumSelectXCH:
// 			SamplerTypeSwitch();
// 			return	FALSE;
// 		}
// 	} while ( enumSelectESC != item );

// 	if ( changed )
// 	{
// 		SampleSetSave();
// 	}
// 	return	TRUE;
// }
