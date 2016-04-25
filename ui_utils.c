/*
 * ui_utils.c
 *
 *  Created on: 2015. 8. 19.
 *      Author: jeki
 */
#include "ui_utils.h"
#include "utils.h"
#include "ui_toolbox_pic.h"
#include <string.h>

/*Canvas Attributes
 * layout_num   => return int of layout_num, this can be loaded from config file
 *
 * ucnt         => return int of number of unit in one layout
 * bcwh         => return int of number of block in 1 unit (horizontal, vertical).
 *
 * uxy1         => return x, y (int) for cdcanvas unit 1
 * uxy2         => return x, y (int) for cdcanvas unit 2
 * uxy3         => return x, y (int) for cdcanvas unit 3
 * uxy4         => return x, y (int) for cdcanvas unit 4
 *
 * uwh1        => return width, height (int) for cdcanvas unit 1
 * uwh2        => return width, height (int) for cdcanvas unit 2
 * uwh3        => return width, height (int) for cdcanvas unit 3
 * uwh4        => return width, height (int) for cdcanvas unit 4
 *
 * bwh1        => return int size (width, height) for unit block 1
 * bwh2        => return int size (width, height) for unit block 2
 * bwh3        => return int size (width, height) for unit block 3
 * bwh4        => return int size (width, height) for unit block 4
 *
 * img1        => imImage for unit 1
 * img2        => imImage for unit 2
 * img3        => imImage for unit 3
 * img4        => imImage for unit 4
 *
 * ims1        => small imImage for unit 1 (img / 4 sized) for normal image buffer
 * ims2        => small imImage for unit 2 (img / 4 sized) for normal image buffer
 * ims3        => small imImage for unit 3 (img / 4 sized) for normal image buffer
 * ims4        => small imImage for unit 4 (img / 4 sized) for normal image buffer
 *
 * imf1        => imImage flag for unit 1
 * imf2        => imImage flag for unit 2
 * imf3        => imImage flag for unit 3
 * imf4        => imImage flag for unit 4
 *
 * block_pointer => return block toggle pointer for grid of normal /expanded grid.
 */
extern Ihandle *checkChildName(Ihandle *child, const char *child_name);

void layout_update(Ihandle *ih, int laytxt_val)
{
	int clw,clh,cleh;
	float posy;
	Ihandle* laylbl = IupGetDialogChild(ih, "LAYLABEL");
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
	Ihandle* canvaslayout = IupGetDialogChild(ih, "CANVASLAYOUT");
	IupGetIntInt(canvaslayout, "DRAWSIZE", &clw,&clh);

	if(laytxt_val < 11)
	{
		cleh = clw * 3/4;
		posy = (float)((laytxt_val - 1) * cleh) / (float)cleh/10.0f;        // top
		IupSetFloat(canvaslayout, "POSY", posy);

		IupSetInt(laylbl, "TITLE", laytxt_val);
		IupSetInt(canvas, "layout_num", laytxt_val);

		setUnitLayoutImages(canvas);
		setUnitLayoutSize(canvas);
		IupSetInt(canvas,"resize_update",1);
		IupUpdate(canvas);
	}
}

void drawPropertiesShow(Ihandle *canvas)
{
	int i, canvas_width, canvas_height;
	cdCanvas* cd_canvas = (cdCanvas*)IupGetAttribute(canvas, "cdCanvas");
	int scrollbar_size = IupGetInt(NULL, "SCROLLBARSIZE");
	IupGetIntInt(canvas, "DRAWSIZE", &canvas_width, &canvas_height);
	char strTmp[10];
	if(IupGetInt(canvas, "properties_show"))
	{
		time_t rawtime;
		struct tm * timeinfo;
		char timebuffer[13];

		time (&rawtime);
		timeinfo = localtime (&rawtime);

		strftime (timebuffer,13,"%H:%M:%S",timeinfo);

		cdCanvasForeground(cd_canvas, cdEncodeColor(100,100,100));
		cdCanvasBox(cd_canvas, 0, scrollbar_size + 220,
				canvas_height - scrollbar_size - 200, canvas_height - scrollbar_size + 13);

		cdCanvasForeground(cd_canvas, cdEncodeColor(0,0,0));
		cdCanvasFont(cd_canvas, "Arial", 0, 8);
		cdCanvasText(cd_canvas, 1, canvas_height - scrollbar_size + 2, "time: ");
		cdCanvasText(cd_canvas, 25, canvas_height - scrollbar_size + 2, IupGetAttribute(canvas, "timestart"));
		cdCanvasText(cd_canvas, 70, canvas_height - scrollbar_size + 2, "-");
		cdCanvasText(cd_canvas, 75, canvas_height - scrollbar_size + 2, timebuffer);

		cdCanvasForeground(cd_canvas, cdEncodeColor(255,255,255));
		cdCanvasFont(cd_canvas, "Arial", 0, 10);
		cdCanvasText(cd_canvas, scrollbar_size + 210, canvas_height - scrollbar_size - 10, "x");

		cdCanvasForeground(cd_canvas, cdEncodeColor(255,255,255));
		cdCanvasFont(cd_canvas, "Arial", 0, 15);
		cdCanvasText(cd_canvas, 1, canvas_height - scrollbar_size - 20, "Properties:");
		cdCanvasText(cd_canvas, scrollbar_size, canvas_height - scrollbar_size - 45, "Machine");
		cdCanvasText(cd_canvas, scrollbar_size, canvas_height - scrollbar_size - 70, "Layout");
		cdCanvasText(cd_canvas, scrollbar_size, canvas_height - scrollbar_size - 95, "Sequence");
		cdCanvasText(cd_canvas, scrollbar_size, canvas_height - scrollbar_size - 120, "Speed factor");
		cdCanvasText(cd_canvas, scrollbar_size, canvas_height - scrollbar_size - 145, "Dwell");
		cdCanvasText(cd_canvas, scrollbar_size, canvas_height - scrollbar_size - 170, "Delay (ms)");
		cdCanvasText(cd_canvas, scrollbar_size, canvas_height - scrollbar_size - 195, "Scene");

		for(i = 0; i < 7; i++)
		{
			cdCanvasText(cd_canvas, 150, canvas_height - scrollbar_size - 45 - 25 * i, "=");
		}

		cdCanvasText(cd_canvas, 170, canvas_height - scrollbar_size - 45,
				IupGetAttribute(IupGetDialogChild(canvas,"MACHNOTEXT"), "VALUE"));
		cdCanvasText(cd_canvas, 170, canvas_height - scrollbar_size - 70,
				IupGetAttribute(IupGetDialogChild(canvas,"LAYLABEL"), "TITLE"));
		cdCanvasText(cd_canvas, 170, canvas_height - scrollbar_size - 95,
				IupGetAttribute(IupGetDialogChild(canvas,"SEQNUMTEXT"), "VALUE"));
		cdCanvasText(cd_canvas, 170, canvas_height - scrollbar_size - 120,
				IupGetAttribute(IupGetDialogChild(canvas,"SPEEDFACTORTEXTSHOW"), "VALUE"));
		cdCanvasText(cd_canvas, 170, canvas_height - scrollbar_size - 145,
				IupGetAttribute(IupGetDialogChild(canvas,"DWELLTEXT"), "VALUE"));
		sprintf(strTmp, "%d", IupGetInt(IupGetDialogChild(canvas,"DELAYLABEL"), "TITLE"));
		cdCanvasText(cd_canvas, 170, canvas_height - scrollbar_size - 170,strTmp);
		cdCanvasText(cd_canvas, 170, canvas_height - scrollbar_size - 195,
						IupGetAttribute(IupGetDialogChild(canvas,"SEQLABELCUR"), "TITLE"));

		if(IupGetInt(canvas, "msgflag"))
		{
			cdCanvasFont(cd_canvas, "Arial", 0, 15);
			cdCanvasForeground(cd_canvas, cdEncodeColor(255,0,0));
			cdCanvasText(cd_canvas, 1, canvas_height - scrollbar_size - 220,
					IupGetAttribute(canvas,"msgtext"));
		}
	}
	else
	{
		cdCanvasForeground(cd_canvas, cdEncodeColor(100,100,100));
		cdCanvasBox(cd_canvas, 1, 8,
				canvas_height - scrollbar_size - 12, canvas_height - scrollbar_size);

		cdCanvasForeground(cd_canvas, cdEncodeColor(255,255,255));
		cdCanvasFont(cd_canvas, "Times", CD_BOLD, 10);
		cdCanvasText(cd_canvas, 1, canvas_height - scrollbar_size - 10, ">");
	}
}

void showMsgDlg(Ihandle *parent_dlg, char *msgtype, char *err_title, char *err_msg)
{
	static unsigned char display_flag=0;
	if(!display_flag)
	{
		display_flag=1;
		Ihandle *mesghandle = IupMessageDlg();
		//	IupSetHandle("MESGDIALOG", mesghandle);
		IupSetAttribute(mesghandle, "DIALOGTYPE", msgtype);
		IupSetAttribute(mesghandle, "VALUE",err_msg);
		IupSetAttribute(mesghandle, "TITLE",err_title);
		//	IupSetAttributeHandle(mesghandle, "PARENTDIALOG", parent_dlg);
		IupPopup(mesghandle, IUP_CURRENT, IUP_CURRENT);
		IupDestroy(mesghandle);
		display_flag=0;
	}
}

