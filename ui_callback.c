#include "utils.h"
#include "ui_utils.h"

int allexpand_action_cb(Ihandle *ih)
{
	unsigned char i;
	Ihandle *canvas = IupGetDialogChild(ih, "CANVAS");
	char *bptr= IupGetAttribute(canvas,"block_pointer");

	for (i = 0; i < 64; i++)
		bptr[i] = 1;

	IupSetAttribute(canvas,"block_pointer", bptr);
	IupUpdate(canvas);
	return IUP_DEFAULT;
}

int allnormal_action_cb(Ihandle *ih)
{
	unsigned char i;
	Ihandle *canvas = IupGetDialogChild(ih, "CANVAS");
	char *bptr= IupGetAttribute(canvas,"block_pointer");

	for (i = 0; i < 64; i++)
		bptr[i] = 0;

	IupSetAttribute(canvas,"block_pointer", bptr);
	IupUpdate(canvas);
	return IUP_DEFAULT;
}

int item_test_action_cb(Ihandle *ih)
{
	FILE *pFile = fopen("testdata", "wb");
	if(pFile)
	{
	  unsigned int i;
	  fprintf(pFile,"char comports[256][11]={");
	  for(i=0;i<256;i++)
	  {
		  fprintf(pFile,"\"\\\\\\\\.\\\\COM%d\",", i+1);
		  if((i %20) == 0) fputs("\n\t", pFile);
	  }
	  fputs("};\n", pFile);
	  fclose (pFile);
	}

	return IUP_DEFAULT;
}

int comporttxtlist_dropped_cb(Ihandle* comporttxtlist)
{
	char *txtbuf = IupGetAttribute(comporttxtlist,"VALUE");
	updateComportTxtList(comporttxtlist, txtbuf);
	return IUP_DEFAULT;
}

int comporttxtlist_valuechanged_cb(Ihandle* comporttxtlist)
{
	printf("value changed\n");
	return IUP_DEFAULT;
}

int comportlist_action_cb(Ihandle *comportlist, char *text, int item, int state)
{
	Ihandle* comporttimer = IupGetDialogChild(comportlist, "COMPORTTIMER");

	if(state)
		IupSetAttribute(IupGetDialogChild(comportlist, "COMPORTBUTTON"), "ACTIVE", "YES");
	else
	{
		IupSetAttribute(comporttimer, "RUN", "YES");
		IupSetAttribute(IupGetDialogChild(comportlist, "COMPORTBUTTON"), "ACTIVE", "NO");
	}
	return IUP_DEFAULT;
}

int comporttxtlist_action_cb(Ihandle *comporttxtlist, char *text, int item, int state)
{
	printf("text = %s, item = %d, state = %d\n", text, item, state);
	return IUP_DEFAULT;
}

int item_sendcomport_action_cb(Ihandle *ih)
{
	Ihandle* comportlist = IupGetDialogChild(ih, "COMPORTLIST");
	Ihandle* comporttxtlist = IupGetDialogChild(ih, "COMPORTTXTLIST");
	int comnum =IupGetInt(comportlist, "VALUESTRING") - 1;
	char *txtbuf = IupGetAttribute(comporttxtlist,"VALUE");

	sendComPortPuts(comnum, txtbuf);
	sendComPortByte(comnum, 13);

	updateComportTxtList(comporttxtlist, txtbuf);
	return IUP_DEFAULT;
}

int item_autocomport_action_cb(Ihandle *ih)
{
	int i;
	unsigned char buf[4096];
	for(i = 0; i < 16; i++)
	{
		if(openComPort(i,115200))
		{
			printf("port=%d. sending request machine no.\n", i);
			sendComPortPuts(i, "?MB2\r");

			getComPortPoll(i, buf,4095);
			closeComPort(i);
		}
	}
	return IUP_DEFAULT;
}

int comport_timersearch_action_cb(Ihandle *ih)
{
	Ihandle* comporttimer = IupGetDialogChild(ih, "COMPORTTIMER");
	Ihandle* comportlist = IupGetDialogChild(ih, "COMPORTLIST");
	Ihandle* matmculabel = IupGetDialogChild(ih,"MATMCULABEL");

	static unsigned char comportnumsearch = 0;
	unsigned char i;
	char strTmp[4];
	unsigned char comportlistExistFlag = 0;
	unsigned char comportlistNullFlag = 0;
	unsigned char comportlistCnt = IupGetInt(comportlist, "COUNT");
//	unsigned char firstRun = 1;
	unsigned char comportlistval;
	int comnum =IupGetInt(comportlist, "VALUESTRING");
//	if(firstRun)
//	{
//		firstRun = 0;
//		for(i = 0; i < 0xff; i++)
//		{
//			closeComPort(i);
//		}
//	}

	if(comnum)
	{
		IupSetAttribute(ih, "RUN", "NO");
		IupSetAttribute(IupGetDialogChild(ih, "COMPORTBUTTON"), "ACTIVE", "YES");
	}
	else if(strcmp(IupGetAttribute(matmculabel, "TITLE"), "Found") !=0)
	{
		for(i = 1; i < comportlistCnt; i++)
		{
			sprintf(strTmp, "%d", i + 1);
			comportlistval = IupGetInt(comportlist, strTmp);
			if(comportlistval == (comportnumsearch + 1)) {comportlistExistFlag = 1; break;}
		}
		if(!comportlistExistFlag)
		{
			if(comPortisAvailable(comportnumsearch,115200))
			{
				for(i = 1; i < comportlistCnt; i++)
				{
					sprintf(strTmp, "%d", i + 1);
					comportlistval = IupGetInt(comportlist, strTmp);
					if(!comportlistval)
					{
//						printf("insert comport %d\n", comportnumsearch);
						IupSetInt(comportlist, strTmp, comportnumsearch + 1);
						comportlistNullFlag = 1;
						break;
					}
				}
				if(!comportlistNullFlag)
				{
//					printf("add comport %d\n", comportnumsearch);
					sprintf(strTmp, "%d", comportlistCnt + 2);
					IupSetStrf(comportlist, "APPENDITEM", "%d", comportnumsearch + 1);
				}
				if(!IupGetInt(comporttimer, "RUN")) IupSetAttribute(comporttimer, "RUN", "YES");
			}
		}
		if(comportnumsearch < 0xff) comportnumsearch++;
		else comportnumsearch = 0;
	}
//	else
//	{
//		IupSetAttribute(ih, "RUN", "NO");
//		IupSetAttribute(IupGetDialogChild(ih, "COMPORTBUTTON"), "ACTIVE", "YES");
//		IupSetAttribute(IupGetDialogChild(ih, "COMPORTTIMER"), "RUN", "NO");
//		IupSetAttribute(comportlist, "2", NULL);
//	}
	return IUP_DEFAULT;
}

//int comport_timersearch_action_cb(Ihandle *ih)
//{
//	Ihandle* comporttimer = IupGetDialogChild(ih, "COMPORTTIMER");
//	Ihandle* comportlist = IupGetDialogChild(ih, "COMPORTLIST");
//	Ihandle* matmculabel = IupGetDialogChild(ih,"MATMCULABEL");
//	unsigned char i;
//	char strTmp[4];
//
////	IupSetAttribute(comporttimer, "RUN", "NO");
//	if(strcmp(IupGetAttribute(matmculabel, "TITLE"), "Found") !=0)
//	{
//		for(i = 0; i < 0xff; i++)
//		{
////			printf("searching comport %d\n", i);
//			if(comPortisAvailable(i,115200))
//			{
////				printf("open comport %d\n", i);
//				sprintf(strTmp, "%d", i + 1);
//				IupSetStrf(comportlist, "APPENDITEM", "%d", i + 1);
////				closeComPort(i);
////				if(!IupGetInt(comporttimer, "RUN")) IupSetAttribute(comporttimer, "RUN", "YES");
//			}
//		}
//	}
//	else
//	{
////		printf("stop searching\n");
//		IupSetAttribute(ih, "RUN", "NO");
//		IupSetAttribute(IupGetDialogChild(ih, "COMPORTBUTTON"), "ACTIVE", "YES");
//	}
//	return IUP_DEFAULT;
//}

int comport_timer_action_cb(Ihandle *ih)
{
	Ihandle* comportlist = IupGetDialogChild(ih, "COMPORTLIST");
	Ihandle* matmculabel = IupGetDialogChild(ih,"MATMCULABEL");
	int comnum =IupGetInt(comportlist, "VALUESTRING");
	unsigned char comportlistcnt = IupGetInt(comportlist,"COUNT");
	static int comnumdetect = 0;
	static unsigned char comportNullCnt[256];
	static unsigned char strStatusFlag=0;
//	static unsigned int openCnt = 0;

	int n = 0;
	int i = 0;
	unsigned char buf[4096];

	if(comnum)
	{
		if(comnumdetect)
		{
			for(i = 0; i <0xff;i++)
			{
				if(comportNullCnt[i]) closeComPort(i);
			}
#ifdef FULLVERSION
			IupSetAttribute(ih, "RUN", "NO");
#else
			if(strcmp(IupGetAttribute(matmculabel, "TITLE"), "Found")==0)
					openComPort(comnumdetect - 1, 115200);
			else
				IupSetAttribute(ih, "RUN", "NO");
#endif
			comnumdetect = 0;
		}
		if(!IupGetInt(IupGetDialogChild(ih,"CANVAS"), "on_update"))
		{
			n = getComPortPoll(comnum-1, buf, 4095);
			checkComportBuf(ih, comnum, n, buf, 0);
//			openCnt++;
//			if(openCnt > 4000) //1 minute
//			{
//				closeComPort(comnum-1);
//				openComPort(comnum-1, 115200);
//				openCnt=0;
//			}
		}
	}
	else if(strcmp(IupGetAttribute(matmculabel, "TITLE"), "Found") !=0)
	{
		char strTmp[4];
		for(i = 1; i < comportlistcnt; i++)
		{
			sprintf(strTmp,"%d", i+1);
			comnumdetect = IupGetInt(comportlist, strTmp);
			if(comnumdetect)
			{
				if(!comportNullCnt[comnumdetect - 1])
				{
					openComPort(comnumdetect - 1, 115200);
					comportNullCnt[comnumdetect - 1]++;
				}
				if(comportNullCnt[comnumdetect - 1])
				{
					n = getComPortPoll(comnumdetect - 1, buf, 4095);
					checkComportBuf(ih, comnumdetect, n, buf, 1);
					if(!n)
					{
						comportNullCnt[comnumdetect - 1]++;
						if(comportNullCnt[comnumdetect - 1] > 20)
						{
							closeComPort(comnumdetect - 1);
							comportNullCnt[comnumdetect - 1] = 0;
						}
					}
				}
				if(strcmp(IupGetAttribute(matmculabel, "TITLE"), "Found") !=0)
				{
					strStatusFlag++;
					if(strStatusFlag > 4) {IupSetAttribute(matmculabel, "TITLE", "Auto \\"); strStatusFlag = 0;}
					else if(strStatusFlag > 3) IupSetAttribute(matmculabel, "TITLE", "Auto |");
					else if(strStatusFlag > 2) IupSetAttribute(matmculabel, "TITLE", "Auto /");
					else IupSetStrf(matmculabel, "TITLE", "Auto %d --", comnumdetect);
				}
			}

		}

//		IupSetAttribute(IupGetDialogChild(ih, "COMPORTTIMERSEARCH"), "RUN", "YES");
	}
//	else if(detectcnt) //condition for auto detect are: detectcnt > 0 and matmculabel is NULL
//	{
//		if(detectedmatch > 3)
//		{
//			IupSetAttribute(matmculabel, "TITLE", "Found");
//			IupSetStrf(comportlist, "1","%d", comnumdetect + 1);
//
//			IupSetStrf(comportlist, "VALUESTRING","%d", comnumdetect + 1);
//			IupSetAttribute(comportbutton, "ACTIVE", "YES");
//			closeComPort(comnumdetect);
//			detectedmatch=0;
//			detectcnt = 0;
//		}
//		else
//		{
//			n = getComPortPoll(comnumdetect, buf, 4095);
//			detectcnt++;
//			if(detectcnt > 5)
//			{
//				detectcnt = 0;
//				closeComPort(comnumdetect);
//				if(comnumdetect < 255) comnumdetect++;
//				else comnumdetect = 0;
//				detectedmatch = 0;
//			}
//		}
//	}

	return IUP_DEFAULT;
}

