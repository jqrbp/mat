
#include "utils.h"
#include "ui_utils.h"
#include "ui_callback.h"
#include "ui_toolbox_pic.h"

Ihandle* create_main_menu(Ihandle *config)
{
	Ihandle *menu;
	Ihandle *sub_menu_file, *file_menu, *item_exit, *item_new, *item_open, *item_saveas;//*item_save, *item_revert
	Ihandle *sub_menu_edit, *edit_menu, *item_copy, *item_paste, *item_print, *item_pagesetup;
//	Ihandle *sub_menu_help, *help_menu, *item_help, *item_about;
	Ihandle *sub_menu_view, *view_menu, *item_toolbar, *item_statusbar;
	Ihandle *item_zoomin, *item_zoomout, *item_actualsize;
	Ihandle  *recent_menu, *item_background;

	item_new = IupItem("&New\tCtrl+N", NULL);
	IupSetAttribute(item_new, "IMAGE", "IUP_FileNew");
	IupSetCallback(item_new, "ACTION", (Icallback)item_new_action_cb);

	item_open = IupItem("&Open...\tCtrl+O", NULL);
	IupSetAttribute(item_open, "IMAGE", "IUP_FileOpen");
	IupSetCallback(item_open, "ACTION", (Icallback)item_open_action_cb);

//	item_save = IupItem("&Save\tCtrl+S", NULL);
//	IupSetAttribute(item_save, "NAME", "ITEM_SAVE");
//	IupSetAttribute(item_save, "IMAGE", "IUP_FileSave");
//	IupSetCallback(item_save, "ACTION", (Icallback)item_save_action_cb);

	item_saveas = IupItem("Save &As...", NULL);
	IupSetAttribute(item_saveas, "NAME", "ITEM_SAVEAS");
	IupSetCallback(item_saveas, "ACTION", (Icallback)item_saveas_action_cb);

//	item_revert = IupItem("&Revert", NULL);
//	IupSetAttribute(item_revert, "NAME", "ITEM_REVERT");
//	IupSetCallback(item_revert, "ACTION", (Icallback)item_revert_action_cb);

	item_pagesetup = IupItem("Page Set&up...", NULL);
	IupSetCallback(item_pagesetup, "ACTION", (Icallback)item_pagesetup_action_cb);

	item_print = IupItem("&Print...\tCtrl+P", NULL);
	IupSetCallback(item_print, "ACTION", (Icallback)item_print_action_cb);

	item_exit = IupItem("E&xit", NULL);
	IupSetCallback(item_exit, "ACTION", (Icallback)item_exit_action_cb);

	item_copy = IupItem("&Copy\tCtrl+C", NULL);
	IupSetAttribute(item_copy, "NAME", "ITEM_COPY");
	IupSetAttribute(item_copy, "IMAGE", "IUP_EditCopy");
	IupSetCallback(item_copy, "ACTION", (Icallback)item_copy_action_cb);

	item_paste = IupItem("&Paste\tCtrl+V", NULL);
	IupSetAttribute(item_paste, "NAME", "ITEM_PASTE");
	IupSetAttribute(item_paste, "IMAGE", "IUP_EditPaste");
	IupSetCallback(item_paste, "ACTION", (Icallback)item_paste_action_cb);

	item_zoomin = IupItem("Zoom &In\tCtrl++", NULL);
	IupSetAttribute(item_zoomin, "IMAGE", "IUP_ZoomIn");
	IupSetCallback(item_zoomin, "ACTION", (Icallback)zoomin_action_cb);

	item_zoomout = IupItem("Zoom &Out\tCtrl+-", NULL);
	IupSetAttribute(item_zoomout, "IMAGE", "IUP_ZoomOut");
	IupSetCallback(item_zoomout, "ACTION", (Icallback)zoomout_action_cb);

	item_actualsize = IupItem("&Actual Size\tCtrl+0", NULL);
	IupSetAttribute(item_actualsize, "IMAGE", "IUP_ZoomActualSize");
	IupSetCallback(item_actualsize, "ACTION", (Icallback)actualsize_action_cb);

	item_background = IupItem("&Background...", NULL);
	IupSetCallback(item_background, "ACTION", (Icallback)item_background_action_cb);

	item_toolbar = IupItem("&Toobar", NULL);
	IupSetCallback(item_toolbar, "ACTION", (Icallback)item_toolbar_action_cb);
	IupSetAttribute(item_toolbar, "VALUE", "ON");

	item_statusbar = IupItem("&Statusbar", NULL);
	IupSetCallback(item_statusbar, "ACTION", (Icallback)item_statusbar_action_cb);
	IupSetAttribute(item_statusbar, "VALUE", "ON");

//	item_help = IupItem("&Help...", NULL);
//	IupSetCallback(item_help, "ACTION", (Icallback)item_help_action_cb);
//
//	item_about = IupItem("&About...", NULL);
//	IupSetCallback(item_about, "ACTION", (Icallback)item_about_action_cb);

	recent_menu = IupMenu(NULL);

	file_menu = IupMenu(
	item_new,
	item_open,
//	item_save,
	item_saveas,
//	item_revert,
	IupSeparator(),
//	item_pagesetup,
//	item_print,
//	IupSeparator(),
	IupSubmenu("Recent &Files", recent_menu),
	IupSeparator(),
	item_exit,
	NULL);
	edit_menu = IupMenu(
	item_copy,
	item_paste,
	NULL);
	view_menu = IupMenu(
	item_zoomin,
	item_zoomout,
	item_actualsize,
	IupSeparator(),
	item_background,
	IupSeparator(),
	item_toolbar,
	item_statusbar,
	NULL);
//	help_menu = IupMenu(
//	item_help,
//	item_about,
//	NULL);

	IupSetCallback(file_menu, "OPEN_CB", (Icallback)file_menu_open_cb);
	IupSetCallback(edit_menu, "OPEN_CB", (Icallback)edit_menu_open_cb);

	sub_menu_file = IupSubmenu("&File", file_menu);
	sub_menu_edit = IupSubmenu("&Edit", edit_menu);
	sub_menu_view = IupSubmenu("&View", view_menu);
//	sub_menu_help = IupSubmenu("&Help", help_menu);

	menu = IupMenu(
	sub_menu_file,
	sub_menu_edit,
	sub_menu_view,
//	sub_menu_help,
	NULL);

	  /* Initialize variables from the configuration file */
	if (!IupConfigGetVariableIntDef(config, "MainWindow", "Statusbar", 1))
	  {
	    IupSetAttribute(item_statusbar, "VALUE", "OFF");
	  }

	if (!IupConfigGetVariableIntDef(config, "MainWindow", "Toolbar", 1))
	  {
	    IupSetAttribute(item_toolbar, "VALUE", "OFF");
	  }

	IupConfigRecentInit(config, recent_menu, config_recent_cb, 10);

	return menu;
}