void checkComportBuf(Ihandle *ih, int comnum, int n, unsigned char *bufin, int detectcnt)
{
	Ihandle* comportlist = IupGetDialogChild(ih, "COMPORTLIST");
	Ihandle* matmculabel = IupGetDialogChild(ih,"MATMCULABEL");
	Ihandle* comportbutton = IupGetDialogChild(ih, "COMPORTBUTTON");
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
	static unsigned char buf[4100];
	static int bufptr = 0;
//	static unsigned int monitorCnt = 0;
	int maxbufptr = 0;
	int comportmode = IupGetInt(ih, "COMPORTMODE");
	int i, j;
	static unsigned char ACKNum=0;
	static int detectedmatch = 0;

	if(n > 0)
	{
		bufin[n] = 0; /* always put a "null" at the end of a string! */

		  for(j = 0; j < n; j++)
		  {
			if(bufin[j] == '\r')
			{
				maxbufptr = bufptr;
				buf[bufptr] = 0;
				bufptr = 0;
			}
			else if(bufin[j] != '\n')
			{
				buf[bufptr] = bufin[j];
				bufptr++;
				maxbufptr = 0;
			}

			if(bufptr > 4096)
			{
				bufptr = 0;
				maxbufptr = bufptr;
			}

		  for(i=0; i < maxbufptr; i++)
		  {
			if(buf[i] == '!') //nack
			{
				if(getIntHexCmpChar((char*)buf,"!3537", i, 1)) sendComPortPuts(comnum-1, "?3537FD\r");
				if(getIntHexCmpChar((char*)buf,"!3444", i, 1)) sendComPortPuts(comnum-1, "?34448F\r");
				if(getIntHexCmpChar((char*)buf,"!3443", i, 1)) sendComPortPuts(comnum-1, "?344388\r");
				if(getIntHexCmpChar((char*)buf,"!3531", i, 1)) sendComPortPuts(comnum-1, "?3531FB\r");
				if(getIntHexCmpChar((char*)buf,"!3538", i, 1)) sendComPortPuts(comnum-1, "?3538F2\r");
				if(getIntHexCmpChar((char*)buf,"!3533", i, 2))
				{
					unsigned int seqnum = getIntHexCmpChar((char*)buf,"!3533", i, 2);
					unsigned char strTmp[4];
					unsigned char strTmp2[12];
					sprintf((char*)strTmp,"%c%c%c%c",0x35,0x33,((seqnum >> 8) & 0xff), (seqnum & 0xff));
					sprintf((char*)strTmp2, "?%02X%02X%02X%02X%02X\r", strTmp[0], strTmp[1], strTmp[2], strTmp[3], getCheckSumCharArr(strTmp,0,4));
					sendComPortPuts(comnum-1, (char*)strTmp2);
				}
			}

			if(buf[i] == '^') //ack
			{
				switch(getIntHexCmpChar((char*)buf,"^34",i, 1))
				{
					case 0x44: ACKNum = ACKMACHINENO; break;
					case 0x43: ACKNum = ACKLAYNUM; break;
					default: break;
				}
				switch(getIntHexCmpChar((char*)buf,"^35",i, 1))
				{
					case 0x37: ACKNum = ACKWRITEEN; break;
					case 0x31: ACKNum = ACKSEQNUM; break;
					case 0x38: ACKNum = ACKSPEEDFACTOR; break;
					case 0x33: ACKNum = ACKSCENEMAP; break;
					default: break;
				}

				if(ACKNum)
					if(comportmode == COMPORTSENDSEQ)
					{
						char *strTmpLine = NULL;
						char *comseqtmpfilename = getTmpFileName();
						switch(ACKNum)
						{
							case ACKWRITEEN:
							{
								IupSetAttribute(IupGetHandle("PROGRESSBARDIALOG"), "DESCRIPTION", "Preparing Storage");
								IupSetAttribute(IupGetHandle("PROGRESSBARDIALOG"), "INC", NULL);
								ACKNum = 0;
								break;
							}
							case ACKMACHINENO: // get machine num send layout num
							{
								sendComPortPuts(comnum - 1, getFileStrLine(comseqtmpfilename, 2));
								sendComPortByte(comnum - 1, 13);
								IupSetAttribute(IupGetHandle("PROGRESSBARDIALOG"), "DESCRIPTION", "Send Layout No");
								IupSetAttribute(IupGetHandle("PROGRESSBARDIALOG"), "INC", NULL);
								ACKNum = 0;
								break;
							}
							case ACKLAYNUM:// get layout num send seq num
							{
								sendComPortPuts(comnum - 1, getFileStrLine(comseqtmpfilename, 3));
								sendComPortByte(comnum - 1, 13);
								IupSetAttribute(IupGetHandle("PROGRESSBARDIALOG"), "DESCRIPTION", "Send Seq No");
								IupSetAttribute(IupGetHandle("PROGRESSBARDIALOG"), "INC", NULL);
								ACKNum = 0;
								break;
							}
							case ACKSEQNUM: // get seq num send speed factor
							{
								sendComPortPuts(comnum - 1, getFileStrLine(comseqtmpfilename, 4));
								sendComPortByte(comnum - 1, 13);
								IupSetAttribute(IupGetHandle("PROGRESSBARDIALOG"), "DESCRIPTION", "Send Speed Factor No");
								IupSetAttribute(IupGetHandle("PROGRESSBARDIALOG"), "INC", NULL);
								ACKNum = 0;
								break;
							}
							case ACKSPEEDFACTOR:// get speed factor star send scene
							{
								sendComPortPuts(comnum - 1, getFileStrLine(comseqtmpfilename, 5));
								sendComPortByte(comnum - 1, 13);
								IupSetStrf(IupGetHandle("PROGRESSBARDIALOG"), "DESCRIPTION", "Send Scene %d of %d", 1, IupGetInt(IupGetDialogChild(ih, "SEQLABEL"), "TITLE"));
								IupSetAttribute(IupGetHandle("PROGRESSBARDIALOG"), "INC", NULL);
								ACKNum = 0;
								break;
							}
							case ACKSCENEMAP:// send scene
							{
								unsigned int seq_val = getIntHexChar((char*)buf,i + 5,2);
//								printf("scene = %s\n", getFileStrLine(comseqtmpfilename, 5 + seq_val));
								sendComPortPuts(comnum - 1, getFileStrLine(comseqtmpfilename, 5 + seq_val));
								sendComPortByte(comnum - 1, 13);
								IupSetAttribute(IupGetHandle("PROGRESSBARDIALOG"), "INC", NULL);
								IupSetStrf(IupGetHandle("PROGRESSBARDIALOG"), "DESCRIPTION", "Send Scene %d of %d", seq_val, IupGetInt(IupGetDialogChild(ih, "SEQLABEL"), "TITLE"));
								if(seq_val == IupGetInt(IupGetDialogChild(ih, "SEQLABEL"), "TITLE"))
								{
									IupSetInt(ih, "COMPORTMODE", 0);
									remove(comseqtmpfilename);
									IupHide(IupGetHandle("PROGRESSBARDIALOG"));
									IupSetAttribute(IupGetDialog(ih), "ACTIVE", "YES");
									comportDisableRTS(comnum - 1);
								}
								ACKNum = 0;
								break;
							}
							default: break;
						}
						if(strTmpLine != NULL) free(strTmpLine);
						if(comseqtmpfilename !=NULL) free(comseqtmpfilename);
					}
			}

			if(buf[i] == '*')
			{
				if(getIntHexCmpChar((char*)buf,"*ffff",i, 2) == 0xffff)
				{
					if(comportmode == COMPORTREADFLASH)
					{
						IupSetInt(ih, "COMPORTMODE", 0);
						comportDisableRTS(comnum - 1);
						//enable control related with playback
						enableSeqControl(ih);
						enableComportSeqControl(ih);
						IupSetAttribute(IupGetDialogChild(ih, "COMPORTREADFLASHBUTTON"), "TITLE", "Read");
					}
				}

				if(getIntHexCmpChar((char*)buf,"*3537",i, 1) == 0xff)
				{
					if(comportmode == COMPORTSENDSEQ)//after get ack write enable confirmation, send machine num
					{
						char *comseqtmpfilename = getTmpFileName();
						sendComPortPuts(comnum - 1, getFileStrLine(comseqtmpfilename, 1));
						sendComPortByte(comnum - 1, 13);
						IupSetAttribute(IupGetHandle("PROGRESSBARDIALOG"), "DESCRIPTION", "Send Machine No");
						IupSetAttribute(IupGetHandle("PROGRESSBARDIALOG"), "INC", NULL);
						if(comseqtmpfilename !=NULL) free(comseqtmpfilename);
					}
				}

				if(getIntHexCmpChar((char*)buf,"*3444",i, 1)) //machine number
				{
					unsigned int machnoval = getIntHexCmpChar((char*)buf,"*3444",i, 1);
					if(comportmode == COMPORTMONITOR)
					{
						if(machnoval != IupGetInt(IupGetDialogChild(ih, "MACHNOTEXT"), "VALUE")) IupSetInt(IupGetDialogChild(ih, "MACHNOTEXT"), "VALUE",machnoval);
					}
					else if(comportmode == COMPORTREADFLASH)
					{
						IupSetInt(IupGetDialogChild(ih, "MACHNOTEXT"), "VALUE",machnoval);
						sendComPortPuts(comnum - 1, "?344388\r");
					}
					else if(ACKNum == ACKMACHINENO)
					{
	//					if(comportmode == COMPORTSENDSEQ) // get machine num send layout num
	//					{
	//						sendComPortPuts(comnum - 1, getFileStrLine(comseqtmpfilename, 2));
	//						sendComPortByte(comnum - 1, 13);
	//					}
	//					else
						ACKNum = 0;
					}
					if(detectcnt)
					{
//						printf("detected machine num\n");
						IupSetInt(IupGetDialogChild(ih, "MACHNOTEXT"), "VALUE",getIntHexCmpChar((char*)buf,"*3444",i, 1));
//						IupSetStrf(comportlist, "1","%d", comnumdetect + 1);
						detectedmatch++;
	//					IupSetStrf(comportlist, "VALUESTRING","%d", comnumdetect + 1);
	//					IupSetAttribute(comportbutton, "ACTIVE", "YES");

	//					closeComPort(comnumdetect);
//						detectcnt = 1;
					}

				}

				if(getIntHexCmpChar((char*)buf,"*3443",i, 1)) //layout number
				{
					int laytxt_val = getIntHexCmpChar((char*)buf,"*3443",i, 1);

//					printf("found layout num\n");
					if(laytxt_val > 10)
					{
						IupSetInt(canvas, "msgflag", 1);
						IupSetAttribute(canvas, "msgtext", "Data Error Found: Please Rewrite MATRIX Sequence!");
					}
					else
					{
						if(IupGetInt(canvas, "msgflag"))
						{
							IupSetInt(canvas, "msgflag", 0);
							IupSetInt(canvas,"resize_update",1);
						}
					}

					if(comportmode == COMPORTMONITOR)
					{
						Ihandle* laylbl = IupGetDialogChild(ih, "LAYLABEL");
						if(laytxt_val > 10)
							IupSetInt(laylbl, "TITLE", laytxt_val);
						else if(laytxt_val != IupGetInt(laylbl, "TITLE"))
						{
							IupSetInt(laylbl, "TITLE", laytxt_val);
							IupSetInt(canvas, "layout_num", laytxt_val);
							layout_update(ih, laytxt_val);
						}
					}
					else if(comportmode == COMPORTREADFLASH)
					{
						Ihandle* laylbl = IupGetDialogChild(ih, "LAYLABEL");
						if(laytxt_val > 10)
							IupSetInt(laylbl, "TITLE", laytxt_val);
						else if(laytxt_val != IupGetInt(laylbl, "TITLE"))
						{
							IupSetInt(laylbl, "TITLE", laytxt_val);
							IupSetInt(canvas, "layout_num", laytxt_val);
							layout_update(ih, laytxt_val);
						}
						sendComPortPuts(comnum - 1, "?3531FB\r");
					}
					else if(ACKNum == ACKLAYNUM)
					{
	//					if(comportmode == COMPORTSENDSEQ)// get layout num send seq num
	//					{
	//						sendComPortPuts(comnum - 1, getFileStrLine(comseqtmpfilename, 3));
	//						sendComPortByte(comnum - 1, 13);
	//					}
	//					else
						ACKNum = 0;
					}
					if(detectcnt) detectedmatch++;
				}
				if(getIntHexCmpChar((char*)buf,"*3531",i, 1))
				{
					if(comportmode == COMPORTMONITOR)
					{
						Ihandle* seqnumtxt = IupGetDialogChild(ih, "SEQNUMTEXT");
						int seqnumtxtl_val = getIntHexCmpChar((char*)buf,"*3531",i, 1);
						if(seqnumtxtl_val != IupGetInt(seqnumtxt, "VALUE"))
							IupSetInt(seqnumtxt, "VALUE", seqnumtxtl_val);
					}
					else if(comportmode == COMPORTREADFLASH)
					{
						Ihandle* seqnumtxt = IupGetDialogChild(ih, "SEQNUMTEXT");
						int seqnumtxtl_val = getIntHexCmpChar((char*)buf,"*3531",i, 1);

						IupSetInt(seqnumtxt, "VALUE", seqnumtxtl_val);
						sendComPortPuts(comnum - 1, "?3538F2\r");
					}
					else if(ACKNum == ACKSEQNUM)
					{
	//					if(comportmode == COMPORTSENDSEQ)// get seq num send speed factor
	//					{
	//						sendComPortPuts(comnum - 1, getFileStrLine(comseqtmpfilename, 4));
	//						sendComPortByte(comnum - 1, 13);
	//					}
	//					else
						ACKNum = 0;
					}
					if(detectcnt)
					{
//						printf("found seq num\n");
						detectedmatch++;
	//						closeComPort(comnumdetect);
//						detectcnt = 1;
					}

				}
				if(getIntHexCmpChar((char*)buf,"*3538",i, 1))
				{
					if(comportmode == COMPORTMONITOR)
					{
						Ihandle* speedfactortxt = IupGetDialogChild(ih, "SPEEDFACTORTEXT");
						Ihandle* speedfactortxtshow = IupGetDialogChild(ih, "SPEEDFACTORTEXTSHOW");
						int speedfactortxt_val = getIntHexCmpChar((char*)buf,"*3538",i, 1);

						if(speedfactortxt_val != IupGetInt(speedfactortxt, "VALUE"))
						{
							IupSetInt(speedfactortxt, "VALUE", speedfactortxt_val);
							IupSetInt(speedfactortxtshow, "VALUE", speedfactortxt_val * 100/128);
						}
					}
					else if(comportmode == COMPORTREADFLASH)
					{
						Ihandle* speedfactortxt = IupGetDialogChild(ih, "SPEEDFACTORTEXT");
						Ihandle* speedfactortxtshow = IupGetDialogChild(ih, "SPEEDFACTORTEXTSHOW");
						int speedfactortxt_val = getIntHexCmpChar((char*)buf,"*3538",i, 1);

						IupSetInt(speedfactortxt, "VALUE", speedfactortxt_val);
						IupSetInt(speedfactortxtshow, "VALUE", speedfactortxt_val * 100/128);
						sendComPortPuts(comnum - 1, "?35330001F8\r");
					}
					else if(ACKNum == ACKSPEEDFACTOR)
					{
	//					if(comportmode == COMPORTSENDSEQ)// get seq num send speed factor
	//					{
	//						sendComPortPuts(comnum - 1, getFileStrLine(comseqtmpfilename, 5));
	//						sendComPortByte(comnum - 1, 13);
	//					}
	//					else
						ACKNum = 0;
					}
					if(detectcnt)
					{
//						printf("found speed factor\n");
	//						closeComPort(comnumdetect);
						detectedmatch++;
//						detectcnt = 1;
					}

				}
				if(getIntHexCmpChar((char*)buf,"*3533", i, 2))
				{
					if(comportmode == COMPORTMONITOR)
					{
						if(n > (i + 27))
						{
							unsigned int bptrNum;
							unsigned char j, k;
							unsigned int seq_val = getIntHexChar((char*)buf,i + 5,2);
							unsigned int dwell_val = getIntHexChar((char*)buf,i + 25,2);

							if(canvas)
							{
//								monitorCnt++;
//								if(monitorCnt > 4000)
//								{
//									IupSetInt(canvas,"zoom_update", 1);
//									monitorCnt=0;
//								}

								if(!IupGetInt(canvas, "on_update"))
								{
									char *bptr = IupGetAttribute(canvas, "block_pointer");

									for(j = 0; j < 4; j++)
									{
										bptrNum = getIntHexChar((char*)buf, i + 21 - (4*j),2);
										for(k = 0; k<16;k++)
										{
											if(bptrNum & (1 << k))
													bptr[16 * j + k] = 1;
											else bptr[16 * j + k] = 0;
										}
									}
									IupSetAttribute(canvas, "block_pointer", bptr);
									if(!IupGetInt(canvas, "playmode"))IupSetInt(canvas, "playmode", 1);
									IupSetInt(IupGetDialogChild(ih, "SEQLABELCUR"), "TITLE", seq_val);
									IupSetInt(IupGetDialogChild(ih, "DWELLTEXT"), "VALUE", dwell_val);

									Ihandle* delaylbl = IupGetDialogChild(ih, "DELAYLABEL");
									Ihandle* speedfactxt = IupGetDialogChild(ih, "SPEEDFACTORTEXT");

									double speedfactor_val = IupGetDouble(speedfactxt,"VALUE");
									double delay_val = speedfactor_val * dwell_val / 128;
									IupSetStrf(delaylbl, "TITLE", "%.2f ms", delay_val);

									if(!IupGetInt(canvas, "on_update"))IupUpdate(canvas);
								}
//								else printf("canvas on update ignored! \n");
							}
						}
					}
					else if(comportmode == COMPORTREADFLASH)
					{
						if(n > (i + 27))
						{
							unsigned int bptrNum;
							unsigned char j, k;
							unsigned int seq_val = getIntHexChar((char*)buf,i + 5,2);
							static unsigned int seq_val_old=0;
							unsigned int dwell_val = getIntHexChar((char*)buf,i + 25,2);
							unsigned char strTmp[4] = {0x35,0x33,0,0};
							unsigned char strTmpSend[12];

							if(seq_val==1) seq_val_old=0;

							if(canvas)
							{
								if(seq_val == (seq_val_old + 1))
								{
									char *bptr = IupGetAttribute(canvas, "block_pointer");

									for(j = 0; j < 4; j++)
									{
										bptrNum = getIntHexChar((char*)buf, i + 21 - (4*j),2);
										for(k = 0; k<16;k++)
										{
											if(bptrNum & (1 << k))
													bptr[16 * j + k] = 1;
											else bptr[16 * j + k] = 0;
										}
									}
									IupSetAttribute(canvas, "block_pointer", bptr);
									if(!IupGetInt(canvas, "on_update")) IupUpdate(canvas);
									save_seq_step_file(seq_val, bptr, dwell_val);

									IupSetInt(IupGetDialogChild(ih, "SEQLABEL"), "TITLE", seq_val);
									IupSetInt(IupGetDialogChild(ih, "SEQLABELCUR"), "TITLE", seq_val);
									IupSetInt(IupGetDialogChild(ih, "DWELLTEXT"), "VALUE", dwell_val);

									Ihandle* delaylbl = IupGetDialogChild(ih, "DELAYLABEL");
									Ihandle* speedfactxt = IupGetDialogChild(ih, "SPEEDFACTORTEXT");
									Ihandle* seqobj = IupGetDialogChild(ih, "SEQVAL");

									double speedfactor_val = IupGetDouble(speedfactxt,"VALUE");
									double delay_val = speedfactor_val * dwell_val / 128;
									IupSetStrf(delaylbl, "TITLE", "%.2f ms", delay_val);

									load_seq_step(ih, seq_val);
									IupSetInt(seqobj, "MAX", seq_val - 1);
									IupSetInt(seqobj, "SHOWTICKS", seq_val);

									strTmp[2] = ((seq_val +1) >> 8) & 0xff;
									strTmp[3] = (seq_val +1) & 0xff;
									sprintf((char*)strTmpSend, "?%02X%02X%02X%02X%02X\r", strTmp[0], strTmp[1], strTmp[2], strTmp[3], getCheckSumCharArr(strTmp, 0, 4));
									sendComPortPuts(comnum - 1, (char*)strTmpSend);
									seq_val_old=seq_val;
								}
								else
								{
									strTmp[2] = (seq_val >> 8) & 0xff;
									strTmp[3] = (seq_val & 0xff);
									sprintf((char*)strTmpSend, "?%02X%02X%02X%02X%02X\r", strTmp[0], strTmp[1], strTmp[2], strTmp[3], getCheckSumCharArr(strTmp, 0, 4));
									sendComPortPuts(comnum - 1, (char*)strTmpSend);
								}
							}
						}
					}

					if(detectcnt)
					{
//						printf("found scene map\n");
//						detectedmatch = 0;
	//					closeComPort(comnumdetect);
//						detectcnt = 1;
					}
				}
			}
		  }

		if(detectedmatch > 3)
		{
//			printf("Found\n");
			IupSetStrf(comportlist, "1","%d", comnum);
			IupSetStrf(comportlist, "VALUESTRING","%d", comnum);
			IupSetAttribute(comportbutton, "ACTIVE", "YES");
			IupSetAttribute(matmculabel, "TITLE", "Found");
			detectedmatch=0;
		}
//		  puts((char *)buf);
	  }
	}
}
void setDwellVal(Ihandle *parent_dlg, unsigned int dwell_input_val)
{
	Ihandle* dwelltxt = IupGetDialogChild(parent_dlg, "DWELLTEXT");
	Ihandle* delaylbl = IupGetDialogChild(parent_dlg, "DELAYLABEL");
	Ihandle* speedfactxt = IupGetDialogChild(parent_dlg, "SPEEDFACTORTEXT");
	Ihandle* canvas = IupGetDialogChild(parent_dlg, "CANVAS");
	double speedfactor_val = IupGetDouble(speedfactxt,"VALUE");
	double dwelltxt_val = (double)dwell_input_val;
	double delay_val = speedfactor_val * dwelltxt_val / 128;

	if(delay_val < 10)
	{
		delay_val = 10;
		dwelltxt_val = delay_val * 128 / speedfactor_val;
	}
	if (dwelltxt_val > 0xffff)
	{
		dwelltxt_val = 0xffff;
		delay_val = speedfactor_val *  dwelltxt_val / 128;
	}
	IupSetStrf(delaylbl, "TITLE", "%.2f ms", delay_val);
	IupSetInt(dwelltxt, "VALUE", dwelltxt_val);
	if(IupGetInt(canvas, "properties_show")) IupUpdate(canvas);
}