int comport_valuechanged_cb(Ihandle *ih)
{
	Ihandle* comportsearch = IupGetDialogChild(ih, "COMPORTTIMERSEARCH");
	Ihandle* comportbtn = IupGetDialogChild(ih, "COMPORTBUTTON");
	IupSetAttribute(comportsearch, "RUN", "NO");
	IupSetAttribute(comportbtn, "ACTIVE", "YES");
	return IUP_DEFAULT;
}

int comport_dropped_cb(Ihandle *ih, int state)
{
	Ihandle* baudlist = IupGetDialogChild(ih, "COMPORTBAUDLIST");
	Ihandle* comporttimer = IupGetDialogChild(ih, "COMPORTTIMER");
	unsigned char i;

	int baud = IupGetInt(baudlist,"VALUESTRING");
	if(state)
	{
		IupSetAttribute(comporttimer, "RUN", "NO");
		IupSetAttribute(ih,"2", NULL);
		for(i = 0; i < 16; i++)
		{
			if(comPortisAvailable(i,baud))
			{
				IupSetStrf(ih, "APPENDITEM","%d", i + 1);
			}
		}
		IupSetAttribute(comporttimer, "RUN", "YES");
	}
	return IUP_DEFAULT;
}

int comport_connect_click_cb(Ihandle* item_comport, int button, int pressed, int x, int y, char* status)
{
	if(!pressed)
	{
		Ihandle* comportlist = IupGetDialogChild(item_comport, "COMPORTLIST");
		Ihandle* comporttimer = IupGetDialogChild(item_comport, "COMPORTTIMER");
		Ihandle* comporttimersearch = IupGetDialogChild(item_comport, "COMPORTTIMERSEARCH");
		Ihandle* baudlist = IupGetDialogChild(item_comport, "COMPORTBAUDLIST");

		int comnum =IupGetInt(comportlist, "VALUESTRING") - 1;
		int baudrate = IupGetInt(baudlist,"VALUESTRING");

		if(strncmp("Connect", IupGetAttribute(item_comport, "TITLE"), 7)==0)
		{
			if(openComPort(comnum, baudrate))
			{
				IupSetAttribute(item_comport, "TITLE", "Disconnect");
				IupSetAttribute(comportlist, "ACTIVE", "NO");
				IupSetAttribute(baudlist,  "ACTIVE", "NO");
				IupSetAttribute(IupGetDialogChild(item_comport, "COMPORTTXTLIST"), "ACTIVE", "YES");
				IupSetAttribute(IupGetDialogChild(item_comport, "COMPORTSENDBUTTON"), "ACTIVE", "YES");
				IupSetAttribute(IupGetDialogChild(item_comport, "COMPORTSENDSEQBUTTON"), "ACTIVE", "YES");
				IupSetAttribute(IupGetDialogChild(item_comport, "COMPORTREADFLASHBUTTON"), "ACTIVE", "YES");
				IupSetAttribute(IupGetDialogChild(item_comport, "COMPORTMONITORBUTTON"), "ACTIVE", "YES");
				IupSetInt(IupGetDialogChild(item_comport, "CANVAS"), "ComportConnected",1);
				if(!IupGetInt(comporttimer, "RUN")) IupSetAttribute(comporttimer, "RUN", "YES");
			}
			else
			{
				IupSetAttribute(item_comport,  "ACTIVE", "NO");
				IupSetAttribute(IupGetDialogChild(item_comport, "MATMCULABEL"),"TITLE","Searching...");
				IupSetAttribute(comportlist, "1", "0");
				IupSetAttribute(comportlist, "2", NULL);
				showMsgDlg(IupGetDialog(item_comport), "ERROR", "Couldn't Connect to Device", "Please Check Connection!");
//				if(IupGetInt(comporttimer, "RUN")) IupSetAttribute(comporttimer, "RUN", "NO");
				if(!IupGetInt(comporttimersearch, "RUN")) IupSetAttribute(comporttimersearch, "RUN", "YES");
			}
		}
		else if(strncmp("Disconnect", IupGetAttribute(item_comport, "TITLE"), 10)==0)
		{
			IupSetAttribute(item_comport, "TITLE", "Connect");
			IupSetAttribute(comporttimer, "RUN", "NO");
			closeComPort(comnum);
			IupSetAttribute(comportlist, "ACTIVE", "YES");
			IupSetAttribute(baudlist,  "ACTIVE", "YES");
			IupSetAttribute(IupGetDialogChild(item_comport, "COMPORTBUTTON"), "ACTIVE", "YES");
			IupSetAttribute(IupGetDialogChild(item_comport, "COMPORTTXTLIST"), "ACTIVE", "NO");
			IupSetAttribute(IupGetDialogChild(item_comport, "COMPORTSENDBUTTON"), "ACTIVE", "NO");
			IupSetAttribute(IupGetDialogChild(item_comport, "COMPORTSENDSEQBUTTON"), "ACTIVE", "NO");
			IupSetAttribute(IupGetDialogChild(item_comport, "COMPORTREADFLASHBUTTON"), "ACTIVE", "NO");
			IupSetAttribute(IupGetDialogChild(item_comport, "COMPORTMONITORBUTTON"), "ACTIVE", "NO");
			IupSetInt(IupGetDialogChild(item_comport, "CANVAS"), "ComportConnected",0);
		}
	}

	return IUP_DEFAULT;
}

int comport_monitor_click_cb(Ihandle* ih, int button, int pressed, int x, int y, char* status)
{
	Ihandle *comporttimer = IupGetDialogChild(ih, "COMPORTTIMER");
	Ihandle *seqlbl = IupGetDialogChild(ih, "SEQLABEL");
	Ihandle *seqlblcur = IupGetDialogChild(ih, "SEQLABELCUR");

	if(!pressed)
	{
		if(strcmp("Monitor", IupGetAttribute(ih, "TITLE")) == 0)
		{
			//save current setting layout, speed factor, and machine number
			save_seq_step(ih, IupGetInt(seqlblcur, "TITLE"));
			IupSetInt(comporttimer, "speed_factor_cur", IupGetInt(IupGetDialogChild(ih,"SPEEDFACTORTEXT"), "VALUE"));
			IupSetInt(comporttimer, "layout_num_cur", IupGetInt(IupGetDialogChild(ih,"CANVAS"), "layout_num"));
			IupSetInt(comporttimer, "machine_no_cur", IupGetInt(IupGetDialogChild(ih,"MACHNOTEXT"), "VALUE"));
			IupSetInt(comporttimer, "seq_val_cur", IupGetInt(seqlblcur, "TITLE"));
			IupSetInt(comporttimer, "seq_val_max", IupGetInt(seqlbl, "TITLE"));

			//disable control related with playback
			disableSeqControl(ih);
			disableComportSeqControl(ih);

			IupSetAttribute(ih, "TITLE", "Stop");
			IupSetAttribute(ih, "ACTIVE", "YES");

			//start comport mode and MAT
			IupSetInt(comporttimer, "COMPORTMODE", (int)COMPORTMONITOR);
//			sendComPortPuts(comnum - 1, "*353700FD\r");
		}
		else if (strcmp("Stop", IupGetAttribute(ih, "TITLE")) == 0)
		{
			//stop mat comport mode
			IupSetInt(comporttimer, "COMPORTMODE", 0);
			IupSetAttribute(ih, "TITLE", "Monitor");

			//restore current setting layout, speed factor, and machine number
			IupSetInt(IupGetDialogChild(ih,"SPEEDFACTORTEXT"), "VALUE", IupGetInt(comporttimer, "speed_factor_cur"));
			IupSetInt(IupGetDialogChild(ih,"SPEEDFACTORTEXTSHOW"), "VALUE", IupGetInt(comporttimer, "speed_factor_cur") * 100 / 128);
			IupSetInt(IupGetDialogChild(ih,"CANVAS"), "layout_num", IupGetInt(comporttimer, "layout_num_cur"));
			IupSetInt(IupGetDialogChild(ih,"LAYLABEL"), "TITLE", IupGetInt(comporttimer, "layout_num_cur"));
			IupSetInt(IupGetDialogChild(ih,"MACHNOTEXT"), "VALUE", IupGetInt(comporttimer, "machine_no_cur"));

			//restore sequence val
			IupSetInt(seqlbl, "TITLE", IupGetInt(comporttimer, "seq_val_max"));
			IupSetInt(seqlblcur, "TITLE", IupGetInt(comporttimer, "seq_val_cur"));
			load_seq_step(ih,  IupGetInt(comporttimer, "seq_val_cur"));

			//enable control
			enableSeqControl(ih);
			enableComportSeqControl(ih);

			//update canvas
			load_seq_step(ih, IupGetInt(seqlblcur, "TITLE"));
			layout_update(ih, IupGetInt(comporttimer, "layout_num_cur"));
		}
	}
	return IUP_DEFAULT;
}

static int progressBar_cancel_cb(Ihandle* ih)
{
  Ihandle *parentDlg = IupGetAttributeHandle(ih, "PARENTDIALOG");
  Ihandle *comporttimer = IupGetDialogChild(parentDlg, "COMPORTTIMER");
  int ret = IupAlarm("Warning!", "Interrupt Processing?", "Yes", "No", NULL);
  if (ret == 1) /* Yes Interrupt */
  {
	if(IupGetInt(comporttimer, "COMPORTMODE"))
	{
			IupSetInt(comporttimer, "COMPORTMODE", 0);
			IupHide(ih);
			IupSetAttribute(parentDlg, "ACTIVE", "YES");
	}
    return IUP_DEFAULT;
  }

  return IUP_CONTINUE;
}