Ihandle* create_status_bar(Ihandle *config)
{
	Ihandle *statusbar;
	Ihandle *dwelltxt, *layoutlabel, *speedfactorshowtxt, *seqnumtxt;

	seqnumtxt = IupSetAttributes(IupText(NULL), "SIZE=27x, PADDING=5x5, VALUE=1, ACTIVE=NO, NAME=SEQNUMTEXT, ALIGNMENT=ARIGHT,TIP=\"Sequence Number\"");
	speedfactorshowtxt = IupSetCallbacks(IupSetAttributes(IupText(NULL), "SIZE=20x, PADDING=5x5, VALUE=100, NAME=SPEEDFACTORTEXTSHOW, ACTIVE=NO, ALIGNMENT=ARIGHT,TIP=\"Speed Factor (%)\""),
			 "K_ANY", (Icallback)speedfactorshow_valuechanged_cb, NULL);

	dwelltxt = IupText(NULL);
	IupSetAttributes(dwelltxt, "SIZE=30x, PADDING=5x5, ACTIVE=NO, VALUE=100, NAME=DWELLTEXT, TIP=\"Dwell time\", ALIGNMENT=ACENTER");
	IupSetCallbacks(dwelltxt, "K_ANY", (Icallback)dwell_valuechanged_cb, NULL);

	layoutlabel = IupSetAttributes(IupLabel(NULL), "SIZE=12x, PADDING=5x5, TITLE=1, NAME=LAYLABEL, READONLY=YES, ALIGNMENT=ARIGHT,TIP=\"Layout Num\"");

	  statusbar = IupHbox(
#ifdef FULLVERSION
	    IupSetAttributes(IupLabel("Goto:"), "SIZE=25x, PADDING=5x5, ALIGNMENT=ARIGHT"),
		IupSetCallbacks(IupSetAttributes(IupText(NULL), "SIZE=27x, PADDING=5x5, VALUE=1, NAME=SEQTEXT, ALIGNMENT=ARIGHT,TIP=\"Goto Scene Num\""), "K_ANY", (Icallback)seqjump_action_cb, NULL),
		IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
#endif
		IupSetAttributes(IupLabel("1"), "SIZE=22x, TIP=\"Current Scene\", NAME=SEQLABELCUR, ALIGNMENT=ARIGHT"),
		IupSetAttributes(IupLabel("/"), "SIZE=5x, PADDING=5x5, ALIGNMENT=ACENTER"),
		IupSetAttributes(IupLabel("1"), "SIZE=22x, TIP=\"Max Scene\", NAME=SEQLABEL, ALIGNMENT=ALEFT"),
		IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
#ifdef FULLVERSION
		IupSetCallbacks(IupSetAttributes(IupButton(NULL, NULL), "IMAGE=IUP_mypsm_remove, NAME=SCENERMBTN, FLAT=Yes, TIP=\"Remove Scene\""), "ACTION", scenerm_action_cb, NULL),
		IupSetCallbacks(IupSetAttributes(IupButton(NULL, NULL), "IMAGE=IUP_mypsm_add, NAME=SCENEADDBTN, FLAT=Yes, TIP=\"Insert Scene\""), "ACTION", sceneadd_action_cb, NULL),
		IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
#endif
		IupSetAttributes(IupLabel("Mach No:"), "PADDING=5x5, ALIGNMENT=ARIGHT"),
		IupSetCallbacks(IupSetAttributes(IupText(NULL), "SIZE=20x, PADDING=5x5, VALUE=1, NAME=MACHNOTEXT, ACTIVE=NO, ALIGNMENT=ARIGHT,TIP=\"Machine Serial Number\""), "K_ANY", (Icallback)machno_valuechanged_cb, NULL),
		IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
		IupSetAttributes(IupLabel("Layout:"), "SIZE=30x, PADDING=5x5, ALIGNMENT=ARIGHT"),
#ifdef FULLVERSION
		IupSetCallbacks(IupSetAttributes(IupSpinbox (layoutlabel),"ACTIVE=NO, NAME=LAYSPIN"),"SPIN_CB", (Icallback)layoutspin_action_cb, NULL),
#else
		layoutlabel,
#endif
		IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
		IupSetAttributes(IupLabel("Seq:"), "SIZE=25x, PADDING=5x5, ALIGNMENT=ARIGHT"),
#ifdef FULLVERSION
		IupSetCallbacks(seqnumtxt, "K_ANY", (Icallback)seqnum_action_cb, NULL),
#else
		seqnumtxt,
#endif
		IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
		IupSetAttributes(IupLabel("Factor (%):"), "PADDING=8x5, ALIGNMENT=ARIGHT"),
#ifdef FULLVERSION
		IupSetCallbacks(IupSetAttributes(IupSpinbox (speedfactorshowtxt),"ACTIVE=NO, NAME=SPEEDFACTORSPIN"),"SPIN_CB", (Icallback)speedfactorspin_action_cb, NULL),
#else
		speedfactorshowtxt,
#endif
		IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
		IupSetAttributes(IupLabel("Dwell: "), "ALIGNMENT=ACENTER"),
#ifdef FULLVERSION
		IupSetCallbacks(IupSetAttributes(IupSpinbox (dwelltxt), "NAME=DWELLSPIN, ACTIVE=NO"),"SPIN_CB", (Icallback)dwellspin_action_cb, NULL),
#else
		dwelltxt,
#endif
		IupSetAttributes(IupLabel("100.00 ms"), "SIZE=50x, PADDING=5x5, ALIGNMENT=ACENTER, NAME=DELAYLABEL, TIP=\"Delay time (min. 10 ms)\""),
//	    IupSetAttributes(IupLabel("0 x 0"), "SIZE=70x, PADDING=10x5, NAME=SIZELABEL, ALIGNMENT=ACENTER"),
//	    IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
		IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
		IupSetAttributes(IupLabel(NULL), "EXPAND=HORIZONTAL"),
		IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
	    IupSetAttributes(IupLabel("100%"), "SIZE=30x, PADDING=10x5, NAME=ZOOMLABEL, ALIGNMENT=ARIGHT"),
//	    IupSetCallbacks(IupSetAttributes(IupButton(NULL, NULL), "IMAGE=IUP_ZoomOut, FLAT=Yes, TIP=\"Zoom Out (Ctrl+-)\""), "ACTION", zoomout_action_cb, NULL),
	    IupSetCallbacks(IupSetAttributes(IupVal(NULL), "VALUE=0, MIN=-3, MAX=3, RASTERSIZE=80x25, NAME=ZOOMVAL"), "VALUECHANGED_CB", zoom_valuechanged_cb, NULL),
//	    IupSetCallbacks(IupSetAttributes(IupButton(NULL, NULL), "IMAGE=IUP_ZoomIn, FLAT=Yes, TIP=\"Zoom In (Ctrl++)\""), "ACTION", zoomin_action_cb, NULL),
	    IupSetCallbacks(IupSetAttributes(IupButton(NULL, NULL), "IMAGE=IUP_ZoomActualSize, FLAT=Yes, TIP=\"Actual Size (Ctrl+0)\""), "ACTION", actualsize_action_cb, NULL),
		IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
//		IupSetAttributes(IupLabel("1"), "VISIBLE=NO, NAME=SPEEDLABEL"),
//		IupSetAttributes(IupLabel("1x"), "SIZE=30x, PADDING=5x5, NAME=SPEEDLABELSHOW, ALIGNMENT=ACENTER,TIP=\"Playback Speed\""),
//		IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
		IupSetAttributes(IupLabel("Search"), "SIZE = 33x, NAME=MATMCULABEL"),
		IupSetCallbacks(IupSetAttributes(IupTimer(), "TIME=10, RUN=NO, NAME=SEQTIMER"), "ACTION_CB", seqtimer_action_cb, NULL),
		IupSetCallbacks(IupSetAttributes(IupText(NULL), "VALUE=128, VISIBLE=NO, NAME=SPEEDFACTORTEXT, ACTIVE=NO"),
								 "K_ANY", (Icallback)speedfactor_valuechanged_cb, NULL),
	    NULL);
	  IupSetAttribute(statusbar, "NAME", "STATUSBAR");
	  IupSetAttribute(statusbar, "ALIGNMENT", "ACENTER");

	  /* Initialize variables from the configuration file */
	  if (!IupConfigGetVariableIntDef(config, "MainWindow", "Statusbar", 1))
	  {
		IupSetAttribute(statusbar, "FLOATING", "YES");
		IupSetAttribute(statusbar, "VISIBLE", "NO");
	  }

	  return statusbar;
}