void setSpeedFactorVal(Ihandle *parent_dlg, unsigned int speedfactortxtshow_val)
{
	Ihandle* dwelltxt = IupGetDialogChild(parent_dlg, "DWELLTEXT");
	Ihandle* delaylbl = IupGetDialogChild(parent_dlg, "DELAYLABEL");
	Ihandle* speedfactxt = IupGetDialogChild(parent_dlg, "SPEEDFACTORTEXT");
	Ihandle* speedfactxtshow = IupGetDialogChild(parent_dlg, "SPEEDFACTORTEXTSHOW");
	Ihandle* canvas = IupGetDialogChild(parent_dlg, "CANVAS");

	double speedfactortxtshow_val_out = speedfactortxtshow_val;
	double speedfactor_val = speedfactortxtshow_val_out*128/100;
	double dwelltxt_val = IupGetDouble(dwelltxt, "VALUE");
	double delay_val = speedfactor_val * dwelltxt_val / 128;

	if(delay_val < 10)
	{
		delay_val = 10;
		speedfactor_val = delay_val * 128 / dwelltxt_val;
		speedfactortxtshow_val_out = speedfactor_val * 100 / 128;
	}
	if (speedfactor_val > 0xff)
	{
		speedfactor_val = 0xff;
		delay_val =  dwelltxt_val * speedfactor_val / 128;
		speedfactortxtshow_val_out = speedfactor_val * 100 / 128;
	}
	IupSetStrf(delaylbl, "TITLE", "%.2f ms", delay_val);
	IupSetInt(speedfactxt, "VALUE", speedfactor_val);
	IupSetInt(speedfactxtshow, "VALUE", speedfactortxtshow_val_out);
	if(IupGetInt(canvas, "properties_show")) IupUpdate(canvas);
}

void disableSeqControl(Ihandle *ih)
{
	IupSetAttribute(IupGetDialogChild(ih, "DWELLTEXT"),"ACTIVE", "NO");
	IupSetAttribute(IupGetDialogChild(ih, "DWELLSPIN"),"ACTIVE", "NO");
	IupSetAttribute(IupGetDialogChild(ih, "SCENERMBTN"),"ACTIVE", "NO");
	IupSetAttribute(IupGetDialogChild(ih, "SCENEADDBTN"),"ACTIVE", "NO");
	IupSetAttribute(IupGetDialogChild(ih, "SEQTEXT"),"ACTIVE", "NO");
	IupSetAttribute(IupGetDialogChild(ih, "LAYSPIN"),"ACTIVE", "NO");
//	IupSetAttribute(IupGetDialogChild(ih, "CANVASLAYOUT"),"ACTIVE", "NO");
	IupSetInt(IupGetDialogChild(ih, "CANVAS"), "playmode", 1);
}

void enableSeqControl(Ihandle *ih)
{
#ifdef FULLVERSION
	IupSetInt(IupGetDialogChild(ih, "PLAYTOGGLE"), "VALUE", 0);
	IupSetAttribute(IupGetDialogChild(ih, "PLAYTOGGLE"), "IMAGE","IUP_Play");
	IupSetAttribute(IupGetDialogChild(ih, "DWELLTEXT"),"ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(ih, "DWELLSPIN"),"ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(ih, "SCENERMBTN"),"ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(ih, "SCENEADDBTN"),"ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(ih, "SEQTEXT"),"ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(ih, "LAYSPIN"),"ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(ih, "SPEEDFACTORSPIN"),"ACTIVE", "YES");
//	IupSetAttribute(IupGetDialogChild(ih, "CANVASLAYOUT"),"ACTIVE", "YES");
#endif
	IupSetInt(IupGetDialogChild(ih, "CANVAS"), "playmode", 0);
}
void disableComportControl(Ihandle *ih)
{
	IupSetAttribute(IupGetDialogChild(ih, "MACHNOTEXT"), "ACTIVE", "NO");

	IupSetAttribute(IupGetDialogChild(ih, "COMPORTTXTLIST"), "ACTIVE", "NO");
	IupSetAttribute(IupGetDialogChild(ih, "COMPORTSENDBUTTON"), "ACTIVE", "NO");
	IupSetAttribute(IupGetDialogChild(ih, "COMPORTSENDSEQBUTTON"), "ACTIVE", "NO");
	IupSetAttribute(IupGetDialogChild(ih, "COMPORTREADFLASHBUTTON"), "ACTIVE", "NO");
	IupSetAttribute(IupGetDialogChild(ih, "COMPORTMONITORBUTTON"), "ACTIVE", "NO");
}
void disableComportSeqControl(Ihandle *ih)
{
	disableComportControl(ih);
	IupSetAttribute(IupGetDialogChild(ih, "PLAYCONTROLFRAME"), "ACTIVE", "NO");
	IupSetAttribute(IupGetDialogChild(ih, "COMPORTBUTTON"), "ACTIVE", "NO");
	IupSetAttribute(IupGetDialogChild(ih, "SPEEDFACTORTEXTSHOW"), "ACTIVE", "NO");
	IupSetAttribute(IupGetDialogChild(ih, "SPEEDFACTORSPIN"),"ACTIVE", "NO");
}

void enableComportControl(Ihandle *ih)
{
#ifdef FULLVERSION
	IupSetAttribute(IupGetDialogChild(ih, "MACHNOTEXT"), "ACTIVE", "YES");

	IupSetAttribute(IupGetDialogChild(ih, "COMPORTTXTLIST"), "ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(ih, "COMPORTSENDBUTTON"), "ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(ih, "COMPORTSENDSEQBUTTON"), "ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(ih, "COMPORTREADFLASHBUTTON"), "ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(ih, "COMPORTMONITORBUTTON"), "ACTIVE", "YES");
#endif
}
void enableComportSeqControl(Ihandle *ih)
{
#ifdef FULLVERSION
	enableComportControl(ih);
	IupSetAttribute(IupGetDialogChild(ih, "SPEEDFACTORTEXTSHOW"), "ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(ih, "SPEEDFACTORSPIN"), "ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(ih, "PLAYCONTROLFRAME"), "ACTIVE", "YES");
#endif
	IupSetAttribute(IupGetDialogChild(ih, "COMPORTBUTTON"), "ACTIVE", "YES");
}

void clean_memloc(Ihandle *ih)
{
  Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
  Ihandle* seqlbl = IupGetDialogChild(ih, "SEQLABEL");
  IupSetAttribute(IupGetDialogChild(ih, "SEQTIMER"), "RUN", "NO");
  IupSetAttribute(IupGetDialogChild(ih, "COMPORTTIMER"), "RUN", "NO");
  IupSetAttribute(IupGetDialogChild(ih, "COMPORTTIMERSEARCH"), "RUN", "NO");

  char *bptr = IupGetAttribute(canvas, "block_pointer");
  char *bptr_old = IupGetAttribute(canvas, "block_pointer_old");
  char tmpfiles[15];

	//remove all seq files
	//maximum  number is token from max label. +1 if there is jump from maximum number.
	remove_seq_step_files(IupGetInt(seqlbl, "TITLE") + 1);

	sprintf(tmpfiles, "del /F /Q %s", tmpfolder);
	system(tmpfiles);

	free(bptr);free(bptr_old);
	bptr=NULL;
	bptr_old=NULL;
    rmdir(tmpfolder);
}

void load_seq_step(Ihandle *ih, unsigned int seq_val)
{
	Ihandle* seqobj = IupGetDialogChild(ih, "SEQVAL");
	Ihandle* seqlblcur = IupGetDialogChild(ih, "SEQLABELCUR");
	Ihandle* seqlblmax = IupGetDialogChild(ih, "SEQLABEL");
	Ihandle* seqtxt = IupGetDialogChild(ih, "SEQTEXT");
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
	Ihandle* dwelltxt = IupGetDialogChild(ih, "DWELLTEXT");
	Ihandle* delaylbl = IupGetDialogChild(ih, "DELAYLABEL");
	Ihandle* speedfactor =  IupGetDialogChild(ih, "SPEEDFACTORTEXT");
	char *bptr = IupGetAttribute(canvas, "block_pointer");
	unsigned int delay_val;
	unsigned int seq_val_max = IupGetInt(seqlblmax, "TITLE");

	if(seq_val <= MAXSCENENUM)
	{
		load_seq_step_file(seq_val, &(*bptr), &delay_val);
		IupSetAttribute(canvas,"block_pointer", bptr);
		if(delay_val)
		{
			IupSetInt(dwelltxt, "VALUE", delay_val);
			IupSetStrf(delaylbl, "TITLE", "%.2f ms", IupGetDouble(speedfactor,"VALUE") * (double)delay_val / 128);
		}

		//update UI
		IupSetDouble(seqobj, "VALUE", seq_val - 1);  /* fixed increments when using buttons */
		IupSetInt(seqtxt,"VALUE", seq_val);
		IupSetInt(seqlblcur,"TITLE", seq_val);

		//if seq_val reach maximum value
		if(seq_val == (seq_val_max + 1))
		{
			IupSetInt(seqlblmax,"TITLE", seq_val);
			IupSetInt(seqobj, "MAX", seq_val - 1);
			IupSetInt(seqobj, "SHOWTICKS", seq_val);
		}

		IupUpdate(canvas);
	}
}

void save_seq_step(Ihandle *ih, unsigned int seq_val)
{
	if(seq_val <= MAXSCENENUM)
	{
		Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
		Ihandle* dwelltxt = IupGetDialogChild(ih, "DWELLTEXT");
		unsigned int delay_val = IupGetInt(dwelltxt,"VALUE");
		char *bptr = IupGetAttribute(canvas, "block_pointer");
		save_seq_step_file(seq_val, bptr, delay_val);
	}
}