void initProgBarDlg(Ihandle* parent_dialog, char *_title)
{
	Ihandle *progressBarDlg;
	progressBarDlg = IupProgressDlg();
	IupSetHandle("PROGRESSBARDIALOG", progressBarDlg);
	IupSetAttribute(progressBarDlg, "TITLE", _title);
	IupSetAttribute(progressBarDlg, "COUNT", 0);
	IupSetCallback(progressBarDlg, "CANCEL_CB", progressBar_cancel_cb);
	IupSetAttributeHandle(progressBarDlg, "PARENTDIALOG", parent_dialog);
	IupSetAttribute(parent_dialog, "ACTIVE", "NO");
	IupShow(progressBarDlg);
}

int comport_sendseq_click_cb(Ihandle* ih, int button, int pressed, int x, int y, char* status)
{
	Ihandle *comportlist = IupGetDialogChild(ih, "COMPORTLIST");
	Ihandle *comporttimer = IupGetDialogChild(ih, "COMPORTTIMER");
	int comnum = IupGetInt(comportlist, "VALUESTRING");

	Ihandle* seqlblcur = IupGetDialogChild(ih, "SEQLABELCUR");
	save_seq_step(ih, IupGetInt(seqlblcur, "TITLE"));

	if(!pressed)
	{
		comportEnableRTS(comnum - 1);
		char *comseqtmpfilename = getTmpFileName();
		remove(comseqtmpfilename);
		save_file_seq(ih, comseqtmpfilename);

		//initiate progress bar dialog
		initProgBarDlg(IupGetDialog(ih), "Sequence Transfer Progress");
		IupSetInt(IupGetHandle("PROGRESSBARDIALOG"), "TOTALCOUNT", 5 + IupGetInt(IupGetDialogChild(ih, "SEQLABEL"), "TITLE"));

		IupSetInt(comporttimer, "COMPORTMODE", (int)COMPORTSENDSEQ);
		sendComPortPuts(comnum - 1, getFileStrLine(comseqtmpfilename, 0));
		sendComPortByte(comnum - 1, 13);

		free(comseqtmpfilename);
	}
	return IUP_DEFAULT;
}

int comport_read_click_cb(Ihandle* ih, int button, int pressed, int x, int y, char* status)
{
	Ihandle *comportlist = IupGetDialogChild(ih, "COMPORTLIST");
	int comnum = IupGetInt(comportlist, "VALUESTRING");

	if(!pressed)
	{
		if(strncmp("Read", IupGetAttribute(ih, "TITLE"), 4)==0)
		{
			comportEnableRTS(comnum - 1);
			IupSetAttribute(ih, "TITLE", "Cancel");
			remove_seq_step_files(IupGetInt(IupGetDialogChild(ih, "SEQLABEL"), "TITLE") + 1);
			//disable control related with playback
			disableSeqControl(ih);
			disableComportSeqControl(ih);
			IupSetAttribute(ih, "ACTIVE", "YES");
			IupSetInt(IupGetDialogChild(ih, "COMPORTTIMER"), "COMPORTMODE", (int)COMPORTREADFLASH);
			sendComPortPuts(comnum - 1, "?34448F\r");
		}
		else if(strncmp("Cancel", IupGetAttribute(ih, "TITLE"), 6)==0)
		{
			comportDisableRTS(comnum - 1);
			IupSetInt(IupGetDialogChild(ih, "COMPORTTIMER"), "COMPORTMODE", 0);
			//enable control related with playback
			enableSeqControl(ih);
			enableComportSeqControl(ih);
			IupSetAttribute(ih, "TITLE", "Read");
		}
	}
	return IUP_DEFAULT;
}

int dwellup_action_cb(Ihandle *ih)
{
	Ihandle* dwelltxt = IupGetDialogChild(ih, "DWELLTEXT");
	setDwellVal(IupGetDialog(ih), IupGetInt(dwelltxt,"VALUE") + 10);

	return IUP_DEFAULT;
}
int dwelldown_action_cb(Ihandle *ih)
{
	Ihandle* dwelltxt = IupGetDialogChild(ih, "DWELLTEXT");
	setDwellVal(IupGetDialog(ih), IupGetInt(dwelltxt,"VALUE") - 10);

	return IUP_DEFAULT;
}

int dwellspin_action_cb(Ihandle *ih, int inc)
{
	Ihandle* dwelltxt = IupGetDialogChild(ih, "DWELLTEXT");
	setDwellVal(IupGetDialog(ih), IupGetInt(dwelltxt,"VALUE") + inc);
	return IUP_DEFAULT;
}

int machno_valuechanged_cb(Ihandle *ih, int c)
{
	Ihandle* machnotxt = IupGetDialogChild(ih, "MACHNOTEXT");
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
	int machnotxt_val = IupGetInt(machnotxt, "VALUE");
	static int machnotxt_val_old = 0xff;
	if(c == K_CR)
	{
		if(machnotxt_val > 0xff)
		{
			IupSetInt(machnotxt, "VALUE", machnotxt_val_old);
		}
		else
		{
			machnotxt_val_old = IupGetDouble(machnotxt, "VALUE");
		}
		IupSetFocus(canvas);
		if(IupGetInt(canvas, "properties_show"))IupUpdate(canvas);
	}
	if((c > K_9) && (c < K_ESC)) return IUP_IGNORE;
	return IUP_DEFAULT;
}

int speedfactorspin_action_cb(Ihandle *ih, int inc)
{
	Ihandle* speedfacttxtshow = IupGetDialogChild(ih, "SPEEDFACTORTEXTSHOW");
	setSpeedFactorVal(IupGetDialog(ih), IupGetInt(speedfacttxtshow,"VALUE") + inc);
	return IUP_DEFAULT;
}

int speedup_action_cb(Ihandle *ih)
{
	return speedfactorspin_action_cb(ih, -1);
}

int speeddown_action_cb(Ihandle *ih)
{
	return speedfactorspin_action_cb(ih, +1);
}

int speedfactorshow_valuechanged_cb(Ihandle *ih, int c)
{
	if(c == K_CR)
	{
		setSpeedFactorVal(IupGetDialog(ih), IupGetInt(ih,"VALUE"));
		IupSetFocus(IupGetDialogChild(ih, "CANVAS"));
	}
	if((c > K_9) && (c < K_ESC)) return IUP_IGNORE;

	return IUP_DEFAULT;
}

int speedfactor_valuechanged_cb(Ihandle *ih, int c)
{
	Ihandle* speedfactxtshow = IupGetDialogChild(ih, "SPEEDFACTORTEXTSHOW");
	if(c == K_CR)
	{
		setSpeedFactorVal(IupGetDialog(ih), IupGetInt(speedfactxtshow,"VALUE"));
		IupSetFocus(IupGetDialogChild(ih, "CANVAS"));
	}
	if((c > K_9) && (c < K_ESC)) return IUP_IGNORE;

	return IUP_DEFAULT;
}

int dwell_valuechanged_cb(Ihandle *ih, int c)
{
	Ihandle* dwelltxt = IupGetDialogChild(ih, "DWELLTEXT");
	if(c == K_CR)
	{
		setDwellVal(IupGetDialog(ih), IupGetInt(dwelltxt,"VALUE"));
		IupSetFocus(IupGetDialogChild(ih, "CANVAS"));
	}
	if((c > K_9) && (c < K_ESC)) return IUP_IGNORE;

	return IUP_DEFAULT;
}

int layoutspin_up_cb(Ihandle *ih)
{
	Ihandle* laylbl = IupGetDialogChild(ih, "LAYLABEL");
	Ihandle *canvas = IupGetDialogChild(ih, "CANVAS");
	Ihandle *seqtimer = IupGetDialogChild(ih, "SEQTIMER");

	int laytxt_val = IupGetInt(laylbl, "TITLE");
	if(!IupGetInt(canvas, "playmode"))
		if((IupGetInt(seqtimer,"RUN") == 0))
		{
			laytxt_val++;

			if(laytxt_val > 10) laytxt_val = 1;
			else if(laytxt_val < 1) laytxt_val = 10;

			layout_update(ih, laytxt_val);
		}
	return IUP_DEFAULT;
}

int layoutspin_down_cb(Ihandle *ih)
{
	Ihandle* laylbl = IupGetDialogChild(ih, "LAYLABEL");
	Ihandle *canvas = IupGetDialogChild(ih, "CANVAS");
	Ihandle *seqtimer = IupGetDialogChild(ih, "SEQTIMER");

	int laytxt_val = IupGetInt(laylbl, "TITLE");

	if(!IupGetInt(canvas, "playmode"))
		if((IupGetInt(seqtimer,"RUN") == 0))
		{
			laytxt_val--;

			if(laytxt_val > 10) laytxt_val = 1;
			else if(laytxt_val < 1) laytxt_val = 10;

			layout_update(ih, laytxt_val);
		}
	return IUP_DEFAULT;
}

int layoutspin_action_cb(Ihandle *ih, int inc)
{
	seq_update(ih, 1);
	if(inc > 0) return layoutspin_up_cb(ih);
	else if(inc < 0) return layoutspin_down_cb(ih);
	return IUP_DEFAULT;
}

int sceneadd_action_cb(Ihandle *ih)
{
	Ihandle* seqobj = IupGetDialogChild(ih, "SEQVAL");
	Ihandle* seqlbl = IupGetDialogChild(ih, "SEQLABEL");
	Ihandle* seqlblcur = IupGetDialogChild(ih, "SEQLABELCUR");
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");

	unsigned int seq_val_cur = IupGetInt(seqlblcur, "TITLE"); //current scene number
	unsigned int seq_val_max = IupGetInt(seqlbl, "TITLE"); //max scene number

	if(seq_val_max < MAXSCENENUM)
	{
		if(seq_val_cur < seq_val_max) rename_seq_files_up(seq_val_cur + 1, seq_val_max);

		save_seq_step(ih, seq_val_cur+1); // save current scene to inserted scene

		IupSetInt(seqlbl,"TITLE", seq_val_max+1);
		IupSetInt(seqobj,"MAX", seq_val_max);
		IupSetInt(seqobj,"SHOWTICKS", seq_val_max+1);
		IupSetInt(seqobj,"VALUE", seq_val_cur);

		IupUpdate(canvas);
	}
	return IUP_DEFAULT;
}

