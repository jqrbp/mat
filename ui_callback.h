/*
 * ui_callback.h
 *
 *  Created on: 2015. 8. 6.
 *      Author: jeki
 */

#ifndef UI_CALLBACK_H_
#define UI_CALLBACK_H_

#include <iup.h>

extern int item_test_action_cb(Ihandle *ih);
extern int item_comporttxtlist_dropped_cb(Ihandle* comporttxtlist);
extern int item_comporttxtlist_valuechanged_cb(Ihandle* comporttxtlist);
extern int item_comporttxtlist_action_cb(Ihandle *comporttxtlist, char *text, int item, int state);
extern int item_sendcomport_action_cb(Ihandle *ih);
extern int item_autocomport_action_cb(Ihandle *ih);
extern int comport_timersearch_action_cb(Ihandle *ih);
extern int comport_timer_action_cb(Ihandle *ih);
extern int comportlist_action_cb(Ihandle *comportlist, char *text, int item, int state);
extern int comport_valuechanged_cb(Ihandle *ih);
extern int comport_dropped_cb(Ihandle *ih, int state);
extern int comport_connect_click_cb(Ihandle* item_comport, int button, int pressed, int x, int y, char* status);
extern int comport_monitor_click_cb(Ihandle* ih, int button, int pressed, int x, int y, char* status);
extern int comport_sendseq_click_cb(Ihandle* ih, int button, int pressed, int x, int y, char* status);
extern int comport_read_click_cb(Ihandle* ih, int button, int pressed, int x, int y, char* status);

extern int speedfactorspin_action_cb(Ihandle *ih, int inc);
extern int speedfactorshow_valuechanged_cb(Ihandle *ih, int c);
extern int speedfactor_valuechanged_cb(Ihandle *ih, int c);
extern int machno_valuechanged_cb(Ihandle *ih, int c);
extern int dwelldown_action_cb(Ihandle *ih);
extern int dwellup_action_cb(Ihandle *ih);
extern int dwellspin_action_cb(Ihandle* ih, int inc);
extern int dwell_valuechanged_cb(Ihandle *ih, int c);
//extern int speedup_action_cb(Ihandle *ih);
//extern int speeddown_action_cb(Ihandle *ih);

extern int layoutspin_up_cb(Ihandle *ih);
extern int layoutspin_down_cb(Ihandle *ih);
extern int layoutspin_action_cb(Ihandle *ih, int inc);

extern int sceneadd_action_cb(Ihandle *ih);
extern int scenerm_action_cb(Ihandle *ih);

extern int seqrewind_action_cb(Ihandle *ih);
extern int seqforward_action_cb(Ihandle *ih);
extern int seq_valuechanged_cb(Ihandle *ih);
extern int seqnum_action_cb(Ihandle *ih, int c);
extern int seqjump_action_cb(Ihandle *ih, int c);
extern int seqplay_action_cb(Ihandle* ih, int state);
extern int seqtimer_action_cb(Ihandle *ih);
extern int seqstop_action_cb(Ihandle *ih);

extern int canvaslayout_action_cb(Ihandle* canvaslayout);
extern int canvaslayout_openclose_cb(Ihandle* ih, int state);
extern int canvaslayout_map_cb(Ihandle* canvaslayout);
extern int canvaslayout_unmap_cb(Ihandle* canvaslayout);
extern int canvaslayout_click_cb(Ihandle* canvas, int button, int pressed, int x, int y, char* status);
extern int canvaslayout_wheel_cb(Ihandle* canvaslayout, float delta);

extern int canvas_motion_cb(Ihandle* canvas, int x, int y, char *status);
extern int canvas_click_cb(Ihandle* canvas, int button, int pressed, int x, int y, char* status);
extern int canvas_action_cb(Ihandle* canvas);
extern int canvas_map_cb(Ihandle* canvas);
extern int canvas_unmap_cb(Ihandle* canvas);
extern int canvas_resize_cb(Ihandle* canvas);
extern int canvas_wheel_cb(Ihandle* canvas, float delta);
extern int canvas_key_action_cb(Ihandle* canvas, int c);

extern int zoomout_action_cb(Ihandle* ih);
extern int zoomin_action_cb(Ihandle* ih);
extern int allnormal_action_cb(Ihandle *ih);
extern int allexpand_action_cb(Ihandle *ih);
extern int actualsize_action_cb(Ihandle* ih);
extern int zoom_valuechanged_cb(Ihandle* val);

extern int dropfiles_cb(Ihandle* ih, const char* filename);
extern int file_menu_open_cb(Ihandle* ih);
extern int edit_menu_open_cb(Ihandle* ih);
extern int config_recent_cb(Ihandle* ih);

extern int item_new_action_cb(Ihandle* item_new);
extern int item_open_action_cb(Ihandle* item_open);
extern int item_save_action_cb(Ihandle* item_save);
extern int item_saveas_action_cb(Ihandle* item_save);
extern int item_revert_action_cb(Ihandle* item_revert);
extern int item_pagesetup_action_cb(Ihandle* item_pagesetup);
extern int item_print_action_cb(Ihandle* item_print);
extern int item_exit_action_cb(Ihandle* item_exit);
extern int item_copy_action_cb(Ihandle* item_copy);
extern int item_paste_action_cb(Ihandle* item_paste);
extern int item_background_action_cb(Ihandle* item_background);
extern int item_toolbar_action_cb(Ihandle* item_toolbar);
extern int item_statusbar_action_cb(Ihandle* item_statusbar);
extern int item_help_action_cb(void);
extern int item_about_action_cb(void);

#endif /* UI_CALLBACK_H_ */