unsigned char getBlockNum(int imflag, unsigned char unum, int bcntw, int bcnth, int xn, int yn)
{
	unsigned char bnumout = 0;
	int btotal = bcntw*bcnth;
	switch(imflag)
	{
		case normal:
			if(bcntw < 4)
				bnumout = (unsigned char)((bcntw + 4 - bcntw)*(bcnth-1-yn)+xn) + 16 * (unum - 1);
			else
				bnumout = (unsigned char)(bcntw*(bcnth-1-yn)+xn) + btotal * (unum - 1);
			break;
		case mirror:
			if(bcntw < 4)
				bnumout = (unsigned char)((bcntw + 4 - bcntw)*(bcnth-1-yn)+(bcntw-1-xn)) + 16 * (unum - 1);
			else
				bnumout = (unsigned char)(bcntw*(bcnth-1-yn)+(bcntw-1-xn)) + btotal * (unum - 1);
			break;
		case rotate90cw:
			if(bcnth < 4)
				bnumout = (unsigned char)((bcnth + 4 - bcnth) *(bcntw-1-xn)+(bcnth-1-yn)) + 16 * (unum - 1);
			else
				bnumout = (unsigned char)(bcnth*(bcntw-1-xn)+(bcnth-1-yn)) + btotal * (unum - 1);
			break;
		case rotate90ccw:
			if(bcnth < 4)
				bnumout = (unsigned char)((bcnth + 4 - bcnth) * xn+yn) + 16 * (unum - 1);
			else
				bnumout = (unsigned char)(bcnth*xn+yn) + btotal * (unum - 1);
			break;
		case rotate180:
			if(bcntw < 4)
				bnumout = (unsigned char)((bcntw + 4 - bcntw)*yn+(bcntw-1-xn)) + 16 * (unum - 1);
			else
				bnumout = (unsigned char)(bcntw*yn+(bcntw-1-xn)) + btotal * (unum - 1);
			break;
		case flip:
			if(bcntw < 4)
				bnumout = (unsigned char)((bcntw + 4 - bcntw)*yn+xn) + 16 * (unum - 1);
			else
				bnumout = (unsigned char)(bcntw*yn+xn) + btotal * (unum - 1);
			break;
		default:
			return NULL;
			break;
	}
	return bnumout;
}

int getUnitimFlag(Ihandle *canvas, unsigned char unum)
{
	char strTmp[5];
	int imflagout;
	sprintf(strTmp, "imf%d", unum);
	imflagout = IupGetInt(canvas,strTmp);

	return imflagout;
}

void setUnitParam(Ihandle* canvas, unsigned char unum, int *ux, int *uy, int *uw, int *uh, int *bw, int *bh)
{
	char strTmp[5];
	int loffx, loffy;

	IupGetIntInt(canvas, "loff", &loffx, &loffy);

	sprintf(strTmp, "uxy%d", (int)unum);
	IupGetIntInt(canvas, strTmp, &(*ux), &(*uy));
	*ux+=loffx;
	*uy+=loffy;

	sprintf(strTmp, "uwh%d", (int)unum);
	IupGetIntInt(canvas, strTmp, &(*uw), &(*uh));

	sprintf(strTmp, "bwh%d", (int)unum);
	IupGetIntInt(canvas, strTmp, &(*bw), &(*bh));
}

void setBlockImage(Ihandle *canvas, unsigned char unum, imImage **imgb)
{
	char strTmp[5];
	sprintf(strTmp, "ims%d", (int)unum);
	*imgb = (imImage*)IupGetAttribute(canvas, strTmp);
}

void setUnitImage(Ihandle *canvas, unsigned char unum, imImage **imgb)
{
	char strTmp[5];
	sprintf(strTmp, "img%d", (int)unum);
	*imgb = (imImage*)IupGetAttribute(canvas, strTmp);
}

void drawUnitImage(cdCanvas* cd_canvas, imImage *uimg, unsigned char unum, int ux, int uy, int uw, int uh, int bw, int bh)
{
	imcdCanvasPutImage(cd_canvas, uimg, ux, uy, uw, uh, 0, 0, 0, 0);
	imcdCanvasPutImage(cd_canvas, uimg, ux, uy, uw, uh, 0, 0, 0, 0);
	cdCanvasForeground(cd_canvas, CD_BLACK);
	cdCanvasRect(cd_canvas, ux, ux + uw, uy, uy + uh);

	cdCanvasRect(cd_canvas, ux + bw, ux + (bw * 2), uy, uy + uh);
	cdCanvasRect(cd_canvas, ux + (bw * 2), ux + (bw * 3), uy, uy + uh);

	cdCanvasRect(cd_canvas, ux, ux + uw, uy + bh, uy + (bh * 2));
	cdCanvasRect(cd_canvas, ux, ux + uw, uy + (bh * 2), uy + (bh * 3));

	//unit grid
	cdCanvasForeground(cd_canvas, CD_BLACK);
	cdCanvasRect(cd_canvas, ux, ux + uw, uy, uy + uh);

	cdCanvasRect(cd_canvas, ux + bw, ux + (bw * 2), uy, uy + uh);
	cdCanvasRect(cd_canvas, ux + (bw * 2), ux + (bw * 3), uy, uy + uh);

	cdCanvasRect(cd_canvas, ux, ux + uw, uy + bh, uy + (bh * 2));
	cdCanvasRect(cd_canvas, ux, ux + uw, uy + (bh * 2), uy + (bh * 3));
}

void expandBlock(cdCanvas* cd_canvas, imImage *imgb, imImage *imgs,
		int bcntw, int bcnth, int xn, int yn, int bw, int bh,int ux, int uy)
{
	int imgcw = imgb->width/bcntw;
	int imgch = imgb->height/bcnth;
	imImage* imgcrop = imImageCreate(imgcw,imgch,IM_RGB,IM_BYTE);
	int xb = ux + xn*bw;
	int yb = uy + yn*bh;
	imProcessCrop(imgb, imgcrop,xn*imgcw,yn*imgch);
	imcdCanvasPutImage(cd_canvas, imgcrop, xb, yb, bw, bh, 0, 0, 0, 0);
	cdCanvasForeground(cd_canvas, CD_BLACK);
	cdCanvasRect(cd_canvas, xb, xb + bw, yb, yb + bh);
//	if(imgcrop) imImageDestroy(imgcrop);
}

void normalBlock(cdCanvas* cd_canvas, imImage *imgb,
		int xn, int yn, int bw, int bh,int ux, int uy)
{
	int xb = ux + xn*bw;
	int yb = uy + yn*bh;
	imcdCanvasPutImage(cd_canvas, imgb, xb, yb, bw, bh, 0, 0, 0, 0);
	cdCanvasForeground(cd_canvas, CD_BLACK);
	cdCanvasRect(cd_canvas, xb, xb + bw, yb, yb + bh);
}

void clearUnit(Ihandle *canvas, unsigned char unum)
{
	imImage *imgb;
	int i,j;
	int uw, uh, ux, uy, bw, bh;
	cdCanvas* cd_canvas = (cdCanvas*)IupGetAttribute(canvas, "cdCanvas");
	char *bptr = IupGetAttribute(canvas, "block_pointer");
	int imflag = getUnitimFlag(canvas, unum);
	int bcntw, bcnth;
	unsigned char bnum;

	cdCanvasActivate(cd_canvas);

	IupGetIntInt(canvas, "bcwh", &bcntw, &bcnth);
	setUnitImage(canvas, unum, &imgb);
	setUnitParam(canvas, unum, &ux, &uy, &uw, &uh, &bw, &bh);

	if(imgb)
	{
		drawUnitImage(cd_canvas, imgb, unum, ux, uy, uw, uh, bw, bh);
		for(i = 0; i<4;i++)
			for(j = 0; j < 4; j++)
			{
				bnum = getBlockNum(imflag, unum, bcntw, bcnth, i,j);
				if(!bptr[bnum])
					bptr[bnum] = 1;
			}
		IupSetAttribute(canvas,"block_pointer", bptr);
	}
	drawPropertiesShow(canvas);
	cdCanvasFlush(cd_canvas);
}

void drawUnitGridImage(cdCanvas* cd_canvas, imImage *imgu, imImage *imgb, unsigned char unum, int imflag,
		int ucnt, int bcntw, int bcnth,
		int ux, int uy, int uw, int uh, int bw, int bh, char *bptr)
{
	int i,j;
//	drawUnitImage(cd_canvas, imgu, unum, ux, uy, uw, uh, bw, bh);
	unsigned char bnum;

	for(i = 0; i<bcntw;i++)
		for(j = 0; j < bcnth; j++)
		{
			bnum = getBlockNum(imflag, unum, bcntw, bcnth, i,j);
			if(!bptr[bnum]) // block pointer calc: total_block (in 1 unit) *(unum - 1) + (4y) + x
				normalBlock(cd_canvas, imgb,i, j, bw, bh,ux, uy);
			else
				expandBlock(cd_canvas, imgu, imgb, bcntw, bcnth, i, j, bw, bh,ux, uy);
		}
}

void drawUnitBlock(Ihandle *canvas, unsigned char unum, int xn, int yn, unsigned char bnum, unsigned char is_clear)
{
	imImage *imgb, *imgs;
	int bw, bh, ux, uy, uw, uh;
	int bcntw, bcnth;

	cdCanvas* cd_canvas = (cdCanvas*)IupGetAttribute(canvas, "cdCanvas");
	char *bptr = IupGetAttribute(canvas, "block_pointer");

	cdCanvasActivate(cd_canvas);

	setBlockImage(canvas, unum, &imgs);
	setUnitImage(canvas, unum, &imgb);
	setUnitParam(canvas, unum, &ux, &uy, &uw, &uh, &bw, &bh);
	IupGetIntInt(canvas,"bcwh", &bcntw, &bcnth);

	if(imgb)
	{
		if(is_clear)
		{
			if(!bptr[bnum])
			{
				expandBlock(cd_canvas, imgb, imgs, bcntw, bcnth, xn, yn, bw, bh, ux, uy);
				bptr[bnum] = 1;
			}
			else if(is_clear == 2)
			{
				normalBlock(cd_canvas, imgs,xn, yn, bw, bh, ux, uy);
				bptr[bnum] = 0;
			}
		}
		else
		{
			normalBlock(cd_canvas, imgs,xn, yn, bw, bh, ux, uy);
			bptr[bnum] = 0;
		}
		IupSetAttribute(canvas, "block_pointer", bptr);
	}
	drawPropertiesShow(canvas);
	cdCanvasFlush(cd_canvas);
}

void getBlockNumCanvas(Ihandle *canvas, unsigned char unum, int xn, int yn, unsigned char *bnum)
{
	// this function is used to translate xn yn matrix unit (yn = 0 is bottom) (xn = 0 is left)
	// into block number used for block pointer based on layout number.
	// the block number can be used as a data array for scene map that can be sent to mcu via serial comm.
	// unum is unit number, started from 1 to ucnt attribute (in canvas).

	char strTmp[6];
	sprintf(strTmp, "imf%d",unum);
	int imflag = IupGetInt(canvas, strTmp);
	int bcntw, bcnth;
	IupGetIntInt(canvas, "bcwh", &bcntw, &bcnth);

	*bnum = getBlockNum(imflag, unum, bcntw, bcnth, xn,yn);
}
void getUnitPixel(Ihandle *canvas, int x, int y, unsigned char *unum, int *xn, int *yn)
{
	int ux, uy, uw, uh, xu, yu;
	int canvas_width, canvas_height;
	int bw, bh;
	unsigned char i;
	int offbx, offby;
	int ucnt, bcntw, bcnth;

	IupGetIntInt(canvas, "DRAWSIZE", &canvas_width, &canvas_height);
	ucnt = IupGetInt(canvas,"ucnt");
	IupGetIntInt(canvas, "bcwh", &bcntw, &bcnth);

	yu = canvas_height - y;
	xu = x;

	*unum = 0;
	for(i=0;i<ucnt;i++)
	{
		setUnitParam(canvas, i+1, &ux, &uy, &uw, &uh, &bw, &bh);
		if((xu > ux) && (xu < (ux + uw)))
			if((yu > uy) && (yu < (uy + uh)))
			{
				*unum = i + 1;
				offbx = ux + uw;
				offby = uy + uh;
				*xn = bcntw;
				*yn = bcnth;
				while(xu < offbx) {offbx-=bw; *xn=*xn - 1;}
				while(yu < offby) {offby-=bh; *yn=*yn - 1;}
				break;
			}
	}
}

void setUnitBlockSize(Ihandle *canvas, int unit_width, int unit_height, int block_width, int block_height)
{
	char strTmp[5];
	unsigned char i;
	unsigned char ucnt;
	unsigned char imflag;

	ucnt = IupGetInt(canvas,"ucnt");
	for(i=0;i<ucnt;i++)
	{
		sprintf(strTmp,"imf%d",i+1);
		imflag = IupGetInt(canvas,strTmp);

		if((imflag == rotate90cw) || (imflag == rotate90ccw))
		{
			sprintf(strTmp,"uwh%d",i+1);
			IupSetStrf(canvas, strTmp, "%dx%d", unit_height, unit_width);
			sprintf(strTmp,"bwh%d",i+1);
			IupSetStrf(canvas, strTmp, "%dx%d", block_height, block_width);
		}
		else
		{
			sprintf(strTmp,"uwh%d",i+1);
			IupSetStrf(canvas, strTmp, "%dx%d", unit_width, unit_height);
			sprintf(strTmp,"bwh%d",i+1);
			IupSetStrf(canvas, strTmp, "%dx%d", block_width, block_height);
		}
	}
}

void setUnitBlockWH(Ihandle *canvas, int layout_width, int layout_height, int* unit_width, int* unit_height, int* block_width, int* block_height)
{
	Ihandle* zoom_val = IupGetDialogChild(canvas, "ZOOMVAL");
	double scale_factor, layout_ar, canvas_ar;
	int canvas_width, canvas_height;
	int bcntw, bcnth;

	double zoom_index = IupGetDouble(zoom_val, "VALUE");
	double zoom_factor = pow(2, zoom_index);

	IupGetIntInt(canvas, "DRAWSIZE", &canvas_width, &canvas_height);
	IupGetIntInt(canvas,"bcwh",&bcntw, &bcnth);

	canvas_ar = (double)canvas_width / canvas_height;

	//check aspect ratio
	layout_ar = (double)layout_width / layout_height;

	if(canvas_ar > layout_ar)
		scale_factor = zoom_factor * (double)(canvas_height) / (double)layout_height;
	else
		scale_factor = zoom_factor * (double)(canvas_width) / (double)layout_width;

	//set block width and height size
	*block_width = floor((scale_factor * IMGBUFWIDTH)/bcntw);
	*block_height =floor((scale_factor * IMGBUFHEIGHT) /bcnth);

	//set unit width and height size
	*unit_width = *block_width * bcntw;
	*unit_height = *block_height *bcnth;
}