int scenerm_action_cb(Ihandle *ih)
{
	int i = 0;
	Ihandle* seqobj = IupGetDialogChild(ih, "SEQVAL");
	Ihandle* seqlbl = IupGetDialogChild(ih, "SEQLABEL");
	Ihandle* seqtxt = IupGetDialogChild(ih, "SEQTEXT");
	Ihandle* seqlblcur = IupGetDialogChild(ih, "SEQLABELCUR");
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");

	unsigned int seq_val_cur = IupGetInt(seqlblcur, "TITLE"); //current scene number
	unsigned int seq_val_max = IupGetInt(seqlbl, "TITLE"); //max scene number

	remove_seq_file(seq_val_cur); //remove current scene file

	if(seq_val_max > 1)
	{
		IupSetInt(seqlbl,"TITLE", seq_val_max - 1);
		IupSetInt(seqobj,"MAX", seq_val_max - 2);
		IupSetInt(seqobj,"SHOWTICKS", seq_val_max - 1);
	}

	if(seq_val_cur < seq_val_max)
	{
		//rename other scene file to lower
		rename_seq_files_down(seq_val_cur + 1, seq_val_max);
		load_seq_step(ih, seq_val_cur);
		IupSetInt(seqtxt,"VALUE", seq_val_cur);
		IupSetInt(seqobj,"VALUE", seq_val_cur-1);
		IupSetInt(seqlblcur,"TITLE", seq_val_cur);
	}
	else
	{
		if(seq_val_cur > 1)
		{
			load_seq_step(ih, seq_val_cur - 1); // goto lower scene
			IupSetInt(seqtxt,"VALUE", seq_val_cur - 1);
			IupSetInt(seqobj,"VALUE", seq_val_cur-2);
			IupSetInt(seqlblcur,"TITLE", seq_val_cur - 1);
		}
		else
		{
			char *bptr = IupGetAttribute(canvas, "block_pointer");
			for (i = 0; i<64;i++)
				bptr[i] = 1;
		}
	}
	IupUpdate(canvas);
	return IUP_DEFAULT;
}

int seqrewind_action_cb(Ihandle *ih)
{
	Ihandle* seqobj = IupGetDialogChild(ih, "SEQLABELCUR");
	unsigned int seq_val = IupGetInt(seqobj, "TITLE");

	if(seq_val > 1)seq_val--;

	seq_update(ih, seq_val);
	return IUP_DEFAULT;
}

int seqforward_action_cb(Ihandle *ih)
{
	Ihandle* seqobj = IupGetDialogChild(ih, "SEQLABELCUR");
	unsigned int seq_val = IupGetInt(seqobj, "TITLE");

	seq_val++;
	seq_update(ih, seq_val);
	return IUP_DEFAULT;
}

int seq_valuechanged_cb(Ihandle *ih)
{
	double seq_val = IupGetDouble(ih, "VALUE");
	IupSetDouble(ih, "VALUE", round(seq_val));

	//plus one because min seq_val is started with 0
	seq_update(ih, round(seq_val + 1));
	return IUP_DEFAULT;
}

int seqnum_action_cb(Ihandle *ih, int c)
{
	Ihandle* seqnumtxt = IupGetDialogChild(ih, "SEQNUMTEXT");
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
	unsigned int seqnumtxt_val = IupGetInt(seqnumtxt, "VALUE");

	if(c == K_CR)
	{
		if(seqnumtxt_val < 1) IupSetInt(seqnumtxt, "VALUE", 1);
		else
		{
				if(seqnumtxt_val > 0xff) IupSetInt(seqnumtxt, "VALUE", 0xff);
				else
					IupSetInt(seqnumtxt, "VALUE", seqnumtxt_val);
		}
		IupSetFocus(canvas);
		if(IupGetInt(canvas, "properties_show")) IupUpdate(canvas);
	}
	if((c > K_9) && (c < K_ESC)) return IUP_IGNORE;

	return IUP_DEFAULT;
}

int seqjump_action_cb(Ihandle *ih, int c)
{
	Ihandle* seqtxtobj = IupGetDialogChild(ih, "SEQTEXT");
	Ihandle* seqlblmax = IupGetDialogChild(ih, "SEQLABEL");
	Ihandle* seqlblcur = IupGetDialogChild(ih, "SEQLABELCUR");

	unsigned int seq_val_max = IupGetInt(seqlblmax, "TITLE");
	unsigned int seq_val_new = IupGetInt(seqtxtobj, "VALUE");
	unsigned int seq_val_cur = IupGetInt(seqlblcur, "TITLE");

	if(c == K_CR)
	{
		if(seq_val_new > 0)
		{
			IupSetInt(seqtxtobj, "VALUE", seq_val_cur);
			if(seq_val_new <= seq_val_max)
				seq_update(ih, seq_val_new);
		}
		IupSetFocus(IupGetDialogChild(ih, "CANVAS"));
	}
	if((c > K_9) && (c < K_ESC)) return IUP_IGNORE;

	return IUP_DEFAULT;
}

int seqtimer_action_cb(Ihandle *ih)
{
	static unsigned int timercnt = 0;
	Ihandle* seqlbl = IupGetDialogChild(ih, "SEQLABEL");
	Ihandle* seqlblcur = IupGetDialogChild(ih, "SEQLABELCUR");
	Ihandle* seqtimer = IupGetDialogChild(ih, "SEQTIMER");
	Ihandle* delaylbl = IupGetDialogChild(ih, "DELAYLABEL");
	Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
	Ihandle *comporttimer = IupGetDialogChild(ih, "COMPORTTIMER");

	unsigned int seq_val_cur = IupGetInt(seqlblcur, "TITLE");
	unsigned int seq_val_max = IupGetInt(seqlbl, "TITLE");
	double delay_val = IupGetDouble(delaylbl, "TITLE") / 14;

	if(!IupGetInt(comporttimer, "COMPORTMODE"))
	{
		timercnt++;
		if(timercnt > delay_val)
		{
			seq_val_cur++;
			if(seq_val_cur > seq_val_max)
			{
				if(IupGetInt(IupGetDialogChild(ih, "PLAYREPEATTGL"), "VALUE"))
					load_seq_step(ih, 1);
				else
				{
					IupSetAttribute(seqtimer,"RUN", "NO");
					enableSeqControl(ih);
					if(IupGetInt(canvas, "ComportConnected"))
					{
						enableComportControl(ih);
					}
				}
			}
			else
				load_seq_step(ih, seq_val_cur);

			if(IupGetInt(canvas, "ComportConnected"))
			{
				sendComportCanvasState(canvas);
			}

			timercnt = 0;
		}
	}

	return IUP_DEFAULT;
}

int seqplay_action_cb(Ihandle* ih, int state)
{
	Ihandle* seqtimer = IupGetDialogChild(ih, "SEQTIMER");
	Ihandle *comportlist = IupGetDialogChild(ih, "COMPORTLIST");
	int comnum = IupGetInt(comportlist, "VALUESTRING") - 1;
	zoom_update(ih, 0);
	if(state)
	{
		seq_update(ih, 1);

		IupSetAttribute(seqtimer, "RUN", "YES");
		disableSeqControl(ih);
		if(IupGetInt(IupGetDialogChild(ih, "CANVAS"), "ComportConnected"))
		{
			comportEnableRTS(comnum);
			disableComportControl(ih);
		}
		IupSetAttribute(ih, "IMAGE","IUP_Stop");
	}
	else
	{
		IupSetAttribute(seqtimer, "RUN", "NO");
		enableSeqControl(ih);
		if(IupGetInt(IupGetDialogChild(ih, "CANVAS"), "ComportConnected"))
		{
			comportDisableRTS(comnum);
			enableComportControl(ih);
		}
		IupSetAttribute(ih, "IMAGE","IUP_Play");
	}
	return IUP_DEFAULT;
}

int seqstop_action_cb(Ihandle *ih)
{
	Ihandle* seqtimer = IupGetDialogChild(ih, "SEQTIMER");

	IupSetAttribute(seqtimer, "RUN", "NO");
	enableSeqControl(ih);
	if(IupGetInt(IupGetDialogChild(ih, "CANVAS"), "ComportConnected"))
	{
		enableComportControl(ih);
	}
	return IUP_DEFAULT;
}

void toggle_bar_visibility(Ihandle* item, Ihandle* ih)
{
  if (IupGetInt(item, "VALUE"))
  {
    IupSetAttribute(ih, "FLOATING", "YES");
    IupSetAttribute(ih, "VISIBLE", "NO");
    IupSetAttribute(item, "VALUE", "OFF");
  }
  else
  {
    IupSetAttribute(ih, "FLOATING", "NO");
    IupSetAttribute(ih, "VISIBLE", "YES");
    IupSetAttribute(item, "VALUE", "ON");
  }

  IupRefresh(ih);  /* refresh the dialog layout */
}