Ihandle* create_item_comport(Ihandle *config)
{
//	int i, cmdcnt;
//	char strTmp[5];
	Ihandle *item_comport;

	Ihandle *btn_comport, *comport_list, *comport_timer, *comport_timersearch, *comport_baud_list;

#ifdef FULLVERSION
	Ihandle *btn_read_flash_comport, *btn_send_seq_comport; //*btn_disconnect_comport
//	Ihandle *item_comport_txtlist;
	Ihandle *btn_send_comport,  *btn_monitor_comport;
#endif

	comport_list = IupList(NULL);
	IupSetAttributes(comport_list, "SIZE = 30x, VISIBLE=YES, NAME=COMPORTLIST, EDITBOX=NO, DROPDOWN = YES, MULTIPLE=NO, 1=0, TIP=\"Serial Port Number\"");
//	IupSetCallback(comport_list, "VALUECHANGED_CB", (Icallback)comport_valuechanged_cb);
//	IupSetCallback(comport_list, "DROPDOWN_CB", (Icallback)comport_dropped__cb);
//	IupSetCallback(comport_list, "ACTION", (Icallback)comportlist_action_cb);

	comport_baud_list = IupList(NULL);
	IupSetAttributes(comport_baud_list, "SIZE = 50x, NAME=COMPORTBAUDLIST, VISIBLE=NO, EDITBOX=NO, DROPDOWN = YES, MULTIPLE=NO, TIP=\"Baud Rate\"");
	IupSetAttributes(comport_baud_list, "1=1200,2=2400,3=4800,4=9600,5=19200,6=38400,7=57600,8=115200,9=460800,10=921600, VISIBLE_ITEMS = 11, VALUE=8");

	btn_comport = IupButton("Connect", NULL);
		IupSetAttributes(btn_comport, "SIZE = 50x, ACTIVE = NO, NAME = COMPORTBUTTON, FLAT = YES, CANFOCUS =No");
		IupSetCallback(btn_comport, "BUTTON_CB", (Icallback)comport_connect_click_cb);

//	btn_disconnect_comport = IupButton("Disconnect", NULL);
//			IupSetAttributes(btn_disconnect_comport, "SIZE = 50x, NAME = COMPORTDISBUTTON, ACTIVE = NO, CANFOCUS =No");
//			IupSetCallback(btn_disconnect_comport, "ACTION", (Icallback)comport_connect_action_cb);

	comport_timer = IupSetCallbacks(IupSetAttributes(IupTimer(), "TIME=15, RUN=NO, NAME=COMPORTTIMER, COMPORTMODE = 0, ACKNUM = 0"), "ACTION_CB", comport_timer_action_cb, NULL);
	comport_timersearch = IupSetCallbacks(IupSetAttributes(IupTimer(), "TIME=10, RUN=YES, NAME=COMPORTTIMERSEARCH, COMPORTMODE = 0, ACKNUM = 0"), "ACTION_CB", comport_timersearch_action_cb, NULL);
//	btn_auto_comport = IupButton("Auto", NULL);
//			IupSetAttributes(btn_auto_comport, "SIZE = 40x, NAME = COMPORTAUTOBUTTON, CANFOCUS =No");
//			IupSetCallback(btn_auto_comport, "ACTION", (Icallback)item_autocomport_action_cb);

//	item_comport_txtlist = IupList(NULL);
//	IupSetAttributes(item_comport_txtlist, "EXPAND = HORIZONTAL, DROPEXPAND = YES, EDITBOX = YES, DROPDOWN = YES, MULTIPLE=NO, ACTIVE = NO, VISIBLE_ITEMS = 10, NAME=COMPORTTXTLIST, TIP=\"Comm Port Command\"");
//	IupSetCallback(item_comport_txtlist, "ACTION", (Icallback)item_comporttxtlist_action_cb);
//	IupSetCallback(item_comport_txtlist, "VALUECHANGED_CB", (Icallback)item_comporttxtlist_valuechanged_cb);
//	IupSetCallback(item_comport_txtlist, "DROPDOWN_CB", (Icallback)item_comporttxtlist_dropped_cb);

#ifdef FULLVERSION
	btn_send_comport = IupButton("Command", NULL);
				IupSetAttributes(btn_send_comport, "SIZE = 50x, ACTIVE = NO, NAME = COMPORTSENDBUTTON, CANFOCUS =No");
				IupSetCallback(btn_send_comport, "ACTION", (Icallback)item_sendcomport_action_cb);

	btn_send_seq_comport = IupButton("Write", NULL);
				IupSetAttributes(btn_send_seq_comport, "NAME = COMPORTSENDSEQBUTTON, ACTIVE = NO, CANFOCUS =No, FLAT = YES, TIP=\"Write Scenes to Device\"");
				IupSetCallback(btn_send_seq_comport, "BUTTON_CB", (Icallback)comport_sendseq_click_cb);

	btn_read_flash_comport = IupButton("Read", NULL);
				IupSetAttributes(btn_read_flash_comport, "SIZE = 30x, NAME = COMPORTREADFLASHBUTTON, ACTIVE = NO, CANFOCUS =No, FLAT = YES, TIP=\"Read Scenes from Device\"");
				IupSetCallback(btn_read_flash_comport, "BUTTON_CB", (Icallback)comport_read_click_cb);

	btn_monitor_comport = IupButton("Monitor", NULL);
				IupSetAttributes(btn_monitor_comport, "SIZE = 30x, NAME = COMPORTMONITORBUTTON, ACTIVE = NO, CANFOCUS =No, FLAT = YES, TIP=\"Play Scenes from Device\"");
				IupSetCallback(btn_monitor_comport, "BUTTON_CB", (Icallback)comport_monitor_click_cb);
#endif
	item_comport =
		IupSetAttributes(IupHbox(
//		IupSetAttributes(IupLabel("SERIAL COM "), "ALIGNMENT=ARIGHT"),
		IupLabel("COM:"),
		comport_list,
		btn_comport,
//		btn_disconnect_comport,
//		btn_auto_comport,
		comport_timer,
		comport_timersearch,
		IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
//		item_comport_txtlist,
//		btn_send_comport,
#ifdef FULLVERSION
		btn_send_seq_comport,
		btn_read_flash_comport,
		btn_monitor_comport,
#endif
		comport_baud_list,
		IupSetAttributes(IupLabel(NULL), "EXPAND=HORIZONTAL"),
		NULL), "ALIGNMENT=ACENTER");

//  IupSetAttribute(item_comport, "MARGIN", "5x5");
  IupSetAttribute(item_comport, "GAP", "2");

  if (!IupConfigGetVariableIntDef(config, "MainWindow", "ComPortToolbar", 1))
	{
	  IupSetAttribute(item_comport, "FLOATING", "YES");
	  IupSetAttribute(item_comport, "VISIBLE", "NO");
	}

#ifndef FULLVERSION
  	  IupSetInt(comport_timer, "COMPORTMODE", (int)COMPORTMONITOR);
#endif
//  if (IupConfigGetVariableIntDef(config, "ComPort", "CommandCnt", 0))
//	{
//	  cmdcnt = IupConfigGetVariableInt(config, "ComPort", "CommandCnt");
//	  for(i=0;i<cmdcnt;i++)
//	  {
//		  sprintf(strTmp,"%d",i + 1);
//		  IupSetAttribute(item_comport_txtlist, strTmp,
//		  IupConfigGetVariableStrId(config,"ComPort","Command",i+1));
//	  }
//	}

	return item_comport;
}