void setUnitLayoutSize(Ihandle *canvas)
{
	double lh1_calc, lh2_calc;
	int unit_width, unit_height;
	int block_width, block_height;
	int layout_width, layout_height, scaled_layout_width = 0, scaled_layout_height = 0;
	int laynum = IupGetInt(canvas, "layout_num");
	imImage *img = (imImage*)IupGetAttribute(canvas, "IMAGE");

	if(img)
	{
		switch (laynum)
		{
			case 1:
				//check aspect ratio
				layout_width = (IMGBUFWIDTH * 2) + IMGBUFHEIGHT;
				if(IMGBUFWIDTH > (IMGBUFHEIGHT * 2)) layout_height = (IMGBUFWIDTH * 2) + IMGBUFHEIGHT;
				else layout_height = (IMGBUFHEIGHT * 2);

				setUnitBlockWH(canvas, layout_width, layout_height, &unit_width, &unit_height, &block_width, &block_height);

				//scaled layout width and height
				scaled_layout_width = (unit_width * 2) + unit_height;
				if(unit_width > (unit_height * 2)) scaled_layout_height = unit_width;
				else scaled_layout_height = (unit_height * 2);

				//unit 1
				IupSetStrf(canvas, "uxy1", "%dx%d", 0, 0);
				//unit 2
				IupSetStrf(canvas, "uxy2", "%dx%d", unit_width, unit_height);
				//unit 3 rotate 90
				IupSetStrf(canvas, "uxy3", "%dx%d", (unit_width * 2), 0);
				//unit 4
				IupSetStrf(canvas, "uxy4", "%dx%d", unit_width, 0);
				break;

			case 2:
				//check aspect ratio
				layout_width = (IMGBUFWIDTH * 2) + (IMGBUFHEIGHT * 2);
				layout_height = round((double)IMGBUFHEIGHT / 4) + IMGBUFWIDTH;

				setUnitBlockWH(canvas, layout_width, layout_height, &unit_width, &unit_height, &block_width, &block_height);

				//scaled layout width and height
				scaled_layout_width = (unit_width * 2) + (unit_height * 2);
				scaled_layout_height = unit_height + block_height;

				//unit 1 => rotate 90 clockwise
				IupSetStrf(canvas, "uxy1", "%dx%d", 0, 0);
				//unit 2 normal
				IupSetStrf(canvas, "uxy2", "%dx%d", unit_height, unit_width + block_height - unit_height);
				//unit 3 rotate 180
				IupSetStrf(canvas, "uxy3", "%dx%d", unit_height + unit_width, unit_width + block_height - unit_height);
				//unit 4 rotate 90 ccw
				IupSetStrf(canvas, "uxy4", "%dx%d", unit_height + (unit_width * 2), 0);
				break;
			case 3:
				//calculate width and height
				layout_width = IMGBUFWIDTH+ (IMGBUFHEIGHT * 2);

				lh1_calc = ((double)IMGBUFHEIGHT * 2) + ((double) IMGBUFWIDTH / 7);
				lh2_calc = ((double)IMGBUFHEIGHT * 7.0 /8.0) + (double)IMGBUFWIDTH;

				if(lh1_calc > lh2_calc) layout_height = round(lh1_calc);
				else layout_height = round(lh2_calc);

				setUnitBlockWH(canvas, layout_width, layout_height, &unit_width, &unit_height, &block_width, &block_height);

				//scaled layout width and height
				scaled_layout_width = unit_width + (unit_height * 2);

				lh1_calc = ((double)unit_height * 2) + ((double) unit_width / 7);
				lh2_calc = ((double)unit_height * 7.0 /8.0) + (double)unit_width;
				if(lh1_calc > lh2_calc) scaled_layout_height = round(lh1_calc);
				else scaled_layout_height = round(lh2_calc);

				//unit 1 => rotate 90 clockwise
				IupSetStrf(canvas, "uxy1", "%dx%d", 0, unit_height * 7 / 8);
				//unit 2 rotate180
				IupSetStrf(canvas, "uxy2", "%dx%d", unit_height, unit_height);
				//unit 3 rotate 90 ccw
				IupSetStrf(canvas, "uxy3", "%dx%d", unit_width + unit_height, unit_height * 7 / 8);
				//unit 4 normal
				IupSetStrf(canvas, "uxy4", "%dx%d", unit_height, 0);
				break;
			case 4:
				//calculate width and height
				layout_width = IMGBUFWIDTH+ IMGBUFHEIGHT;
				layout_height = IMGBUFWIDTH+ IMGBUFHEIGHT;

				setUnitBlockWH(canvas, layout_width, layout_height, &unit_width, &unit_height, &block_width, &block_height);

				//scaled layout width and height
				scaled_layout_width = unit_width + unit_height;
				scaled_layout_height = unit_width + unit_height;

				//unit 1 mirror
				IupSetStrf(canvas, "uxy1", "%dx%d", 0, 0);
				//unit 2 90 cw
				IupSetStrf(canvas, "uxy2", "%dx%d", 0, unit_height);
				//unit 3 normal
				IupSetStrf(canvas, "uxy3", "%dx%d", unit_height, unit_width);
				//unit 4 90ccw
				IupSetStrf(canvas, "uxy4", "%dx%d", unit_width, 0);
				break;
			case 5:
				//calculate width and height
				layout_width = (IMGBUFWIDTH * 5 + IMGBUFHEIGHT * 4) / 4;

				lh1_calc = (double)IMGBUFHEIGHT * 2;
				lh2_calc = (double)IMGBUFHEIGHT + (double)IMGBUFWIDTH;

				if(lh1_calc > lh2_calc) layout_height = round(lh1_calc);
				else layout_height = round(lh2_calc);

				setUnitBlockWH(canvas, layout_width, layout_height, &unit_width, &unit_height, &block_width, &block_height);

				//scaled layout width and height
				scaled_layout_width = (unit_width * 5 + unit_height * 4) / 4;

				lh1_calc = (double)unit_height * 2;
				lh2_calc = (double)unit_height + (double)unit_width;
				if(lh1_calc > lh2_calc) scaled_layout_height = round(lh1_calc);
				else scaled_layout_height = round(lh2_calc);

				//unit 1 normal
				IupSetStrf(canvas, "uxy1", "%dx%d", 0, 0);
				//unit 2 rotate180
				IupSetStrf(canvas, "uxy2", "%dx%d", 0, unit_height);
				//unit 3 normal
				IupSetStrf(canvas, "uxy3", "%dx%d", unit_width, unit_width);
				//unit 4 90 ccw
				IupSetStrf(canvas, "uxy4", "%dx%d", unit_width, 0);
				break;
			case 6:
				//calculate width and height
				layout_width = IMGBUFWIDTH * 2 + IMGBUFHEIGHT;

				lh1_calc = (double)IMGBUFHEIGHT * 2;
				lh2_calc = ((double)IMGBUFHEIGHT * 7.0 / 8.0) + (double)IMGBUFWIDTH;

				if(lh1_calc > lh2_calc) layout_height = round(lh1_calc);
				else layout_height = round(lh2_calc);

				setUnitBlockWH(canvas, layout_width, layout_height, &unit_width, &unit_height, &block_width, &block_height);

				//scaled layout width and height
				scaled_layout_width = unit_width * 2 + unit_height;

				lh1_calc = (double)unit_height* 2;
				lh2_calc = ((double)unit_height * 7.0 / 8.0) + unit_width;
				if(lh1_calc > lh2_calc) scaled_layout_height = round(lh1_calc);
				else scaled_layout_height = round(lh2_calc);

				//unit 1 mirror
				IupSetStrf(canvas, "uxy1", "%dx%d", 0, unit_height * 7 / 8);
				//unit 2 rotate180
				IupSetStrf(canvas, "uxy2", "%dx%d", unit_width, unit_height);
				//unit 3 90 ccw
				IupSetStrf(canvas, "uxy3", "%dx%d", unit_width * 2, unit_height * 7 / 8);
				//unit 4 normal
				IupSetStrf(canvas, "uxy4", "%dx%d", unit_width, 0);
				break;
			case 7:
				//calculate width and height
				layout_width = (IMGBUFWIDTH * 5 + IMGBUFHEIGHT * 4) / 4;
				layout_height = IMGBUFWIDTH + IMGBUFHEIGHT;

				setUnitBlockWH(canvas, layout_width, layout_height, &unit_width, &unit_height, &block_width, &block_height);

				//scaled layout width and height
				scaled_layout_width = (unit_width * 5 + unit_height * 4) / 4;
				scaled_layout_height = unit_width + unit_height;

				//unit 1 normal
				IupSetStrf(canvas, "uxy1", "%dx%d", 0, 0);
				//unit 2 90cw
				IupSetStrf(canvas, "uxy2", "%dx%d", unit_width - unit_height, unit_height);
				//unit 3 normal
				IupSetStrf(canvas, "uxy3", "%dx%d", unit_width, unit_width);
				//unit 4 90 ccw
				IupSetStrf(canvas, "uxy4", "%dx%d", unit_width, 0);
				break;
			case 8:
				//calculate width and height
				layout_width = IMGBUFWIDTH+ IMGBUFHEIGHT;
				layout_height = IMGBUFWIDTH+ IMGBUFHEIGHT;

				setUnitBlockWH(canvas, layout_width, layout_height, &unit_width, &unit_height, &block_width, &block_height);

				//scaled layout width and height
				scaled_layout_width = unit_width + unit_height;
				scaled_layout_height = unit_width + unit_height;

				//unit 1 mirror
				IupSetStrf(canvas, "uxy1", "%dx%d", 0, 0);
				//unit 2 90 cw
				IupSetStrf(canvas, "uxy2", "%dx%d", 0, unit_height);
				//unit 3 180
				IupSetStrf(canvas, "uxy3", "%dx%d", unit_height, unit_width);
				//unit 4 90ccw
				IupSetStrf(canvas, "uxy4", "%dx%d", unit_width, 0);
				break;
			case 9:
				//check aspect ratio
				layout_width = (IMGBUFWIDTH * 6 / 5) + (IMGBUFHEIGHT * 2);
				layout_height = IMGBUFWIDTH + IMGBUFHEIGHT;

				setUnitBlockWH(canvas, layout_width, layout_height, &unit_width, &unit_height, &block_width, &block_height);

				//scaled layout width and height
				scaled_layout_width = (unit_width * 6 / 5) + (unit_height * 2);
				scaled_layout_height = unit_width + unit_height;

				//unit 1 rotate 90 clockwise
				IupSetStrf(canvas, "uxy1", "%dx%d", 0, 0);
				//unit 2 normal
				IupSetStrf(canvas, "uxy2", "%dx%d", (unit_height*5 - unit_width * 2) / 5, unit_width); //assump 3/5w hole uw * (3 - 5)
				//unit 3 rotate 180
				IupSetStrf(canvas, "uxy3", "%dx%d", (unit_height*5 + unit_width * 3) / 5, unit_width);
				//unit 4 rotate 90 ccw
				IupSetStrf(canvas, "uxy4", "%dx%d", (unit_height * 5 + unit_width *6) /5, 0); //unit_width * (3 - 5 + 5 + 3)
				break;
			case 10:
				//check aspect ratio
				layout_width = IMGBUFWIDTH * 2;
				layout_height = IMGBUFHEIGHT * 2;

				setUnitBlockWH(canvas, layout_width, layout_height, &unit_width, &unit_height, &block_width, &block_height);

				//scaled layout width and height
				scaled_layout_width = unit_width *2;
				scaled_layout_height = unit_height * 2;

				//unit 1 rotate 90 clockwise
				IupSetStrf(canvas, "uxy1", "%dx%d", 0, 0);
				//unit 2 normal
				IupSetStrf(canvas, "uxy2", "%dx%d", unit_width / 2, unit_height); //assump 3/5w hole uw * (3 - 5)
				//unit 3 rotate 180
				IupSetStrf(canvas, "uxy3", "%dx%d", unit_width, 0);
				break;
			default: break;
		}
		IupSetStrf(canvas, "lwh", "%dx%d", scaled_layout_width, scaled_layout_height);
		setUnitBlockSize(canvas, unit_width, unit_height, block_width, block_height);
	}
}

void setUnitimImage(Ihandle *canvas, unsigned char unum, unsigned char imflag)
{
	char strTmp[5];
	imImage *imgout = NULL;

	imImage *imgori = (imImage*)IupGetAttribute(canvas, "IMAGE");
	imImage *img = imImageCreate(IMGBUFWIDTH, IMGBUFHEIGHT, IM_RGB, IM_BYTE);
	imProcessResize(imgori,img,0);

	if(img)
	{
		switch(imflag)
		{
			case normal:
				imgout = imImageCreate(img->width, img->height, IM_RGB, IM_BYTE);
				imgout = img;
				break;
			case mirror:
				imgout = imImageCreate(img->width, img->height, IM_RGB, IM_BYTE);
				imProcessMirror(img, imgout);
				break;
			case rotate90cw:
				imgout = imImageCreate(img->height, img->width, IM_RGB, IM_BYTE);
				imProcessRotate90(img, imgout, 1);
				break;
			case rotate90ccw:
				imgout = imImageCreate(img->height, img->width, IM_RGB, IM_BYTE);
				imProcessRotate90(img, imgout, -1);
				break;
			case rotate180:
				imgout = imImageCreate(img->width, img->height, IM_RGB, IM_BYTE);
				imProcessRotate180(img, imgout);
				break;
			case flip:
				imgout = imImageCreate(img->width, img->height, IM_RGB, IM_BYTE);
				imProcessFlip(img, imgout);
				break;
			default:
				break;
		}
		imImage *imsout = imImageCreate(imgout->width / 2, imgout->height / 2, IM_RGB, IM_BYTE);
		imProcessResize(imgout, imsout, 0);
		sprintf(strTmp, "ims%d",(int)unum);
		IupSetAttribute(canvas,strTmp, (char*)imsout);
		sprintf(strTmp, "imf%d",(int)unum);
		IupSetInt(canvas,strTmp, imflag);
		sprintf(strTmp, "img%d",(int)unum);
		IupSetAttribute(canvas,strTmp, (char*)imgout);
//		imImageDestroy(img);
	}
}