int canvas_action_cb(Ihandle* canvas)
{
//  clock_t t;
//  t = clock();
  unsigned char i, j, k, bnum;
  int canvas_width, canvas_height;

  int ucnt, bcntw,bcnth;
  int uw, uh, ux, uy,bw,bh;
  int ox, oy;
  int imgcw, imgch, xb, yb;
//  unsigned int ri, gi, bi;

  static imImage* image, *img, *imgs, *imgcrop;
  char *bptr = IupGetAttribute(canvas, "block_pointer");
  char *bptr_old = IupGetAttribute(canvas, "block_pointer_old");

  cdCanvas* cd_canvas = (cdCanvas*)IupGetAttribute(canvas, "cdCanvas");
//  char strTmp[2];
//  Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");

//  const char* background = IupConfigGetVariableStrDef(config, "Canvas", "Background", "255 255 255");

  IupGetIntInt(canvas, "DRAWSIZE", &canvas_width, &canvas_height);

  cdCanvasActivate(cd_canvas);

//  /* draw the background */
//  sscanf(background, "%u %u %u", &ri, &gi, &bi);
  cdCanvasBackground(cd_canvas, cdEncodeColor(255, 255, 255));
//

  image = (imImage*)IupGetAttribute(canvas, "IMAGE");

  if (image)
  {
//	  printf("update call\n");
	  IupSetInt(canvas, "on_update", 1);
	  if(!IupGetInt(canvas, "playmode") || IupGetInt(canvas,"zoom_update") || IupGetInt(canvas,"resize_update"))
	  {
//		  Ihandle *seqobj = IupGetDialogChild(IupGetDialog(canvas), "SEQVAL");
		  cdCanvasClear(cd_canvas);
		  int view_width, view_height;
		  float posy = IupGetFloat(canvas, "POSY");
		  float posx = IupGetFloat(canvas, "POSX");

		  IupGetIntInt(canvas, "lwh", &view_width, &view_height);

		  if (canvas_width < view_width)
			ox = (int)floor(-posx*view_width);
		  else
			ox = (canvas_width - view_width) / 2;

		  if (canvas_height < view_height)
		  {
			/* posy is top-bottom, CD is bottom-top.
			   invert posy reference (YMAX-DY - POSY) */
			float dy = IupGetFloat(canvas, "DY");
			posy = 1.0f - dy - posy;
			oy = (int)floor(-posy*view_height);
		  }
		  else
			oy = (canvas_height - view_height) / 2;

		IupSetStrf(canvas, "loff", "%dx%d", ox, oy);

		//get total of unit
		ucnt = IupGetInt(canvas,"ucnt");

		//get total of block in unit
		IupGetIntInt(canvas, "bcwh", &bcntw, &bcnth);

		//draw grid
		for(i = 0; i<ucnt;i++)
		{
			setBlockImage(canvas,i+1,&imgs);
			setUnitImage(canvas,i+1,&img);
			setUnitParam(canvas, i+1, &ux, &uy, &uw, &uh, &bw, &bh);
			drawUnitGridImage(cd_canvas, img, imgs, i+1, getUnitimFlag(canvas,i+1), ucnt, bcntw, bcnth, ux, uy, uw, uh, bw, bh, bptr);
//			if(!IupGetInt(canvas, "playmode"))
//			{
//				if(!IupGetInt(seqobj, "VALUE"))
//				{
//					cdCanvasForeground(cd_canvas, cdEncodeColor(0xff,0xff,0xff));
//					if(uw>uh)
//						cdCanvasFont(cd_canvas, "Times", 0, uh /3 );
//					else
//						cdCanvasFont(cd_canvas, "Times", 0, uw /3 );
//					sprintf(strTmp, "%c", 65+i);
//					cdCanvasText(cd_canvas, ux+uw/3,uy+uh/3,strTmp);
//				}
//			}
		}
		memcpy(bptr_old, bptr, 64);
		IupSetAttribute(canvas, "block_pointer_old", bptr_old);
		if(IupGetInt(canvas,"zoom_update")) IupSetInt(canvas,"zoom_update", 0);
		if(IupGetInt(canvas,"resize_update")) IupSetInt(canvas,"resize_update", 0);

		Ihandle *canvaslayout_expander = IupGetDialogChild(canvas, "CANVASLAYOUTEXPANDER");
		Ihandle *canvaslayout = IupGetDialogChild(canvas, "CANVASLAYOUT");
//
		if(strncmp(IupGetAttribute(canvaslayout_expander, "STATE"), "OPEN",4)==0)
			IupUpdate(canvaslayout);
//		else
//		{
//			IupSetAttribute(canvaslayout_expander, "SIZE", "0x0");
//			IupSetAttribute(canvaslayout, "SIZE", "0x0");
//		}
	  }
	  else
	  {
//		  printf("update mode\n");
		  if(memcmp(bptr_old, bptr, 64))
		  {
			  ucnt = IupGetInt(canvas,"ucnt");
			  IupGetIntInt(canvas, "bcwh", &bcntw, &bcnth);
			  for(i = 0; i<ucnt;i++)
				{
//					setBlockImage(canvas,i+1,&imgs);
					setUnitImage(canvas,i+1,&img);
					setUnitParam(canvas, i+1, &ux, &uy, &uw, &uh, &bw, &bh);

					if(img)
					{
						for(j = 0; j < bcntw; j++)
						{
							for(k = 0; k < bcnth; k++)
							{
								getBlockNumCanvas(canvas, i + 1, j, k, &bnum);
								if(bptr_old[bnum] != bptr[bnum])
								{
									xb = ux + j*bw;
									yb = uy + k*bh;
									if(!bptr[bnum]) // block pointer calc: total_block (in 1 unit) *(unum - 1) + (4y) + x
									{
										imcdCanvasPutImage(cd_canvas, img, xb, yb, bw, bh, 0, 0, 0, 0);
									}
									else
									{
										imgcw = img->width/bcntw;
										imgch = img->height/bcnth;
										imgcrop = imImageCreate(imgcw,imgch,IM_RGB,IM_BYTE);

										imProcessCrop(img, imgcrop,j*imgcw,k*imgch);
										imcdCanvasPutImage(cd_canvas, imgcrop, xb, yb, bw, bh, 0, 0, 0, 0);
										if (imgcrop) imImageDestroy(imgcrop);
									}
									cdCanvasForeground(cd_canvas, CD_BLACK);
									cdCanvasRect(cd_canvas, xb, xb + bw, yb, yb + bh);
									bptr_old[bnum] = bptr[bnum];
								}
							}
						}
					}
				}
			  IupSetAttribute(canvas, "block_pointer_old", bptr_old);
		  }
	  }
	  drawPropertiesShow(canvas);
	  IupSetInt(canvas, "on_update", 0);
  }

  cdCanvasFlush(cd_canvas);
//  t = clock() - t;
//  printf ("canvas time %ld clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
  return IUP_DEFAULT;
}

void canvaslayoutDrawRect(cdCanvas* cd_canvas, int uw, int uh, int offx, int offy, const char *label, unsigned char rot90)
{
	cdCanvasFont(cd_canvas, "Times", 0, uh /3 );
	if(rot90)
	{
		cdCanvasRect(cd_canvas, offx, offx + uh, offy, offy + uw);
		cdCanvasText(cd_canvas, offx + (uh / 3), offy + (uw /3),label);
	}
	else
	{
		cdCanvasRect(cd_canvas, offx, offx + uw, offy, offy + uh);
		cdCanvasText(cd_canvas, offx + (uw / 3), offy + (uh /3),label);
	}
}

void canvaslayoutDrawUnit(cdCanvas* cd_canvas, int layindex, int posy, int clew, int cleh, int uw, int uh)
{
	int offx, offy, ox, oy;
	double lh;
	switch (layindex)
	{
		case 1:
			ox = (clew - (uw*2+uh)) / 2;
			oy = posy + cleh*(10 - layindex) + (cleh - (uh * 2)) /2;

			offx = ox; offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "A", 0);

			offx = ox + uw; offy = oy + uh;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "B", 0);

			offx = ox + uw * 2; offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "C", 1);

			offx = ox + uw;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "D", 0);
			break;
		case 2:
			ox = (clew - (uw*2+uh*2)) / 2;
			oy = posy + cleh*(10 - layindex) + (cleh - (uh/4+uw)) /2;

			offx = ox;
			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "A", 1);

			offx = ox + uh;
			offy = oy + (uw - uh*3/4);
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "B", 0);

			offx += uw;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "C", 0);

			offx += uw;offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "D", 1);
			break;
		case 3:
			lh = ((double)uh * 7.0 / 8.0) + (double)uw;

			ox = (clew - (uw+uh*2)) / 2;
			oy = posy + cleh*(10 - layindex) + (cleh - floor(lh)) /2;

			offx = ox;
			offy = oy + (uh * 7 / 8);
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "A", 1);

			offx += uh;
			offy = oy + uh;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "B", 0);

			offx += uw;
			offy = oy + (uh * 7 / 8);
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "C", 1);

			offx -= uw;
			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "D", 0);
			break;
		case 8:
		case 4:
			ox = (clew - (uw+uh)) / 2;
			oy = posy + cleh*(10 - layindex) + (cleh - (uw+uh)) /2;

			offx = ox;
			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "A", 0);

			offy = oy + uh;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "B", 1);

			offx += uh;
			offy = oy + uw;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "C", 0);

			offx = ox + uw;
			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "D", 1);
			break;
		case 5:
			ox = (clew - (uw*2)) / 2;
			oy = posy + cleh*(10 - layindex) + (cleh - (uw+uh)) /2;

			offx = ox;
			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "A", 0);

			offy = oy + uh;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "B", 0);

			offx = ox + uw;
			offy = oy + uw;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "C", 0);

			offx = ox + uw;
			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "D", 1);
			break;
		case 6:
			lh =((double)uh * 7.0 / 8.0) + uw;
			ox = (clew - (uw*2 + uh)) / 2;
			oy = posy + cleh*(10 - layindex) + (cleh - floor(lh)) /2;

			offx = ox;
			offy = oy + (uh * 7 / 8);
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "A", 0);

			offx = ox + uw;
			offy = oy + uh;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "B", 0);

			offx += uw;
			offy = oy + (uh * 7 / 8);
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "C", 1);

			offx -= uw;
			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "D", 0);
			break;
		case 7:
			ox = (clew - (uw*2)) / 2;
			oy = posy + cleh*(10 - layindex) + (cleh - (uw+uh)) /2;

			offx = ox;
			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "A", 0);

			offx = ox + (uw - uh);
			offy = oy + uh;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "B", 1);

			offx = ox + uw;
			offy = oy + uw;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "C", 0);

			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "D", 1);
			break;
		case 9:
			ox = (clew - ((uw * 6 / 5) + (uh * 2))) / 2;
			oy = posy + cleh*(10 - layindex) + (cleh - (uw+uh)) /2;

			offx = ox;
			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "A", 1);

			offx = ox + (uh*5 - uw * 2) / 5;
			offy = oy + uw;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "B", 0);

			offx = ox + (uh*5 + uw * 3) / 5;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "C", 0);

			offx = ox + (uh * 5 + uw *6) /5;
			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "D", 1);
			break;
		case 10:
			ox = (clew - (uw*2)) / 2;
			oy = posy + cleh*(10 - layindex) + (cleh - (uh*2)) /2;

			offx = ox;
			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "A", 0);

			offx = ox + uw / 2;
			offy = oy + uh;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "B", 0);

			offx = ox + uw;
			offy = oy;
			canvaslayoutDrawRect(cd_canvas, uw, uh, offx, offy, "C", 0);
			break;
		default: break;
	}
}