Ihandle* create_toolbar(Ihandle *config)
{
	Ihandle *btn_copy, *btn_paste, *btn_new, *btn_open, *btn_save, *btn_test;
	Ihandle *toolbar;

	btn_new = IupButton(NULL, NULL);
	IupSetAttribute(btn_new, "IMAGE", "IUP_FileNew");
	IupSetAttribute(btn_new, "FLAT", "Yes");
	IupSetCallback(btn_new, "ACTION", (Icallback)item_new_action_cb);
	IupSetAttribute(btn_new, "TIP", "New (Ctrl+N)");
	IupSetAttribute(btn_new, "CANFOCUS", "No");

	btn_open = IupButton(NULL, NULL);
	IupSetAttribute(btn_open, "IMAGE", "IUP_FileOpen");
	IupSetAttribute(btn_open, "FLAT", "Yes");
	IupSetCallback(btn_open, "ACTION", (Icallback)item_open_action_cb);
	IupSetAttribute(btn_open, "TIP", "Open (Ctrl+O)");
	IupSetAttribute(btn_open, "CANFOCUS", "No");

	btn_save = IupButton(NULL, NULL);
	IupSetAttribute(btn_save, "IMAGE", "IUP_FileSave");
	IupSetAttribute(btn_save, "FLAT", "Yes");
	IupSetCallback(btn_save, "ACTION", (Icallback)item_saveas_action_cb);
	IupSetAttribute(btn_save, "TIP", "Save (Ctrl+S)");
	IupSetAttribute(btn_save, "CANFOCUS", "No");

	btn_copy = IupButton(NULL, NULL);
	IupSetAttribute(btn_copy, "IMAGE", "IUP_EditCopy");
	IupSetAttribute(btn_copy, "FLAT", "Yes");
	IupSetCallback(btn_copy, "ACTION", (Icallback)item_copy_action_cb);
	IupSetAttribute(btn_copy, "TIP", "Copy (Ctrl+C)");
	IupSetAttribute(btn_copy, "CANFOCUS", "No");

	btn_paste = IupButton(NULL, NULL);
	IupSetAttribute(btn_paste, "IMAGE", "IUP_EditPaste");
	IupSetAttribute(btn_paste, "FLAT", "Yes");
	IupSetCallback(btn_paste, "ACTION", (Icallback)item_paste_action_cb);
	IupSetAttribute(btn_paste, "TIP", "Paste (Ctrl+V)");
	IupSetAttribute(btn_paste, "CANFOCUS", "No");

	btn_test = IupButton(NULL, NULL);
	IupSetAttribute(btn_test, "IMAGE", "IUP_EditPaste");
	IupSetAttribute(btn_test, "FLAT", "Yes");
	IupSetCallback(btn_test, "ACTION", (Icallback)item_test_action_cb);
	IupSetAttribute(btn_test, "TIP", "test");
	IupSetAttribute(btn_test, "CANFOCUS", "No");

	  toolbar = IupHbox(
	    btn_new,
	    btn_open,
	    btn_save,
	    IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
//	    btn_copy,
//	    btn_paste,
//		btn_test,
		create_item_comport(config),
	    NULL);
	  IupSetAttribute(toolbar, "ALIGNMENT", "ACENTER");
//	  IupSetAttribute(toolbar, "MARGIN", "5x5");
	  IupSetAttribute(toolbar, "GAP", "2");

	  if (!IupConfigGetVariableIntDef(config, "MainWindow", "Toolbar", 1))
	    {
	      IupSetAttribute(toolbar, "FLOATING", "YES");
	      IupSetAttribute(toolbar, "VISIBLE", "NO");
	    }

	  return toolbar;
}