void setUnitLayoutImages(Ihandle *canvas)
{
//	unsigned char i;
	imImage *img = (imImage*)IupGetAttribute(canvas, "IMAGE");
	int laynum = IupGetInt(canvas, "layout_num");
//	char *blocktgl = IupGetAttribute(canvas, "block_pointer");
//	char *blocktgl_old = IupGetAttribute(canvas, "block_pointer_old");
	int ucnt = 0, bcntw = 0, bcnth = 0;

//	if(blocktgl == NULL)
//	{
//		blocktgl = (char*)malloc(64);
//		for(i = 0;i<64;i++)
//			blocktgl[i]=1;
//		IupSetAttribute(canvas, "block_pointer", blocktgl);
//	}
//
//	if(blocktgl_old == NULL)
//	{
//		blocktgl_old = (char*)malloc(64);
//		for(i = 0;i<64;i++)
//			blocktgl_old[i]=1;
//		IupSetAttribute(canvas, "block_pointer_old", blocktgl_old);
//	}

	if(img)
	{
		switch (laynum)
			{
				case 1:
					setUnitimImage(canvas, 1,mirror);
					setUnitimImage(canvas, 2,rotate180);
					setUnitimImage(canvas, 3,rotate90ccw);
					setUnitimImage(canvas, 4,normal);
					ucnt = 4; bcntw = 4; bcnth = 4;
					break;
				case 2:
					setUnitimImage(canvas, 1,rotate90cw);
					setUnitimImage(canvas, 2,normal);
					setUnitimImage(canvas, 3,rotate180);
					setUnitimImage(canvas, 4,rotate90ccw);
					ucnt = 4; bcntw = 4; bcnth = 4;
					break;
				case 3:
					setUnitimImage(canvas, 1,rotate90cw);
					setUnitimImage(canvas, 2,rotate180);
					setUnitimImage(canvas, 3,rotate90ccw);
					setUnitimImage(canvas, 4,normal);
					ucnt = 4; bcntw = 4; bcnth = 4;
					break;
				case 4:
					setUnitimImage(canvas, 1,mirror);
					setUnitimImage(canvas, 2,rotate90cw);
					setUnitimImage(canvas, 3,normal);
					setUnitimImage(canvas, 4,rotate90ccw);
					ucnt = 4; bcntw = 4; bcnth = 4;
					break;
				case 5:
					setUnitimImage(canvas, 1,normal);
					setUnitimImage(canvas, 2,rotate180);
					setUnitimImage(canvas, 3,normal);
					setUnitimImage(canvas, 4,rotate90ccw);
					ucnt = 4; bcntw = 4; bcnth = 4;
					break;
				case 6:
					setUnitimImage(canvas, 1,mirror);
					setUnitimImage(canvas, 2,rotate180);
					setUnitimImage(canvas, 3,rotate90ccw);
					setUnitimImage(canvas, 4,normal);
					ucnt = 4; bcntw = 4; bcnth = 4;
					break;
				case 7:
					setUnitimImage(canvas, 1,normal);
					setUnitimImage(canvas, 2,rotate90cw);
					setUnitimImage(canvas, 3,normal);
					setUnitimImage(canvas, 4,rotate90ccw);
					ucnt = 4; bcntw = 4; bcnth = 4;
					break;
				case 8:
					setUnitimImage(canvas, 1,mirror);
					setUnitimImage(canvas, 2,rotate90cw);
					setUnitimImage(canvas, 3,rotate180);
					setUnitimImage(canvas, 4,rotate90ccw);
					ucnt = 4; bcntw = 4; bcnth = 4;
					break;
				case 9:
					setUnitimImage(canvas, 1,rotate90cw);
					setUnitimImage(canvas, 2,normal);
					setUnitimImage(canvas, 3,rotate180);
					setUnitimImage(canvas, 4,rotate90ccw);
					ucnt = 4; bcntw = 4; bcnth = 4;
					break;
				case 10:
					setUnitimImage(canvas, 1,mirror);
					setUnitimImage(canvas, 2,rotate180);
					setUnitimImage(canvas, 3,normal);
					ucnt = 3; bcntw = 3; bcnth = 3;
					break;
				default: break;
			}
	}

	IupSetInt(canvas,"ucnt",ucnt);
	IupSetStrf(canvas,"bcwh","%dx%d", bcntw, bcnth);
}

void setUnitLayout(Ihandle *canvas)
{
	setUnitLayoutImages(canvas);
	setUnitLayoutSize(canvas);
}

Ihandle *getLastChild(Ihandle *ih)
{
	Ihandle *child = IupGetChild(ih, 0);
	int childCount = IupGetChildCount(child);
	while(childCount > 0)
	{
		child = IupGetChild(child, 0);
		childCount = IupGetChildCount(child);
	}
	return child;
}

Ihandle *getChildwName(Ihandle *ih, const char *child_name)
{
	int i = 0;
	Ihandle *childar;
	Ihandle *child;
	int childCount = IupGetChildCount(ih);


	if(childCount > 0)
	{
		if(childCount > 1)
		{
			for(i = 0; i <childCount; i++)
			{
				child = IupGetChild(ih, i);
				if((childar = checkChildName(child, child_name)) != NULL) return childar;
			}
		}
		else
		{
			child = IupGetChild(ih, 0);
			if((childar = checkChildName(child, child_name)) != NULL) return childar;
		}
	}
	return NULL;
}

Ihandle *checkChildName(Ihandle *child, const char *child_name)
{
	Ihandle *childar;
	char *curname = IupGetAttribute(child, "NAME");
	if(curname !=NULL)
	{
		if(strcmp(curname,child_name) == 0)
		{
			free(curname);
			return child;
		}
		free(curname);
	}
	childar = getChildwName(child, child_name);
	curname = IupGetAttribute(childar, "NAME");
	if(curname !=NULL)
	{
		if(strcmp(curname,child_name) == 0)
		{
			free(curname);
			return childar;
		}
		free(curname);
	}
	return NULL;
}

void show_error(const char* message, int is_error)
{
  Ihandle* dlg = IupMessageDlg();
  IupSetStrAttribute(dlg, "PARENTDIALOG", IupGetGlobal("PARENTDIALOG"));
  IupSetAttribute(dlg, "DIALOGTYPE", is_error ? "ERROR" : "WARNING");
  IupSetAttribute(dlg, "BUTTONS", "OK");
  IupSetStrAttribute(dlg, "TITLE", is_error ? "Error" : "Warning");
  IupSetStrAttribute(dlg, "VALUE", message);
  IupPopup(dlg, IUP_CENTERPARENT, IUP_CENTERPARENT);
  IupDestroy(dlg);
}

void show_file_error(int error)
{
  switch (error)
  {
  case IM_ERR_OPEN:
    show_error("Error Opening File.",  1);
    break;
  case IM_ERR_MEM:
    show_error("Insufficient memory.",  1);
    break;
  case IM_ERR_ACCESS:
    show_error("Error Accessing File.",  1);
    break;
  case IM_ERR_DATA:
    show_error("Image type not Supported.",  1);
    break;
  case IM_ERR_FORMAT:
    show_error("Invalid Format.",  1);
    break;
  case IM_ERR_COMPRESS:
    show_error("Invalid or unsupported compression.",  1);
    break;
  default:
    show_error("Unknown Error.",  1);
  }
}

imImage* read_file(const char* filename)
{
  int error;
  imImage* image = imFileImageLoadBitmap(filename, 0, &error);
  if (error)
    show_file_error(error);
  return image;
}

int write_file(const char* filename, const imImage* image)
{
  const char* format = imImageGetAttribString(image, "FileFormat");
  int error = imFileImageSave(filename, format, image);
  if (error)
  {
    show_file_error(error);
    return 0;
  }
  return 1;
}

/* extracted from the SCROLLBAR attribute documentation */
void scroll_update(Ihandle* ih, int view_width, int view_height)
{
  /* view_width and view_height is the virtual space size */
  /* here we assume XMIN=0, XMAX=1, YMIN=0, YMAX=1 */
  int elem_width, elem_height;
  int canvas_width, canvas_height;
  int scrollbar_size = IupGetInt(NULL, "SCROLLBARSIZE");
  int border = IupGetInt(ih, "BORDER");

  IupGetIntInt(ih, "RASTERSIZE", &elem_width, &elem_height);

  /* if view is greater than canvas in one direction,
  then it has scrollbars,
  but this affects the opposite direction */
  elem_width -= 2 * border;  /* remove BORDER (always size=1) */
  elem_height -= 2 * border;
  canvas_width = elem_width;
  canvas_height = elem_height;
  if (view_width > elem_width)  /* check for horizontal scrollbar */
    canvas_height -= scrollbar_size;  /* affect vertical size */
  if (view_height > elem_height)
    canvas_width -= scrollbar_size;
  if (view_width <= elem_width && view_width > canvas_width)  /* check if still has horizontal scrollbar */
    canvas_height -= scrollbar_size;
  if (view_height <= elem_height && view_height > canvas_height)
    canvas_width -= scrollbar_size;
  if (canvas_width < 0) canvas_width = 0;
  if (canvas_height < 0) canvas_height = 0;

  IupSetFloat(ih, "DX", (float)canvas_width / (float)view_width);
  IupSetFloat(ih, "DY", (float)canvas_height / (float)view_height);
}

void scroll_calc_center(Ihandle* ih, float *x, float *y)
{
  *x = IupGetFloat(ih, "POSX") + IupGetFloat(ih, "DX") / 2.0f;
  *y = IupGetFloat(ih, "POSY") + IupGetFloat(ih, "DY") / 2.0f;
}

void scroll_center(Ihandle* ih, float old_center_x, float old_center_y)
{
  /* always update the scroll position
     keeping it proportional to the old position
     relative to the center of the ih. */

  float dx = IupGetFloat(ih, "DX");
  float dy = IupGetFloat(ih, "DY");

  float posx = old_center_x - dx / 2.0f;
  float posy = old_center_y - dy / 2.0f;

  if (posx < 0) posx = 0;
  if (posx > 1 - dx) posx = 1 - dx;

  if (posy < 0) posy = 0;
  if (posy > 1 - dy) posy = 1 - dy;

  IupSetFloat(ih, "POSX", posx);
  IupSetFloat(ih, "POSY", posy);
}

void seq_update(Ihandle* ih, unsigned int seq_val)
{
	Ihandle* seqlblcur = IupGetDialogChild(ih, "SEQLABELCUR");
	unsigned int seq_val_cur = IupGetInt(seqlblcur,"TITLE");

	if(seq_val_cur <= MAXSCENENUM)
	{
		//save current seq scene first
		save_seq_step(ih, seq_val_cur);

		//load new scene
		load_seq_step(ih, seq_val);
	}
}

void zoom_update(Ihandle* ih, double zoom_index)
{
	Ihandle* zoom_lbl = IupGetDialogChild(ih, "ZOOMLABEL");
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
	imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");
	IupSetInt(canvas,"zoom_update", 1);

	double zoom_factor = pow(2, zoom_index);
	  IupSetStrf(zoom_lbl, "TITLE", "%.0f%%", floor(zoom_factor * 100));
	  if (image)
	  {
	    float old_center_x, old_center_y;
	    int view_width, view_height;

	    setUnitLayoutSize(canvas);
	    IupGetIntInt(canvas, "lwh", &view_width, &view_height);

		scroll_calc_center(canvas, &old_center_x, &old_center_y);

		scroll_update(canvas, view_width, view_height);

		scroll_center(canvas, old_center_x, old_center_y);
	  }
	  IupUpdate(canvas);
}

void set_new_image(Ihandle* canvas, imImage* image, const char* filename, int dirty)
{
  imImage* old_image = (imImage*)IupGetAttribute(canvas, "IMAGE");
//  Ihandle* size_lbl = IupGetDialogChild(canvas, "SIZELABEL");
  Ihandle* zoom_val = IupGetDialogChild(canvas, "ZOOMVAL");

//  if (filename)
//	{
//		IupSetStrAttribute(canvas, "FILENAME", filename);
//		IupSetfAttribute(IupGetDialog(canvas), "TITLE", "%s - MAT SEQUENCE", str_filetitle(filename));
//	}
//	else
//	{
		IupSetAttribute(canvas, "FILENAME", NULL);
		IupSetAttribute(IupGetDialog(canvas), "TITLE", "Untitled.seq - MAT SEQUENCE");
//	}

  /* we are going to support only RGB images with no alpha */
  imImageRemoveAlpha(image);
  if (image->color_space != IM_RGB)
  {
    imImage* new_image = imImageCreateBased(image, -1, -1, IM_RGB, -1);
    imConvertColorSpace(image, new_image);
    imImageDestroy(image);

    image = new_image;
  }

  /* default file format */
  const char* format = imImageGetAttribString(image, "FileFormat");
  if (!format)
    imImageSetAttribString(image, "FileFormat", "JPEG");

  IupSetAttribute(canvas, "DIRTY", dirty? "Yes": "No");
  IupSetAttribute(canvas, "IMAGE", (char*)image);
//
//  FILE *pFile = fopen("imgdata", "wb");
//  int i = 0;
//  fprintf(pFile, "unsigned int defimgWidth = %d;\n", image->width);
//  fprintf(pFile, "unsigned int defimgHeight = %d;\n", image->height);
//  fprintf(pFile, "unsigned int defimgDepth = %d;\n", image->depth);
//  fprintf(pFile, "unsigned char defimgData[] = \n{");
//  printf("%d x %d x %d", image->width, image->height, image->depth);
//  for (int d = 0; d < image->depth; d++)
//  {
//    unsigned char* idata = (unsigned char*)image->data[d];
//
//    for (int y = 0; y < image->height; y++)
//    {
//      for (int x = 0; x < image->width; x++)
//      {
//        int offset = y * image->width + x;
//
//        fprintf(pFile, "%d, ", idata[offset]);
//        i++;
//        if((i % 30) ==0)fprintf(pFile, "\n");
//      }
//    }
//  }
//  fprintf(pFile, "\nunsigned char defimgOffset = %d\n", i);
//  fclose (pFile);
//  IupSetfAttribute(size_lbl, "TITLE", "%d x %d px", image->width, image->height);

  if (old_image)
    imImageDestroy(old_image);

  IupSetDouble(zoom_val, "VALUE", 0);

  setUnitLayout(canvas);
  zoom_update(canvas, 0);
}