int canvaslayout_action_cb(Ihandle* canvaslayout)
{
	unsigned char i;
	Ihandle *dlg = IupGetDialog(canvaslayout);
	Ihandle* laylbl = IupGetDialogChild(dlg, "LAYLABEL");
	Ihandle *canvas = IupGetDialogChild(dlg, "CANVAS");

	cdCanvas* cd_canvas = (cdCanvas*)IupGetAttribute(canvaslayout, "cdCanvas");

	int clew,cleh, dlg_width, dlg_height, canvas_width, canvas_height;
	int uw, uh;
	int laynum_cur = IupGetInt(laylbl, "TITLE");
	char strTmp[3];
	int scrollbar_size = IupGetInt(NULL, "SCROLLBARSIZE");

	IupGetIntInt(dlg,"SIZE", &dlg_width, &dlg_height);
	IupGetIntInt(canvas,"SIZE", &canvas_width, &canvas_height);

	int clw, clh;
//	printf("action canvaslayout\n");
	IupSetStrf(canvaslayout, "SIZE", "%dx", dlg_width / 5 - scrollbar_size);
	IupGetIntInt(canvaslayout, "DRAWSIZE", &clw,&clh);
//	printf("dialog width = %d, height = %d\n", dlg_width, dlg_height);
	clew = clw - 1;
	cleh = clw * 3/4;

	float dy = (float) clh / (float)cleh / 10.0f;
	IupSetFloat(canvaslayout,"DX", 1.0f);
	IupSetFloat(canvaslayout,"DY", dy);

	uw = clew * 2 / 7;
	uh = uw * 3 /4;

	cdCanvasActivate(cd_canvas);
	cdCanvasBackground(cd_canvas, cdEncodeColor(0, 0, 0));

//	float posy = (float)((laynum_cur - 1) * cleh) / (float)cleh/10.0f;        // top
//	IupSetFloat(canvaslayout, "POSY", posy);

	float posy = 1.0f - dy - IupGetFloat(canvaslayout, "POSY");
	int y = (int)floor(-posy*(float)cleh*10.0f);

	cdCanvasForeground(cd_canvas, CD_WHITE);
	cdCanvasBox(cd_canvas, 0, clw, 0, clh);
	cdCanvasForeground(cd_canvas, CD_BLACK);

	int ymin, ymax;
	for(i = 0; i < 10;i++)
	{
		if(laynum_cur != (int)(i+1))
		{
			ymin = y + cleh*(9 - i);
			ymax = y + cleh*(10 - i);
			cdCanvasRect(cd_canvas, 0, clew, ymin, ymax - 1);
			canvaslayoutDrawUnit(cd_canvas, (int)i + 1, y, clew, cleh, uw, uh);
			sprintf(strTmp, "%d", i + 1);
			cdCanvasFont(cd_canvas, "Times", 0, cleh /6 );
			cdCanvasText(cd_canvas, clew/20, ymin + cleh - (cleh / 5),strTmp);
		}
	}

	ymin = y + cleh*(10 - laynum_cur);
	ymax = y + cleh*(11 - laynum_cur);
	cdCanvasForeground(cd_canvas, cdEncodeColor(128,128,128));
	cdCanvasBox(cd_canvas,  0, clew, ymin, ymax - 1);
	cdCanvasForeground(cd_canvas, CD_WHITE);
	cdCanvasRect(cd_canvas,  0, clew, ymin, ymax - 1);
	canvaslayoutDrawUnit(cd_canvas, (int)laynum_cur, y, clew, cleh, uw, uh);
	sprintf(strTmp, "%d", laynum_cur);
	cdCanvasFont(cd_canvas, "Times", 0, cleh /6 );
	cdCanvasText(cd_canvas, clew/20, ymin + cleh - (cleh / 5),strTmp);
	cdCanvasFlush(cd_canvas);
	return IUP_DEFAULT;
}

int canvaslayout_openclose_cb(Ihandle* ih, int state)
{
	Ihandle *dlg = IupGetDialog(ih);
	Ihandle *canvas = IupGetDialogChild(dlg, "CANVAS");

	int dlg_width, dlg_height, canvas_width, canvas_height;

	IupGetIntInt(dlg,"SIZE", &dlg_width, &dlg_height);
	IupGetIntInt(canvas,"SIZE", &canvas_width, &canvas_height);

	if(state) //open
	{
		 IupSetStrf(ih, "SIZE", "%dx", dlg_width / 5);
		 canvaslayout_action_cb( IupGetDialogChild(dlg, "CANVASLAYOUT"));
	}
	else
		 IupSetStrf(ih, "SIZE", "%dx", 0);

	return IUP_DEFAULT;
}

int canvas_motion_cb(Ihandle* canvas, int x, int y, char *status)
{
	Ihandle* seqtimer = IupGetDialogChild(canvas, "SEQTIMER");
	unsigned char unum;
	static unsigned char unum_old = 0;
	int xn, yn;
	static unsigned char xn_old = 0;
	static unsigned char yn_old = 0;
	unsigned char bnum;
	int scrollbar_size = IupGetInt(NULL,"SCROLLBARSIZE");
	if(!IupGetInt(canvas, "playmode"))
		if(IupGetInt(seqtimer,"RUN") == 0)
		{
			if(!IupGetInt(canvas,"open_update"))
			{
				if (iup_isbutton1(status)) /* button1 is pressed */
				{
					getUnitPixel(canvas, x, y, &unum, &xn, &yn);
					if((xn_old != xn) || (yn_old !=yn) || (unum_old != unum))
					{
						if(unum > 0)
						{
							if(IupGetInt(canvas, "properties_show"))
								if(x < (scrollbar_size + 220))
									if(y < scrollbar_size + 200) return IUP_DEFAULT;
							if(iup_isshift(status))
							{
								Ihandle* parentDlg = IupGetDialog(canvas);
								Ihandle* seqlblcur = IupGetDialogChild(parentDlg, "SEQLABELCUR");
								unsigned int seq_val_cur = IupGetInt(seqlblcur, "TITLE");
								save_seq_step(parentDlg, seq_val_cur);
							}

							getBlockNumCanvas(canvas, unum, xn, yn, &bnum);
							drawUnitBlock(canvas, unum, xn, yn, bnum, 0);
							xn_old = xn;
							yn_old = yn;
							unum_old=unum;
							//auto advance to next scene if shift is also pressed
							if(iup_isshift(status))
							{
								Ihandle* parentDlg = IupGetDialog(canvas);
								Ihandle* seqobj = IupGetDialogChild(parentDlg, "SEQVAL");
								Ihandle* seqlbl = IupGetDialogChild(parentDlg, "SEQLABEL");
								Ihandle* seqlblcur = IupGetDialogChild(parentDlg, "SEQLABELCUR");

								unsigned int seq_val_cur = IupGetInt(seqlblcur, "TITLE"); //current scene number
								unsigned int seq_val_max = IupGetInt(seqlbl, "TITLE"); //max scene number
								if(seq_val_cur < MAXSCENENUM)
								{
									if(seq_val_max < MAXSCENENUM)
										if(seq_val_cur < seq_val_max) rename_seq_files_up(seq_val_cur + 1, seq_val_max);

									save_seq_step(parentDlg, seq_val_cur+1); // save current scene to inserted scene

									if(seq_val_max < MAXSCENENUM)
									{
										IupSetInt(seqlbl,"TITLE", seq_val_max+1);
										IupSetInt(seqobj,"MAX", seq_val_max);
										IupSetInt(seqobj,"SHOWTICKS", seq_val_max+1);
									}

									IupSetInt(seqobj,"VALUE", seq_val_cur);
									IupSetInt(seqlblcur,"TITLE", seq_val_cur+1);
								}
							}
//							if(IupGetInt(canvas, "ComportConnected"))
//							{
//								int comnum = IupGetInt(IupGetDialogChild(canvas, "COMPORTLIST"), "VALUESTRING");
//								comportEnableRTS(comnum - 1);
//								sendComportCanvasState(canvas);
//								comportDisableRTS(comnum - 1);
//							}
						}
					}
					IupSetInt(canvas,"LEFT_MOVE", 1);
				}
				if (iup_isbutton3(status)) /* right click is pressed */
				{
					getUnitPixel(canvas, x, y, &unum, &xn, &yn);
					if((xn_old != xn) || (yn_old !=yn) || (unum_old != unum))
					if(unum > 0)
					{
						if(IupGetInt(canvas, "properties_show"))
							if(x < (scrollbar_size + 220))
								if(y < scrollbar_size + 200) return IUP_DEFAULT;
						if(iup_isshift(status))
						{
							Ihandle* parentDlg = IupGetDialog(canvas);
							Ihandle* seqlblcur = IupGetDialogChild(parentDlg, "SEQLABELCUR");
							unsigned int seq_val_cur = IupGetInt(seqlblcur, "TITLE");
							save_seq_step(parentDlg, seq_val_cur);
						}

						getBlockNumCanvas(canvas, unum, xn, yn, &bnum);
						drawUnitBlock(canvas, unum, xn, yn, bnum, 1);
						xn_old = xn;
						yn_old = yn;
						unum_old=unum;
						//auto advance to next scene if shift is also pressed
						if(iup_isshift(status))
						{
							Ihandle* parentDlg = IupGetDialog(canvas);
							Ihandle* seqobj = IupGetDialogChild(parentDlg, "SEQVAL");
							Ihandle* seqlbl = IupGetDialogChild(parentDlg, "SEQLABEL");
							Ihandle* seqlblcur = IupGetDialogChild(parentDlg, "SEQLABELCUR");

							unsigned int seq_val_cur = IupGetInt(seqlblcur, "TITLE"); //current scene number
							unsigned int seq_val_max = IupGetInt(seqlbl, "TITLE"); //max scene number

							if(seq_val_cur < MAXSCENENUM)
							{
								if(seq_val_max < MAXSCENENUM)
									if(seq_val_cur < seq_val_max) rename_seq_files_up(seq_val_cur + 1, seq_val_max);

								save_seq_step(parentDlg, seq_val_cur+1); // save current scene to inserted scene

								if(seq_val_max < MAXSCENENUM)
								{
									IupSetInt(seqlbl,"TITLE", seq_val_max+1);
									IupSetInt(seqobj,"MAX", seq_val_max);
									IupSetInt(seqobj,"SHOWTICKS", seq_val_max+1);
								}

								IupSetInt(seqobj,"VALUE", seq_val_cur);
								IupSetInt(seqlblcur,"TITLE", seq_val_cur+1);
							}
						}
//						if(IupGetInt(canvas, "ComportConnected"))
//						{
//							int comnum = IupGetInt(IupGetDialogChild(canvas, "COMPORTLIST"), "VALUESTRING");
//							comportEnableRTS(comnum - 1);
//							sendComportCanvasState(canvas);
//							comportDisableRTS(comnum - 1);
//						}
					}
					IupSetInt(canvas,"RIGHT_MOVE", 1);
				}
			}
		}
	return IUP_DEFAULT;
}

int canvaslayout_click_cb(Ihandle* canvaslayout, int button, int pressed, int x, int y, char* status)
{
	int clw, clh, cleh, cly, laytxt_val;
	float posy;
	Ihandle *dlg = IupGetDialog(canvaslayout);
	Ihandle* seqtimer = IupGetDialogChild(dlg, "SEQTIMER");
	Ihandle* canvas = IupGetDialogChild(dlg, "CANVAS");
	Ihandle* laylbl = IupGetDialogChild(dlg, "LAYLABEL");

	if(!IupGetInt(canvas, "playmode"))
		if((IupGetInt(seqtimer,"RUN") == 0))
		{
			if(button == IUP_BUTTON1) //left click
			{
				if(!pressed)
				{
					IupGetIntInt(canvaslayout, "DRAWSIZE", &clw,&clh);
					cleh = clw * 3/4;

					posy = IupGetFloat(canvaslayout, "POSY");
					cly = (int)floor(posy*(float)cleh*10.0f);
					laytxt_val = (y + cly) / cleh + 1;
					IupSetInt(laylbl, "TITLE", laytxt_val);
					IupSetInt(canvas, "layout_num", laytxt_val);

					setUnitLayoutImages(canvas);
					setUnitLayoutSize(canvas);
					IupSetInt(canvas,"resize_update",1);
					IupUpdate(canvas);
					IupUpdate(canvaslayout);
				}
			}
		}

	return IUP_DEFAULT;
}