Ihandle* create_main_dialog(Ihandle *config)
{
	unsigned char i;
	time_t rawtime;
	struct tm * timeinfo;


  Ihandle *dlg, *vbox, *canvas;

  Ihandle *canvaslayout_expander, *canvaslayout;
  char *blocktgl = (char*)malloc(64 * sizeof(char *));
  char *blocktgl_old =  (char*)malloc(64 * sizeof(char *));
  char *timestart = (char*)calloc(10, sizeof(char *));

  for(i=0;i<64;i++)
  {
	  blocktgl[i]=1;
	  blocktgl_old[i]=1;
  }

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (timestart,9,"%H:%M:%S",timeinfo);

  canvaslayout = IupCanvas(NULL);
  IupSetAttribute(canvaslayout, "NAME", "CANVASLAYOUT");
  IupSetAttribute(canvaslayout, "EXPAND", "VERTICAL");
  IupSetAttribute(canvaslayout, "BORDER", "NO");
  IupSetAttribute(canvaslayout, "SCROLLBAR", "YES");
  IupSetCallback(canvaslayout, "ACTION", (Icallback)canvaslayout_action_cb);
  IupSetCallback(canvaslayout, "MAP_CB", (Icallback)canvaslayout_map_cb);
  IupSetCallback(canvaslayout, "UNMAP_CB", (Icallback)canvaslayout_unmap_cb);
#ifdef FULLVERSION
  IupSetCallback(canvaslayout, "BUTTON_CB", (Icallback)canvaslayout_click_cb);
#endif
  IupSetCallback(canvaslayout, "WHEEL_CB", (Icallback)canvaslayout_wheel_cb);

  canvaslayout_expander = IupExpander(canvaslayout);
    IupSetAttribute(canvaslayout_expander, "NAME", "CANVASLAYOUTEXPANDER");
    IupSetAttribute(canvaslayout_expander, "EXPAND", "VERTICAL");
    IupSetAttribute(canvaslayout_expander, "BARPOSITION", "RIGHT");
    IupSetAttribute(canvaslayout_expander, "STATE", "CLOSE");
    IupSetCallback(canvaslayout_expander, "OPENCLOSE_CB", (Icallback)canvaslayout_openclose_cb);

	canvas = IupCanvas(NULL);
	IupSetAttribute(canvas, "NAME", "CANVAS");
	IupSetAttribute(canvas, "SCROLLBAR", "YES");
	IupSetAttribute(canvas, "EXPAND", "YES");
	IupSetAttribute(canvas, "DIRTY", "NO");  /* custom attribute */
	IupSetAttribute(canvas, "layout_num", "1");
	IupSetAttribute(canvas, "on_update", "0");
	IupSetAttribute(canvas,"block_pointer", blocktgl);
	IupSetAttribute(canvas, "block_pointer_old", blocktgl_old);
	IupSetAttribute(canvas, "timestart", timestart);
	IupSetCallback(canvas, "ACTION", (Icallback)canvas_action_cb);
	IupSetCallback(canvas, "DROPFILES_CB", (Icallback)dropfiles_cb);
	IupSetCallback(canvas, "MAP_CB", (Icallback)canvas_map_cb);
	IupSetCallback(canvas, "UNMAP_CB", (Icallback)canvas_unmap_cb);
	IupSetCallback(canvas, "WHEEL_CB", (Icallback)canvas_wheel_cb);
	IupSetCallback(canvas, "RESIZE_CB", (Icallback)canvas_resize_cb);
	IupSetCallback(canvas, "BUTTON_CB", (Icallback)canvas_click_cb);

#ifdef FULLVERSION
	IupSetCallback(canvas, "MOTION_CB", (Icallback)canvas_motion_cb);
//	IupSetCallback(canvas, "K_ANY", (Icallback)canvas_key_action_cb);

	Ihandle* playcontrol = IupSetAttributes(IupFrame(IupHbox(
//	IupSetCallbacks(IupSetAttributes(IupButton(NULL, NULL), "IMAGE=IUP_MediaRewind, FLAT=Yes, TIP=\"Slower\""), "ACTION", speeddown_action_cb, NULL),
	IupSetCallbacks(IupSetAttributes(IupButton(NULL, NULL), "IMAGE=IUP_Prev, FLAT=Yes, TIP=\"Back (a)\""), "ACTION", seqrewind_action_cb, NULL),
	IupSetCallbacks(IupSetAttributes(IupVal(NULL), "VALUE=0, PAGESTEP=1, SHOWTICKS=1, STEP=1, EXPAND=HORIZONTAL, NAME=SEQVAL"), "VALUECHANGED_CB", seq_valuechanged_cb, NULL),
	IupSetCallbacks(IupSetAttributes(IupButton(NULL, NULL), "IMAGE=IUP_Next, FLAT=Yes, TIP=\"Next (d)\""), "ACTION", seqforward_action_cb, NULL),
	IupSetCallbacks(IupSetAttributes(IupToggle(NULL, NULL), "IMAGE=IUP_Play, NAME=PLAYTOGGLE, FLAT=Yes, TIP=\"Play (p)\""), "ACTION", (Icallback)seqplay_action_cb, NULL),
//	IupSetCallbacks(IupSetAttributes(IupButton(NULL, NULL), "IMAGE=IUP_Stop, FLAT=Yes, TIP=\"Stop (s)\""), "ACTION", seqstop_action_cb, NULL),
	IupSetAttributes(IupToggle(NULL, NULL), "IMAGE=IUP_Redo, NAME=PLAYREPEATTGL, FLAT=Yes, TIP=\"Repeat\""),
//	IupSetCallbacks(IupSetAttributes(IupButton(NULL, NULL), "IMAGE=IUP_MediaForward, FLAT=Yes, TIP=\"Faster\""), "ACTION", speedup_action_cb, NULL),
	NULL)), "NAME=PLAYCONTROLFRAME, EXPAND = HORIZONTAL");
#endif
  vbox = IupVbox(
#ifdef FULLVERSION
    create_toolbar(config),
	IupHbox(canvas,canvaslayout_expander,NULL),
	playcontrol,
#else
	create_item_comport(config),
	IupHbox(canvas,canvaslayout_expander,NULL),
#endif
    create_status_bar(config),
    NULL);

  dlg = IupDialog(vbox);
//  IupSetAttributeHandle(dlg, "MENU", create_main_menu(config));
  IupSetAttribute(dlg, "SIZE", "HALFxHALF");
  IupSetAttribute(dlg, "MINSIZE", "1000x500");
  IupSetCallback(dlg, "CLOSE_CB", (Icallback)item_exit_action_cb);
  IupSetCallback(dlg, "DROPFILES_CB", (Icallback)dropfiles_cb);

#ifdef FULLVERSION
  IupSetCallback(dlg, "K_cN", (Icallback)item_new_action_cb);
  IupSetCallback(dlg, "K_cO", (Icallback)item_open_action_cb);
  IupSetCallback(dlg, "K_cS", (Icallback)item_save_action_cb);
//  IupSetCallback(dlg, "K_cV", (Icallback)item_paste_action_cb);
//  IupSetCallback(dlg, "K_cC", (Icallback)item_copy_action_cb);
//  IupSetCallback(dlg, "K_cP", (Icallback)item_print_action_cb);
  IupSetCallback(dlg, "K_cMinus", (Icallback)zoomout_action_cb);
  IupSetCallback(dlg, "K_cPlus", (Icallback)zoomin_action_cb);
  IupSetCallback(dlg, "K_cEqual", (Icallback)zoomin_action_cb);
  IupSetCallback(dlg, "K_c0", (Icallback)actualsize_action_cb);
  IupSetCallback(dlg, "K_cZ", (Icallback)allnormal_action_cb);
  IupSetCallback(dlg, "K_cX", (Icallback)allexpand_action_cb);
  IupSetCallback(dlg, "K_p", (Icallback)seqplay_action_cb);
  IupSetCallback(dlg, "K_s", (Icallback)seqstop_action_cb);
  IupSetCallback(dlg, "K_d", (Icallback)seqforward_action_cb);
  IupSetCallback(dlg, "K_a", (Icallback)seqrewind_action_cb);
  IupSetCallback(dlg, "K_w", (Icallback)dwellup_action_cb);
  IupSetCallback(dlg, "K_s", (Icallback)dwelldown_action_cb);
  IupSetCallback(dlg, "K_PGUP", (Icallback)layoutspin_down_cb);
  IupSetCallback(dlg, "K_PGDN", (Icallback)layoutspin_up_cb);
  IupSetCallback(dlg, "K_RIGHT", (Icallback)seqforward_action_cb);
  IupSetCallback(dlg, "K_LEFT", (Icallback)seqrewind_action_cb);
  IupSetCallback(dlg, "K_UP", (Icallback)dwellup_action_cb);
  IupSetCallback(dlg, "K_DOWN", (Icallback)dwelldown_action_cb);

  IupSetCallback(dlg, "K_INS", (Icallback)sceneadd_action_cb);
  IupSetCallback(dlg, "K_DEL", (Icallback)scenerm_action_cb);

  enableSeqControl(dlg);
	IupSetAttribute(IupGetDialogChild(dlg, "MACHNOTEXT"), "ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(dlg, "SPEEDFACTORTEXTSHOW"), "ACTIVE", "YES");
	IupSetAttribute(IupGetDialogChild(dlg, "SEQNUMTEXT"), "ACTIVE", "YES");
//	IupSetAttribute(IupGetDialogChild(dlg, "SPEEDFACTORTEXT"), "ACTIVE", "YES");
#endif
  /* parent for pre-defined dialogs in closed functions (IupMessage and IupAlarm) */
  IupSetAttributeHandle(NULL, "PARENTDIALOG", dlg);
  IupSetAttribute(dlg, "ICON", "IUP_ICON");
  IupSetAttribute(dlg, "CONFIG", (char*)config);

  return dlg;
}

void iup_build_main_dlg(int argc, char **argv)
{
  Ihandle *dlg;
  Ihandle *config;

  IupOpen(&argc, &argv);
  IupImageLibOpen();
  load_all_images_ui_toolbox_pic();
//  load_all_images_defimg();

	config = IupConfig();
#ifdef FULLVERSION
	IupSetAttribute(config, "APP_NAME", "matrix_editor");

	char *strTmp;
	mkdir(tmpfolder);
	strTmp = (char *)malloc(strlen("attrib +s +h ") + strlen(tmpfolder) + 1);
	sprintf(strTmp, "attrib +s +h %s", tmpfolder);
	system(strTmp);
	free(strTmp);

#else
	IupSetAttribute(config, "APP_NAME", "matrix_monitor");
#endif
	IupConfigLoad(config);

  dlg = create_main_dialog(config);

  /* show the dialog at the last position, with the last size */
  IupConfigDialogShow(config, dlg, "MainWindow");

  /* open a file from the command line (allow file association in Windows) */
//  if (file_exists("images/defimg.jpg"))
//  {
//    open_file(dlg, "images/defimg.jpg");

  loadSceneImage(dlg);


  /* initialize the current file, if not already loaded */
  check_new_file(dlg);

  IupMainLoop();

  clean_memloc(dlg);
  IupClose();
}