void check_new_file(Ihandle* dlg)
{
  Ihandle* canvas = IupGetDialogChild(dlg, "CANVAS");
  imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");
  if (!image)
  {
    Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");
    int width = IupConfigGetVariableIntDef(config, "NewImage", "Width", 640);
    int height = IupConfigGetVariableIntDef(config, "NewImage", "Height", 480);

    image = imImageCreate(width, height, IM_RGB, IM_BYTE);

    set_new_image(canvas, image, NULL, 0);
  }
}

void open_file_seq(Ihandle* ih, const char* filename)
{
	unsigned int i = 0, j, k;
	unsigned int seq_val_max = 0, layout_num, speed_factor, machine_no, seq_val, seqnum_val;
	char seqfile[22];
	char lineStr[35];
	char bptr[64];
	unsigned int bptrNum; //,  bptrNum2, bptrNum3, bptrNum4, dwell_val;
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
	Ihandle* seqlblmax = IupGetDialogChild(ih, "SEQLABEL");
	Ihandle* machno = IupGetDialogChild(ih, "MACHNOTEXT");
	Ihandle* speedfac = IupGetDialogChild(ih, "SPEEDFACTORTEXT");
	Ihandle* speedfacshow = IupGetDialogChild(ih, "SPEEDFACTORTEXTSHOW");
	Ihandle* seqnumtxt = IupGetDialogChild(ih, "SEQNUMTEXT");

	if (filename)
	{
		IupSetStrAttribute(canvas, "FILENAME", filename);
		IupSetfAttribute(IupGetDialog(canvas), "TITLE", "%s - MAT SEQUENCE", str_filetitle(filename));
	}
	else
	{
		IupSetAttribute(canvas, "FILENAME", NULL);
		IupSetAttribute(IupGetDialog(canvas), "TITLE", "Untitled - MAT SEQUENCE");
	}

	FILE *pFileWrite;
	FILE *pFileRead = fopen (filename,"rb");
	if (pFileRead!=NULL)
	{
		remove_seq_step_files(IupGetInt(seqlblmax, "TITLE") + 1);
		fgets(lineStr,35, pFileRead); //write enable
		fgets(lineStr,35, pFileRead); // machine no
		machine_no = getIntHexCmpChar(lineStr, "*3444", 0, 1);
//		printf("machine num = %d\n", machine_no);
		fgets(lineStr,35, pFileRead); // layout num
		layout_num = getIntHexCmpChar(lineStr, "*3443",0, 1);
		fgets(lineStr,35, pFileRead); // seq num
		seqnum_val = getIntHexCmpChar(lineStr, "*3531",0, 1);
		fgets(lineStr,35, pFileRead); // speed factor
		speed_factor = getIntHexCmpChar(lineStr, "*3538",0, 1);

//		printf("machine no = %d, layout_num = %d, seq = %d, speedfac = %d\n", machine_no, layout_num, seq_val_max, speed_factor);

		seq_val_max = 0;
		i = 0;
		while(i < 0xffff)
		{
			i++;
			fgets(lineStr,35, pFileRead);
			if(getIntHexCmpChar(lineStr,"*35",0, 1) == 0x37) break;
			else
			{
				seq_val_max++;
				seq_val = getIntHexChar(lineStr,5,2);
				sprintf(seqfile,"%s/seq_%d",tmpfolder, seq_val);
				pFileWrite = fopen(seqfile,"wb");
				if (pFileWrite!=NULL)
				{
					for(j = 0; j < 4; j++)
					{
						bptrNum = getIntHexChar(lineStr,21 - (4*j),2);
						for(k = 0; k<16;k++)
						{
							if(bptrNum & (1 << k)) bptr[16 * j + k] = 1;
							else bptr[16 * j + k] = 0;
						}
					}
					save_seq_step_file(seq_val, bptr, getIntHexChar(lineStr,25,2));
					fclose(pFileWrite);
				}
			}
		}
		fclose (pFileRead);
	}
	if(seq_val_max > 0)
	{
		Ihandle* seqval = IupGetDialogChild(ih, "SEQVAL");
		Ihandle* layoutlbl = IupGetDialogChild(ih, "LAYLABEL");

		IupSetInt(seqlblmax,"TITLE", seq_val_max);
		IupSetInt(seqval,"MAX", seq_val_max -1);
		IupSetInt(seqval,"SHOWTICKS", seq_val_max);
		IupSetInt(canvas,"layout_num", layout_num);
		IupSetInt(layoutlbl, "TITLE", layout_num);
		IupSetInt(machno, "VALUE", machine_no);
		IupSetInt(seqnumtxt, "VALUE", seqnum_val);
		IupSetInt(speedfac, "VALUE", speed_factor);
		IupSetInt(speedfacshow, "VALUE", speed_factor * 100 /128);
		load_seq_step(ih,1);
		setUnitLayoutImages(canvas);
		setUnitLayoutSize(canvas);

		IupUpdate(canvas);
		remove("imgfile");
	}
}

void open_file_isf(Ihandle* ih, const char* filename)
{
	unsigned int j;
	unsigned int seq_val_max = 0, layout_num, speed_factor, machine_no, seqnumtxt_val;
	char seqfile[20];
	char imgfiletmp[15];
	char c;
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
	Ihandle* seqlblmax = IupGetDialogChild(ih, "SEQLABEL");
	Ihandle* machno = IupGetDialogChild(ih, "MACHNOTEXT");
	Ihandle* speedfac = IupGetDialogChild(ih, "SPEEDFACTORTEXT");
	Ihandle* speedfacshow = IupGetDialogChild(ih, "SPEEDFACTORTEXTSHOW");
	Ihandle* seqnumtxt = IupGetDialogChild(ih, "SEQNUMTEXT");

	if (filename)
	{
		IupSetStrAttribute(canvas, "FILENAME", filename);
		IupSetfAttribute(IupGetDialog(canvas), "TITLE", "%s - MAT SEQUENCE", str_filetitle(filename));
	}
	else
	{
		IupSetAttribute(canvas, "FILENAME", NULL);
		IupSetAttribute(IupGetDialog(canvas), "TITLE", "Untitled - MAT SEQUENCE");
	}

	FILE *pFileWrite;
	FILE *pFileRead = fopen (filename,"rb");
	if (pFileRead!=NULL)
	{
		remove_seq_step_files(IupGetInt(seqlblmax, "TITLE") + 1);
//		layout_num = fgetc(pFileRead);
		fscanf(pFileRead,"%d", &machine_no);
		fscanf(pFileRead,"%d", &layout_num);
		fscanf(pFileRead,"%d", &seqnumtxt_val);
		fscanf(pFileRead,"%d", &speed_factor);
		c = fgetc(pFileRead);
		if(c == '\n')seq_val_max++;
		j=1;
		while(c!='\0')
		{
			sprintf(seqfile,"%s/seq_%d", tmpfolder, j);
			pFileWrite = fopen(seqfile,"wb");
			if (pFileWrite!=NULL)
			{
				c = fgetc(pFileRead);
				while(c!='\n')
				{
					fputc(c, pFileWrite);
					c = fgetc(pFileRead);
					if(c == '\0') break;
				}
				fclose(pFileWrite);
				if(c == '\n')seq_val_max++;
			}
			j++;
		}
		c = fgetc(pFileRead);
		if(c=='\n')
		{
			sprintf(imgfiletmp, "%s/imgfile", tmpfolder);
			pFileWrite = fopen(imgfiletmp, "wb");
			if (pFileWrite!=NULL)
			{
				while(!feof(pFileRead))
				{
					c = fgetc(pFileRead);
					fputc(c,pFileWrite);
				}
				fclose(pFileWrite);
			}
		}
		fclose (pFileRead);
	}
	if(seq_val_max > 0)
	{
		Ihandle* seqval = IupGetDialogChild(ih, "SEQVAL");
		Ihandle* layoutlbl = IupGetDialogChild(ih, "LAYLABEL");

		int error;
		imImage *img = imFileImageLoad(imgfiletmp, 0, &error);

		if(img)
			IupSetAttribute(canvas, "IMAGE", (char*)img);

		IupSetInt(seqlblmax,"TITLE", seq_val_max);
		IupSetInt(seqval,"MAX", seq_val_max -1);
		IupSetInt(seqval,"SHOWTICKS", seq_val_max);
		IupSetInt(canvas,"layout_num", layout_num);
		IupSetInt(layoutlbl, "TITLE", layout_num);
		IupSetInt(machno, "VALUE", machine_no);
		IupSetInt(seqnumtxt, "VALUE", seqnumtxt_val);
		IupSetInt(speedfac, "VALUE", speed_factor);
		IupSetInt(speedfacshow, "VALUE", speed_factor * 100 /128);
		load_seq_step(ih,1);

		setUnitLayoutImages(canvas);
		setUnitLayoutSize(canvas);

		IupUpdate(canvas);
		remove(imgfiletmp);
	}
}

void open_file(Ihandle* ih, const char* filename)
{
	const char* ext = str_fileext(filename);
	if (str_compare(ext, "seq", 0))
			open_file_seq(ih,filename);
	else if (str_compare(ext, "isf", 0))
		open_file_isf(ih,filename);
	else
	{
		imImage* image = read_file(filename);
		if (image)
		{
			Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
			Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");

			set_new_image(canvas, image, filename, 0);

			IupConfigRecentUpdate(config, filename);
		}
	}
}

void save_file(Ihandle* canvas)
{
  char* filename = IupGetAttribute(canvas, "FILENAME");
  imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");

  if (write_file(filename, image))
    IupSetAttribute(canvas, "DIRTY", "NO");
}

void set_file_format(imImage* image, const char* filename)
{
  const char* ext = str_fileext(filename);
  const char* format = "JPEG";
  if (str_compare(ext, "jpg", 0) || str_compare(ext, "jpeg", 0))
    format = "JPEG";
  else if (str_compare(ext, "bmp", 0))
    format = "BMP";
  else if (str_compare(ext, "png", 0))
    format = "PNG";
  else if (str_compare(ext, "tga", 0))
    format = "TGA";
  else if (str_compare(ext, "tif", 0) || str_compare(ext, "tiff", 0))
    format = "TIFF";
  imImageSetAttribString(image, "FileFormat", format);
}

void save_file_seq(Ihandle* ih, const char* filename)
{
	unsigned int i = 0, j, k, bptrCnt, ubptr_val;
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
	Ihandle* seqlblmax = IupGetDialogChild(ih, "SEQLABEL");
	Ihandle* machno = IupGetDialogChild(ih, "MACHNOTEXT");
	Ihandle* speedfac = IupGetDialogChild(ih, "SPEEDFACTORTEXT");
	Ihandle* seqnumtxt = IupGetDialogChild(ih, "SEQNUMTEXT");

	unsigned int seq_val_max = IupGetInt(seqlblmax, "TITLE");
	unsigned int seqnumtxt_val = IupGetInt(seqnumtxt, "VALUE");
	unsigned int dwell_val;
	char seqfile[20];
	unsigned char lineStr[14];
	char bptrStr[64];

  	FILE *pFile = fopen (filename,"wb");
  	FILE *pFileRead;
	if (pFile!=NULL)
	{
		//write enable
		sprintf((char*)lineStr,"%c%c%c", 0x35,0x37,0xff);
		fprintf(pFile,"*%02X%02X%02X%02X\r\n", lineStr[0],lineStr[1],lineStr[2], getCheckSumCharArr(lineStr, 0, 3));

		//machine num
		sprintf((char*)lineStr,"%c%c%c", 0x34,0x44,IupGetInt(machno, "VALUE"));
		fprintf(pFile,"*%02X%02X%02X%02X\r\n", lineStr[0],lineStr[1],lineStr[2],getCheckSumCharArr(lineStr, 0, 3));

		//layout num
		sprintf((char*)lineStr,"%c%c%c", 0x34,0x43,IupGetInt(canvas,"layout_num"));
		fprintf(pFile,"*%02X%02X%02X%02X\r\n", lineStr[0],lineStr[1],lineStr[2],getCheckSumCharArr(lineStr, 0, 3));

		//sequence num
		sprintf((char*)lineStr,"%c%c%c", 0x35,0x31,seqnumtxt_val);
		fprintf(pFile,"*%02X%02X%02X%02X\r\n", lineStr[0],lineStr[1],lineStr[2],getCheckSumCharArr(lineStr, 0, 3));

		//speed factor
		sprintf((char*)lineStr,"%c%c%c", 0x35,0x38,IupGetInt(speedfac, "VALUE"));
		fprintf(pFile,"*%02X%02X%02X%02X\r\n", lineStr[0],lineStr[1],lineStr[2],getCheckSumCharArr(lineStr, 0, 3));

		sprintf((char*)lineStr,"%c%c", 0x35,0x33);
		for(i = 0; i < seq_val_max;i++)
		{
			sprintf(seqfile,"%s/seq_%d", tmpfolder, i+1);
			lineStr[2] = (i+1) >> 8;
			lineStr[3] = 0xff & (i+1);

			pFileRead = fopen(seqfile,"rb");
			if (pFileRead!=NULL)
			{
				for(bptrCnt = 0;bptrCnt<64;bptrCnt++)
				{
					bptrStr[bptrCnt]= fgetc(pFileRead);
				}
				fscanf(pFileRead,"%u", &dwell_val);
				fclose(pFileRead);
//				printf("dwell = %d\n", dwell_val);
				for(j = 0; j < 4; j++)
				{
					k = 16;
					ubptr_val = 0;
					while(k > 0)
					{
						k--;
						bptrCnt--;
						ubptr_val = ubptr_val << 1;
						if(bptrStr[bptrCnt] > 1) ubptr_val +=1;
					}
//					printf("ubptr_val = %d\n", ubptr_val);
					lineStr[4+j*2] = (ubptr_val >> 8) & 0xff;
					lineStr[5+j*2] = ubptr_val & 0xff;
				}
				lineStr[12] = (dwell_val >> 8) & 0xff;
				lineStr[13] = dwell_val & 0xff;
				fputc('*', pFile);
				for(j = 0; j<14;j++)
				{
					fprintf(pFile,"%02X", lineStr[j]);
				}
				fprintf(pFile,"%02X\r\n", getCheckSumCharArr(lineStr, 0, 14));
			}
		}
		//write disable
		sprintf((char*)lineStr,"%c%c%c", 0x35,0x37,0x00);
		fprintf(pFile,"*%02X%02X%02X%02X\r\n", lineStr[0],lineStr[1],lineStr[2], getCheckSumCharArr(lineStr, 0, 3));
		fclose (pFile);
	}
}