int canvas_click_cb(Ihandle* canvas, int button, int pressed, int x, int y, char* status)
{
	int scrollbar_size = IupGetInt(NULL,"SCROLLBARSIZE");

#ifdef FULLVERSION
	Ihandle* seqtimer = IupGetDialogChild(canvas, "SEQTIMER");
	unsigned char unum;
	int xn, yn;
	unsigned char bnum;
#endif

	if(button == IUP_BUTTON1) //left click
	{
		if(!pressed)
		{
			if(IupGetInt(canvas, "properties_show"))
			{
				if(x < (scrollbar_size + 220))
					if(y < scrollbar_size + 200)
					{
						if(y < (scrollbar_size + 12))
						{
							if(x > (scrollbar_size + 210))
								if(y > scrollbar_size)
								{
									IupSetInt(canvas, "properties_show", 0);
									if(IupGetInt(canvas, "playmode"))
										IupSetInt(canvas, "zoom_update", 1);
									IupUpdate(canvas);
									return IUP_DEFAULT;
								}
						}
						return IUP_DEFAULT;
					}
			}
			else
			{
				if(x > 0)
					if(x < 8)
						if(y < (scrollbar_size + 12))
							if(y > scrollbar_size)
							{
								IupSetInt(canvas, "properties_show", 1);
								if(IupGetInt(canvas, "playmode"))
									IupSetInt(canvas, "zoom_update", 1);
								IupUpdate(canvas);
								return IUP_DEFAULT;
							}
			}
		}
	}

#ifdef FULLVERSION
	if(!IupGetInt(canvas, "playmode"))
		if((IupGetInt(seqtimer,"RUN") == 0))
		{
			if(button == IUP_BUTTON1) //left click
			{
				if(!pressed)
				{
					if(!IupGetInt(canvas,"LEFT_MOVE"))
					{
						if(!IupGetInt(canvas,"open_update"))
						{
							getUnitPixel(canvas, x, y, &unum, &xn, &yn);
							if(unum > 0)
							{
								getBlockNumCanvas(canvas, unum, xn, yn, &bnum);
								drawUnitBlock(canvas, unum, xn, yn, bnum, 2);
//								if(IupGetInt(canvas, "ComportConnected"))
//								{
//									int comnum = IupGetInt(IupGetDialogChild(canvas, "COMPORTLIST"), "VALUESTRING");
//									comportEnableRTS(comnum - 1);
//									sendComportCanvasState(canvas);
//									comportDisableRTS(comnum - 1);
//								}
							}
						}
						else
							IupSetInt(canvas,"open_update",0);
					}
					else
						IupSetInt(canvas, "LEFT_MOVE", 0);
				}
			}
			if(button == IUP_BUTTON3) //right click
			{
				if(!pressed)
				{
					if(!IupGetInt(canvas,"RIGHT_MOVE"))
					{
						getUnitPixel(canvas, x, y, &unum, &xn, &yn);
						if(unum > 0)
						{
							if(IupGetInt(canvas, "properties_show"))
								if(x < (scrollbar_size + 220))
									if(y < scrollbar_size + 200) return IUP_DEFAULT;

							clearUnit(canvas, unum);
//							if(IupGetInt(canvas, "ComportConnected"))
//							{
//								int comnum = IupGetInt(IupGetDialogChild(canvas, "COMPORTLIST"), "VALUESTRING");
//								comportEnableRTS(comnum - 1);
//								sendComportCanvasState(canvas);
//								comportDisableRTS(comnum - 1);
//							}
						}
					}
					else
						IupSetInt(canvas, "RIGHT_MOVE", 0);
				}
			}
		}
#endif
	return IUP_DEFAULT;
}

int canvaslayout_map_cb(Ihandle* canvaslayout)
{
  cdCanvas* cd_canvas = cdCreateCanvas(CD_IUPDBUFFER, canvaslayout);
  IupSetAttribute(canvaslayout, "cdCanvas", (char*)cd_canvas);
  return IUP_DEFAULT;
}

int canvaslayout_unmap_cb(Ihandle* canvaslayout)
{
  cdCanvas* cd_canvas = (cdCanvas*)IupGetAttribute(canvaslayout, "cdCanvas");
  cdKillCanvas(cd_canvas);
  return IUP_DEFAULT;
}

int canvas_map_cb(Ihandle* canvas)
{
  cdCanvas* cd_canvas = cdCreateCanvas(CD_IUPDBUFFER, canvas);
  IupSetAttribute(canvas, "cdCanvas", (char*)cd_canvas);
  return IUP_DEFAULT;
}

int dropfiles_cb(Ihandle* ih, const char* filename)
{
  if (save_check(ih))
    open_file(ih, filename);

  return IUP_DEFAULT;
}

int canvas_unmap_cb(Ihandle* canvas)
{
  cdCanvas* cd_canvas = (cdCanvas*)IupGetAttribute(canvas, "cdCanvas");
  cdKillCanvas(cd_canvas);
  return IUP_DEFAULT;
}

int zoomout_action_cb(Ihandle* ih)
{
  Ihandle* zoom_val = IupGetDialogChild(ih, "ZOOMVAL");
  double zoom_index = IupGetDouble(zoom_val, "VALUE");
  zoom_index--;
  if (zoom_index < -6)
    zoom_index = -6;
  IupSetDouble(zoom_val, "VALUE", round(zoom_index));  /* fixed increments when using buttons */

  zoom_update(ih, zoom_index);
  return IUP_DEFAULT;
}

int zoomin_action_cb(Ihandle* ih)
{
  Ihandle* zoom_val = IupGetDialogChild(ih, "ZOOMVAL");
  double zoom_index = IupGetDouble(zoom_val, "VALUE");
  zoom_index++;
  if (zoom_index > 6)
    zoom_index = 6;
  IupSetDouble(zoom_val, "VALUE", round(zoom_index));  /* fixed increments when using buttons */

  zoom_update(ih, zoom_index);
  return IUP_DEFAULT;
}

int actualsize_action_cb(Ihandle* ih)
{
  Ihandle* zoom_val = IupGetDialogChild(ih, "ZOOMVAL");
  IupSetDouble(zoom_val, "VALUE", 0);
  zoom_update(ih, 0);
  return IUP_DEFAULT;
}

int canvas_resize_cb(Ihandle* canvas)
{
  imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");
  IupSetInt(canvas,"resize_update", 1);

  if (image)
  {
    float old_center_x, old_center_y;
    int view_width, view_height; //, layout_width, layout_height;
    setUnitLayoutSize(canvas);
	IupGetIntInt(canvas, "lwh", &view_width, &view_height);

    scroll_calc_center(canvas, &old_center_x, &old_center_y);

    scroll_update(canvas, view_width, view_height);

    scroll_center(canvas, old_center_x, old_center_y);
  }
  return IUP_DEFAULT;
}

int canvaslayout_wheel_cb(Ihandle* canvaslayout, float delta)
{
	float posy = IupGetFloat(canvaslayout, "POSY");
	posy -= delta * IupGetFloat(canvaslayout, "DY") / 10.0f;
	IupSetFloat(canvaslayout, "POSY", posy);
	IupUpdate(canvaslayout);
	return IUP_DEFAULT;
}

int canvas_wheel_cb(Ihandle* canvas, float delta)
{
  if (IupGetInt(NULL, "CONTROLKEY"))
  {
    if (delta < 0)
      zoomout_action_cb(canvas);
    else
      zoomin_action_cb(canvas);
  }
  else
  {
//    float posy = IupGetFloat(canvas, "POSY");
//    posy -= delta * IupGetFloat(canvas, "DY") / 10.0f;
//    IupSetFloat(canvas, "POSY", posy);
//    IupUpdate(canvas);

	Ihandle* seqobj = IupGetDialogChild(canvas, "SEQLABELCUR");
	Ihandle* seqlblmax = IupGetDialogChild(canvas, "SEQLABEL");
	unsigned int seq_val = IupGetInt(seqobj, "TITLE");
	unsigned int seq_val_max = IupGetInt(seqlblmax, "TITLE");
//
//	if(seq_val > 1)seq_val--;
//
//	seq_update(ih, seq_val);
	  if(delta > 0)
	  {
		  if(seq_val < seq_val_max) seq_val++;
	  }
	  else if(delta < 0)
	  {
		  if(seq_val > 1)seq_val--;
	  }
	  seq_update(canvas, seq_val);
  }
  return IUP_DEFAULT;
}

int canvas_key_action_cb(Ihandle* canvas, int c)
{
//	if(c == K_cV)
//	{
//		Ihandle *clipboard = IupClipboard();
//		imImage* image = IupGetNativeHandleImage(IupGetAttribute(clipboard, "NATIVEIMAGE"));
//		IupDestroy(clipboard);
//
//		if (!image)
//		{
//		  show_error("Invalid Clipboard Data", 1);
//		  return IUP_DEFAULT;
//		}
//
//		set_new_image(canvas, image, NULL, 1);  /* set dirty */
//	}
//	if(c == K_cC)
//	{
//		imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");
//		Ihandle *clipboard = IupClipboard();
//		IupSetAttribute(clipboard, "NATIVEIMAGE", (char*)IupGetImageNativeHandle(image));
//		IupDestroy(clipboard);
//	}

	return IUP_DEFAULT;
}

int zoom_valuechanged_cb(Ihandle* val)
{
  double zoom_index = IupGetDouble(val, "VALUE");
  zoom_update(val, zoom_index);
  return IUP_DEFAULT;
}

int file_menu_open_cb(Ihandle* ih)
{
  Ihandle* item_revert = IupGetDialogChild(ih, "ITEM_REVERT");
  Ihandle* item_save = IupGetDialogChild(ih, "ITEM_SAVE");
  Ihandle* canvas = IupGetDialogChild(ih, "CANVAS");
  char* filename = IupGetAttribute(canvas, "FILENAME");
  int dirty = IupGetInt(canvas, "DIRTY");

  if (dirty)
    IupSetAttribute(item_save, "ACTIVE", "YES");
  else
    IupSetAttribute(item_save, "ACTIVE", "NO");

  if (dirty && filename)
    IupSetAttribute(item_revert, "ACTIVE", "YES");
  else
    IupSetAttribute(item_revert, "ACTIVE", "NO");
  return IUP_DEFAULT;
}

int edit_menu_open_cb(Ihandle* ih)
{
  Ihandle *clipboard = IupClipboard();

  Ihandle *item_paste = IupGetDialogChild(ih, "ITEM_PASTE");

  if (!IupGetInt(clipboard, "IMAGEAVAILABLE"))
    IupSetAttribute(item_paste, "ACTIVE", "NO");
  else
    IupSetAttribute(item_paste, "ACTIVE", "YES");

  IupDestroy(clipboard);
  return IUP_DEFAULT;
}

int config_recent_cb(Ihandle* ih)
{
  if (save_check(ih))
  {
    char* filename = IupGetAttribute(ih, "TITLE");
    open_file(ih, filename);
  }
  return IUP_DEFAULT;
}

int item_new_action_cb(Ihandle* item_new)
{
	unsigned char i;
//  if (save_check(item_new))
//  {
    Ihandle* canvas = IupGetDialogChild(item_new, "CANVAS");
    Ihandle* seqlbl = IupGetDialogChild(item_new, "SEQLABEL");
    Ihandle* seqval = IupGetDialogChild(item_new, "SEQVAL");
//    Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");
//    int width = IupConfigGetVariableIntDef(config, "NewImage", "Width", 640);
//    int height = IupConfigGetVariableIntDef(config, "NewImage", "Height", 480);
//
//    if (IupGetParam("New Image", NULL, NULL, "Width: %i[1,]\nHeight: %i[1,]\n", &width, &height, NULL))
//    {
//      imImage* image = imImageCreate(width, height, IM_RGB, IM_BYTE);
//
//      IupConfigSetVariableInt(config, "NewImage", "Width", width);
//      IupConfigSetVariableInt(config, "NewImage", "Height", height);
//
//      set_new_image(canvas, image, NULL, 0);
//    }
//  }
    char *bptr= IupGetAttribute(canvas,"block_pointer");
    for (i = 0; i < 64; i++)
    	bptr[i] = 1;

    IupSetAttribute(canvas,"block_pointer", bptr);
	remove_seq_step_files(IupGetInt(seqlbl, "TITLE") + 1);
	load_seq_step(item_new,1);
	IupSetInt(seqlbl, "TITLE", 1);

	IupSetInt(seqval, "MAX", 1);
	IupSetInt(seqval, "SHOWTICKS", 2);
	IupSetInt(seqval, "VALUE", 0);

	IupSetInt(IupGetDialogChild(item_new, "DWELLTEXT"), "VALUE", 100);
	IupSetInt(IupGetDialogChild(item_new, "SPEEDFACTORTEXT"), "VALUE", 128);
	IupSetInt(IupGetDialogChild(item_new, "SPEEDFACTORTEXTSHOW"), "VALUE", 100);
	IupSetAttribute(IupGetDialogChild(item_new, "DELAYLABEL"), "TITLE", "100.00 ms");
	IupSetAttribute(IupGetDialogChild(item_new, "MACHNOTEXT"), "VALUE", "1");
	IupSetInt(IupGetDialogChild(item_new, "LAYLABEL"), "TITLE", 1);
	IupSetInt(canvas,"layout_num", 1);

	setUnitLayoutImages(canvas);
	setUnitLayoutSize(canvas);

	IupUpdate(canvas);
  return IUP_DEFAULT;
}

int item_open_action_cb(Ihandle* item_open)
{
  if (!save_check(item_open))
    return IUP_DEFAULT;

  return select_file(IupGetDialog(item_open), 1);
}

int item_saveas_action_cb(Ihandle* item_saveas)
{
	Ihandle* seqlblcur = IupGetDialogChild(item_saveas, "SEQLABELCUR");
	save_seq_step(item_saveas, IupGetInt(seqlblcur, "TITLE"));
  return select_file(IupGetDialog(item_saveas), 0);
}

int item_save_action_cb(Ihandle* item_save)
{
  Ihandle* canvas = IupGetDialogChild(item_save, "CANVAS");
  char* filename = IupGetAttribute(canvas, "FILENAME");
  if (!filename)
    item_saveas_action_cb(item_save);
  else
  {
    /* test again because in can be called using the hot key */
    int dirty = IupGetInt(canvas, "DIRTY");
    if (dirty)
      save_file(canvas);
  }
  return IUP_DEFAULT;
}

int item_revert_action_cb(Ihandle* item_revert)
{
  Ihandle* canvas = IupGetDialogChild(item_revert, "CANVAS");
  char* filename = IupGetAttribute(canvas, "FILENAME");
  open_file(item_revert, filename);
  return IUP_DEFAULT;
}

int item_pagesetup_action_cb(Ihandle* item_pagesetup)
{
  Ihandle* canvas = IupGetDialogChild(item_pagesetup, "CANVAS");
  Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");
  int margin_width = IupConfigGetVariableIntDef(config, "Print", "MarginWidth", 20);
  int margin_height = IupConfigGetVariableIntDef(config, "Print", "MarginHeight", 20);

  if (IupGetParam("Page Setup", NULL, NULL, "Margin Width (mm): %i[1,]\nMargin Height (mm): %i[1,]\n", &margin_width, &margin_height, NULL))
  {
    IupConfigSetVariableInt(config, "Print", "MarginWidth", margin_width);
    IupConfigSetVariableInt(config, "Print", "MarginHeight", margin_height);
  }

  return IUP_DEFAULT;
}

int item_print_action_cb(Ihandle* item_print)
{
  Ihandle* canvas = IupGetDialogChild(item_print, "CANVAS");
  unsigned int ri, gi, bi;
  imImage* image;
  char* title = IupGetAttribute(IupGetDialog(item_print), "TITLE");
  Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");
  const char* background = IupConfigGetVariableStrDef(config, "Canvas", "Background", "255 255 255");

  cdCanvas* cd_canvas = cdCreateCanvasf(CD_PRINTER, "%s -d", title);
  if (!cd_canvas)
    return IUP_DEFAULT;

  /* draw the background */
  sscanf(background, "%u %u %u", &ri, &gi, &bi);
  cdCanvasBackground(cd_canvas, cdEncodeColor((unsigned char)ri, (unsigned char)gi, (unsigned char)bi));
  cdCanvasClear(cd_canvas);

  /* draw the image at the center of the canvas */
  image = (imImage*)IupGetAttribute(canvas, "IMAGE");
  if (image)
  {
    int x, y, canvas_width, canvas_height, view_width, view_height;
    double canvas_width_mm, canvas_height_mm;
    Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");
    int margin_width = IupConfigGetVariableIntDef(config, "Print", "MarginWidth", 20);
    int margin_height = IupConfigGetVariableIntDef(config, "Print", "MarginHeight", 20);

    cdCanvasGetSize(cd_canvas, &canvas_width, &canvas_height, &canvas_width_mm, &canvas_height_mm);

    /* convert to pixels */
    margin_width = (int)((margin_width * canvas_width) / canvas_width_mm);
    margin_height = (int)((margin_height * canvas_height) / canvas_height_mm);

    view_fit_rect(canvas_width - 2 * margin_width, canvas_height - 2 * margin_height,
                  image->width, image->height,
                  &view_width, &view_height);

    x = (canvas_width - view_width) / 2;
    y = (canvas_height - view_height) / 2;

    imcdCanvasPutImage(cd_canvas, image, x, y, view_width, view_height, 0, 0, 0, 0);
  }

  cdKillCanvas(cd_canvas);
  return IUP_DEFAULT;
}

int item_exit_action_cb(Ihandle* item_exit)
{
  Ihandle* dlg = IupGetDialog(item_exit);
  Ihandle* config = (Ihandle*)IupGetAttribute(dlg, "CONFIG");
  Ihandle* canvas = IupGetDialogChild(dlg, "CANVAS");
  imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");

  saveComPortConfig(dlg);

  if (!save_check(item_exit))
    return IUP_IGNORE;  /* to abort the CLOSE_CB callback */

  if (image)
    imImageDestroy(image);

  IupConfigDialogClosed(config, dlg, "MainWindow");

  IupConfigSave(config);
  IupDestroy(config);
  return IUP_CLOSE;
}

int item_copy_action_cb(Ihandle* item_copy)
{
  Ihandle* canvas = IupGetDialogChild(item_copy, "CANVAS");
  imImage* image = (imImage*)IupGetAttribute(canvas, "IMAGE");
  Ihandle *clipboard = IupClipboard();
  IupSetAttribute(clipboard, "NATIVEIMAGE", (char*)IupGetImageNativeHandle(image));
  IupDestroy(clipboard);
  return IUP_DEFAULT;
}

int item_paste_action_cb(Ihandle* item_paste)
{
  if (save_check(item_paste))
  {
    Ihandle* canvas = IupGetDialogChild(item_paste, "CANVAS");

    Ihandle *clipboard = IupClipboard();
    imImage* image = IupGetNativeHandleImage(IupGetAttribute(clipboard, "NATIVEIMAGE"));
    IupDestroy(clipboard);

    if (!image)
    {
      show_error("Invalid Clipboard Data", 1);
      return IUP_DEFAULT;
    }

    set_new_image(canvas, image, NULL, 1);  /* set dirty */
  }
  return IUP_DEFAULT;
}

int item_background_action_cb(Ihandle* item_background)
{
  Ihandle* canvas = IupGetDialogChild(item_background, "CANVAS");
  Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");
  Ihandle* colordlg = IupColorDlg();
  const char* background = IupConfigGetVariableStrDef(config, "Canvas", "Background", "255 255 255");
  IupSetStrAttribute(colordlg, "VALUE", background);
  IupSetAttributeHandle(colordlg, "PARENTDIALOG", IupGetDialog(item_background));

  IupPopup(colordlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

  if (IupGetInt(colordlg, "STATUS") == 1)
  {
    background = IupGetAttribute(colordlg, "VALUE");
    IupConfigSetVariableStr(config, "Canvas", "Background", background);

    IupUpdate(canvas);
  }

  IupDestroy(colordlg);
  return IUP_DEFAULT;
}

int item_toolbar_action_cb(Ihandle* item_toolbar)
{
  Ihandle* canvas = IupGetDialogChild(item_toolbar, "CANVAS");
  Ihandle* toolbar = IupGetChild(IupGetParent(canvas), 0);
  Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");

  toggle_bar_visibility(item_toolbar, toolbar);

  IupConfigSetVariableStr(config, "MainWindow", "Toolbar", IupGetAttribute(item_toolbar, "VALUE"));
  return IUP_DEFAULT;
}

int item_statusbar_action_cb(Ihandle* item_statusbar)
{
  Ihandle* canvas = IupGetDialogChild(item_statusbar, "CANVAS");
  Ihandle* statusbar = IupGetBrother(canvas);
  Ihandle* config = (Ihandle*)IupGetAttribute(canvas, "CONFIG");

  toggle_bar_visibility(item_statusbar, statusbar);

  IupConfigSetVariableStr(config, "MainWindow", "Statusbar", IupGetAttribute(item_statusbar, "VALUE"));
  return IUP_DEFAULT;
}

int item_help_action_cb(void)
{
  IupHelp("http://www.tecgraf.puc-rio.br/iup");
  return IUP_DEFAULT;
}

int item_about_action_cb(void)
{
  IupMessage("About", "   Simple Paint\n\nAutors:\n   Gustavo Lyrio\n   Antonio Scuri");
  return IUP_DEFAULT;
}