void save_file_isf(Ihandle* canvas, const char* filename)
{
	unsigned int i = 0;
	Ihandle* seqlblmax = IupGetDialogChild(canvas, "SEQLABEL");
	Ihandle* machno = IupGetDialogChild(canvas, "MACHNOTEXT");
	Ihandle* speedfac = IupGetDialogChild(canvas, "SPEEDFACTORTEXT");
	Ihandle* seqnumtxt = IupGetDialogChild(canvas, "SEQNUMTEXT");
	unsigned int seq_val_max = IupGetInt(seqlblmax, "TITLE");
	imImage *img = (imImage *)IupGetAttribute(canvas,"IMAGE");
	char seqfile[15];
	char c;

  	FILE *pFile = fopen (filename,"wb");
  	FILE *pFileRead;
	if (pFile!=NULL)
	{
		fprintf(pFile, "%d %d %d %d\n", IupGetInt(machno,"VALUE"), IupGetInt(canvas,"layout_num"),  IupGetInt(seqnumtxt,"VALUE"), IupGetInt(speedfac,"VALUE"));
		for(i = 0; i < seq_val_max;i++)
		{
			sprintf(seqfile,"%s/seq_%d", tmpfolder, i+1);
			pFileRead = fopen(seqfile,"rb");
			if (pFileRead!=NULL)
			{
				while(!feof(pFileRead))
				{
					c = fgetc(pFileRead);
					fputc(c, pFile);
				}
				fclose(pFileRead);
				if((i + 1) < seq_val_max) fputc('\n', pFile);
				else fputc('\0', pFile);
			}
		}
		fputc('\n', pFile);
		write_file("imgfile",img);
		pFileRead = fopen("imgfile","rb");
		if (pFileRead!=NULL)
		{
			while(!feof(pFileRead))
			{
				c = fgetc(pFileRead);
				fputc(c, pFile);
			}
			fclose(pFileRead);
		}
		fclose (pFile);
		remove("imgfile");
	}
}

void saveas_file(Ihandle* canvas, const char* filename)
{
  const char* ext = str_fileext(filename);
  char * pch;
  char *strfilenameold = IupGetAttribute(canvas, "FILENAME");
  char * strfilename = NULL;

  if(ext == NULL)
  {
	  printf("ext null\n");
	  strfilename = (char*) malloc(strlen(filename) + 5);
	  sprintf(strfilename, "%s%s", filename, ".seq");
	  save_file_seq(canvas,strfilename);
	  free(strfilenameold);
	  IupSetStrAttribute(canvas, "FILENAME", strfilename);
	  IupSetfAttribute(IupGetDialog(canvas), "TITLE", "%s - MAT SEQUENCE", str_filetitle(strfilename));
  }
  else if (str_compare(ext, "isf", 0))
  {
    		save_file_isf(canvas,filename);
    		IupSetfAttribute(IupGetDialog(canvas), "TITLE", "%s - MAT SEQUENCE", str_filetitle(filename));

  }
  else if(!str_compare(ext, "seq", 0))
  {
	  strfilename = (char*) malloc(strlen(filename) + 1);
	  memcpy(strfilename, filename, strlen(filename));
		pch = strstr (strfilename,ext);
		strncpy (pch,"seq\0",4);
		save_file_seq(canvas,strfilename);
		free(strfilenameold);
		IupSetStrAttribute(canvas, "FILENAME", strfilename);
		IupSetfAttribute(IupGetDialog(canvas), "TITLE", "%s - MAT SEQUENCE", str_filetitle(strfilename));
  }
  else
  {
	  free(strfilenameold);
	  save_file_seq(canvas,filename);
	  IupSetStrAttribute(canvas, "FILENAME", filename);
	  IupSetfAttribute(IupGetDialog(canvas), "TITLE", "%s - MAT SEQUENCE", str_filetitle(filename));
  }
//  printf("strfilename = %s\n", strfilename);
//  if (str_compare(ext, "seq", 0))
//  {





//  }
//  else if (str_compare(ext, "isf", 0))
//  		save_file_isf(canvas,filename);
//  	else
//  	{
//	  imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");
//	  set_file_format(image, filename);
//
//	  if (write_file(filename, image))
//	  {
//		Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");
//
//		IupSetfAttribute(IupGetDialog(canvas), "TITLE", "%s - Simple Paint", str_filetitle(filename));
//		IupSetStrAttribute(canvas, "FILENAME", filename);
//		IupSetAttribute(canvas, "DIRTY", "NO");
//
//		IupConfigRecentUpdate(config, filename);
//	  }
//  	}
}

int save_check(Ihandle* ih)
{
  Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
  if (IupGetInt(canvas, "DIRTY"))
  {
    switch (IupAlarm("Warning", "File not saved! Save it now?", "Yes", "No", "Cancel"))
    {
    case 1:  /* save the changes and continue */
      save_file(canvas);
      break;
    case 2:  /* ignore the changes and continue */
      break;
    case 3:  /* cancel */
      return 0;
    }
  }
  return 1;
}

int select_file_isf(Ihandle* parent_dlg, unsigned char is_open)
{
	Ihandle* config = (Ihandle*)IupGetAttribute(parent_dlg, "CONFIG");
	  Ihandle* canvas = IupGetDialogChild(parent_dlg, "CANVAS");
	  const char* dir = IupConfigGetVariableStr(config, "MainWindow", "LastDirectory");

	  Ihandle* filedlg = IupFileDlg();
	  if (is_open)
	    IupSetAttribute(filedlg, "DIALOGTYPE", "OPEN");
	  else
	  {
	    IupSetAttribute(filedlg, "DIALOGTYPE", "SAVE");
	    IupSetStrAttribute(filedlg, "FILE", IupGetAttribute(canvas, "FILENAME"));
	  }
	  IupSetAttribute(filedlg, "EXTFILTER", "SEQ Files|*.seq;|All Files|*.*|");
	  IupSetStrAttribute(filedlg, "DIRECTORY", dir);
	  IupSetAttributeHandle(filedlg, "PARENTDIALOG", parent_dlg);

	  IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);
	  if (IupGetInt(filedlg, "STATUS") != -1)
	  {
	    char* filename = IupGetAttribute(filedlg, "VALUE");
	    if (is_open)
	      open_file_isf(parent_dlg, filename);
	    else
	      save_file_isf(canvas, filename);
	  }

	  IupDestroy(filedlg);
	return IUP_DEFAULT;
}

int select_file(Ihandle* parent_dlg, int is_open)
{
  Ihandle* config = (Ihandle*)IupGetAttribute(parent_dlg, "CONFIG");
  Ihandle* canvas = IupGetDialogChild(parent_dlg, "CANVAS");
  IupSetInt(canvas,"open_update",1);

  const char* dir = IupConfigGetVariableStr(config, "MainWindow", "LastDirectory");

  Ihandle* filedlg = IupFileDlg();
  if (is_open)
    IupSetAttribute(filedlg, "DIALOGTYPE", "OPEN");
  else
  {
	time_t rawtime;
	struct tm * timeinfo;
	char timebuffer[13];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (timebuffer,13,"%y%m%d%H%M%S",timeinfo);

    IupSetAttribute(filedlg, "DIALOGTYPE", "SAVE");
//    IupSetStrAttribute(filedlg, "FILE", IupGetAttribute(canvas, "FILENAME"));
    IupSetStrf(filedlg, "FILE", "matno_%03d_lay_%02d_%02X%02X_%s.seq",
    		IupGetInt(IupGetDialogChild(parent_dlg, "MACHNOTEXT"), "VALUE"),
    		IupGetInt(IupGetDialogChild(parent_dlg, "LAYLABEL"), "TITLE"),
			IupGetInt(IupGetDialogChild(parent_dlg, "SEQLABEL"), "TITLE"),
			IupGetInt(IupGetDialogChild(parent_dlg, "SPEEDFACTORTEXT"), "VALUE"),
			timebuffer);
  }
//  IupSetAttribute(filedlg, "EXTFILTER", "Sequence Files|*.seq|Image & Sequence Files|*.bmp;*.jpg;*.png;*.tif;*.tga; *.seq; *.isf|All Files|*.*|");
  IupSetAttribute(filedlg, "EXTFILTER", "Sequence File (.seq)|*.seq|");
  IupSetStrAttribute(filedlg, "DIRECTORY", dir);
  IupSetAttributeHandle(filedlg, "PARENTDIALOG", parent_dlg);

  IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);
  if (IupGetInt(filedlg, "STATUS") != -1)
  {
    char* filename = IupGetAttribute(filedlg, "VALUE");
    if (is_open)
      open_file(parent_dlg, filename);
    else
      saveas_file(canvas, filename);

    dir = IupGetAttribute(filedlg, "DIRECTORY");
    IupConfigSetVariableStr(config, "MainWindow", "LastDirectory", dir);
  }

  IupDestroy(filedlg);
  return IUP_DEFAULT;
}

void view_fit_rect(int canvas_width, int canvas_height, int image_width, int image_height, int *view_width, int *view_height)
{
  *view_width = canvas_width;
  *view_height = (canvas_width * image_height) / image_width;

  if (*view_height > canvas_height)
  {
    *view_height = canvas_height;
    *view_width = (canvas_height * image_width) / image_height;
  }
}

unsigned char isExistComList(Ihandle* comlist, char* txt)
{
	int i = 0;
	int listcnt = IupGetInt(comlist, "COUNT");
	char strTmp[6];
	char *txtbuf;
	size_t txtbuflen, txtlen;

	txtlen = strlen(txt);

	for(i=0; i < listcnt; i++)
	{
		sprintf(strTmp, "%d", i + 1);
		txtbuf=IupGetAttribute(comlist,strTmp);
		txtbuflen = strlen(txtbuf);

		if(txtlen == txtbuflen)
		{
			if(strncmp(txtbuf, txt, txtlen) == 0)
			{
				return  1;
			}
		}
	}
	return 0;
}

void appendUpComList(Ihandle* comlist, char* txt)
{
	int i = IupGetInt(comlist, "COUNT");
	char strTmp[6];
	char *txtbuf;

	IupSetAttribute(comlist, "APPENDITEM", txt);

	while(i > 1)
	{
		sprintf(strTmp, "%d", i);
		txtbuf = IupGetAttribute(comlist, strTmp);
		sprintf(strTmp, "%d", i + 1);
		IupSetAttribute(comlist, strTmp,  txtbuf);
		i--;
	}
	IupSetAttribute(comlist, "1", txt);
}

void updateComportTxtList(Ihandle* comlist, char* txt)
{
	size_t len = strlen(txt);
	if(len > 0)
	{
		if(!isExistComList(comlist, txt))
		{
			IupSetAttribute(comlist, "APPENDITEM", txt);
		}
	}
}

void saveComPortConfig(Ihandle *ih)
{
	int i=0;
	char strTmp[5];
//	int maxlistnum = 20;
	Ihandle* comlisttxt =IupGetDialogChild(ih, "COMPORTTXTLIST");
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
	Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");

	int listcnt = IupGetInt(comlisttxt, "COUNT");

	if(listcnt > 0)
	{
//		if(listcnt < maxlistnum)
			IupConfigSetVariableInt(config,"ComPort", "CommandCnt", listcnt);
//		else
//			IupConfigSetVariableInt(config,"ComPort", "CommandCnt", 10);

		while(i < listcnt)
		{
			i++;
			sprintf(strTmp,"%d",i);
			IupConfigSetVariableStrId(config, "ComPort", "Command", i, IupGetAttribute(comlisttxt, strTmp));
		}
	}
}

void sendComportCanvasState(Ihandle *canvas)
{
	Ihandle* dwelltxt = IupGetDialogChild(IupGetDialog(canvas), "DWELLTEXT");
	Ihandle* comportlist = IupGetDialogChild(IupGetDialog(canvas), "COMPORTLIST");

	int comnum =IupGetInt(comportlist, "VALUESTRING") - 1;
	unsigned int dwell_val = IupGetInt(dwelltxt,"VALUE");
	char *bptrStr=IupGetAttribute(canvas,"block_pointer");

	unsigned char j,k, bptrCnt = 64;
	unsigned int ubptr_val;
	unsigned char lineStr[14];
	char strCom[33];
	char strHexTmp[3];

	sprintf((char*)lineStr,"%c%c", 0x35,0x33);
	strncpy(strCom, "*", 2);

	for(j = 0; j < 4; j++)
	{
		k = 16;
		ubptr_val = 0;
		while(k > 0)
		{
			k--;
			bptrCnt--;
			ubptr_val = ubptr_val << 1;
			if(bptrStr[bptrCnt] > 0) ubptr_val +=1;
		}

		lineStr[4+j*2] = (ubptr_val >> 8) & 0xff;
		lineStr[5+j*2] = ubptr_val & 0xff;
	}
	lineStr[12] = (dwell_val >> 8) & 0xff;
	lineStr[13] = dwell_val & 0xff;
	for(j = 0; j<14;j++)
	{
		sprintf(strHexTmp, "%02X", lineStr[j]);
		strcat(strCom,strHexTmp);
	}
	sprintf(strHexTmp, "%02X", getCheckSumCharArr(lineStr, 0, 14));
	strcat(strCom,strHexTmp);

	sendComPortPuts(comnum, strCom);
	sendComPortByte(comnum, 13);
}

void loadSceneImage(Ihandle *dialog)
{
	imImage *image = imImageCreate(defimgWidth, defimgHeight, IM_RGB, IM_BYTE);
	  	unsigned int i = 0;
	  	for (int d = 0; d < image->depth; d++)
	  	  {
	  		unsigned char* idata = (unsigned char*)image->data[d];

	  		for (int y = 0; y < image->height; y++)
	  		{
	  		  for (int x = 0; x < image->width; x++)
	  		  {
	  			int offset = y * image->width + x;

	  			idata[offset] = defimgData[i];
	  			i++;
	  		  }
	  		}
	  	  }
	  	  set_new_image(IupGetDialogChild(dialog, "CANVAS"), image, "untitled.seq", 0);
}
