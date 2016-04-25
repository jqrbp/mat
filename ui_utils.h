/*
 * ui_utils.h
 *
 *  Created on: 2015. 8. 19.
 *      Author: jeki
 */

#ifndef UI_UTILS_H_
#define UI_UTILS_H_

#include <iup.h>
#include <iup_config.h>
#include <iupgl.h>
#include <cd.h>
#include <cdprint.h>
#include <cdiup.h>
#include <cdirgb.h>
#include <im.h>
#include <im_image.h>
#include <im_convert.h>
#include <im_process_ana.h>
#include <im_process_glo.h>
#include <im_process_loc.h>
#include <iupim.h>

#define IMGBUFWIDTH	 480
#define IMGBUFHEIGHT 360

#define ACKMACHINENO 		1
#define ACKLAYNUM			2
#define ACKSEQNUM			3
#define ACKSPEEDFACTOR		4
#define ACKSCENE			5
#define ACKSCENEMAP			6
#define ACKWRITEEN			7

#define COMPORTMONITOR		1
#define COMPORTSENDSEQ		2
#define COMPORTREADFLASH	3

#define MAXSCENENUM			4090
//#define FULLVERSION			1

typedef enum {normal, mirror, rotate90cw, rotate90ccw, rotate180, flip} uimgflag;

//typedef struct _unit_param
//{
//	unsigned char unum; //unit no
//	int ux;             //unit position x
//	int uy;             //unit position y
//	int uw;             //unit width
//	int uh;             //unit height
//	int bw;             //unit block width
//	int bh;             //unit block height
//	imImage *uimg;      //unit image
//	uimgflag uimgf;    //unit image process flag (normal, mirror, rotate, flip)
//} unit_param;
//
//typedef struct _layout_param
//{
//	unsigned char lnum; 		//layout no.
//	int	loffx;					// layout offset x
//	int loffy;          		//layout offset y
//	int lw;             		// layout width
//	int lh;             		//layout height
//	unit_param *up1;    		//unit 1 parameters
//	unit_param *up2;    		//unit 2 parameters
//	unit_param *up3;    		//unit 3 parameters
//	unit_param *up4;    		//unit 4 paramters
//	unsigned char bptr[64];     //grid toggle pointer
//} layout_param;

extern void layout_update(Ihandle *ih, int laytxt_val);
extern void drawPropertiesShow(Ihandle *canvas);
extern void showMsgDlg(Ihandle *parent_dlg, char *msgtype, char *err_title, char *err_msg);
extern void checkComportBuf(Ihandle *ih, int comnum, int n, unsigned char *buf, int detectcnt);
extern void setDwellVal(Ihandle *parent_dlg, unsigned int dwell_input_val);
extern void setSpeedFactorVal(Ihandle *parent_dlg, unsigned int speedfactortxtshow_val);
extern void disableSeqControl(Ihandle *ih);
extern void disableComportSeqControl(Ihandle *ih);
extern void enableComportControl(Ihandle *ih);
extern void disableComportControl(Ihandle *ih);
extern void enableComportSeqControl(Ihandle *ih);
extern void enableSeqControl(Ihandle *ih);
extern void clean_memloc(Ihandle *ih);
extern void load_seq_step(Ihandle *ih, unsigned int seq_val);
extern void save_seq_step(Ihandle *ih, unsigned int seq_val);
extern void unitParamInit(unsigned char unum, int ux, int uy, int uw, int uh, int bw, int bh, imImage *uimg, uimgflag uimgf);
extern int getUnitimFlag(Ihandle *canvas, unsigned char unum);
extern void setUnitParam(Ihandle* canvas, unsigned char unum, int *ux, int *uy, int *uw, int *uh, int *bw, int *bh);
extern void setBlockImage(Ihandle *canvas, unsigned char unum, imImage **imgb);
extern void setUnitImage(Ihandle *canvas, unsigned char unum, imImage **imgb);
extern void expandBlock(cdCanvas* cd_canvas, imImage *imgb, imImage *imgs,
		int bcntw, int bcnth, int xn, int yn, int bw, int bh,int ux, int uy);
extern void normalBlock(cdCanvas* cd_canvas, imImage *imgb,
		int xn, int yn, int bw, int bh,int ux, int uy);
extern void clearUnit(Ihandle *canvas, unsigned char unum);
extern void drawUnitGridImage(cdCanvas* cd_canvas, imImage *imgu, imImage *imgb, unsigned char unum, int imflag,
		int ucnt, int bcntw, int bcnth, int ux, int uy, int uw, int uh, int bw, int bh, char *bptr);
extern void drawUnitBlock(Ihandle *canvas, unsigned char unum, int xn, int yn, unsigned char bnum, unsigned char is_clear);
extern void getBlockNumCanvas(Ihandle *canvas, unsigned char unum, int xn, int yn, unsigned char *bnum);
extern void getUnitPixel(Ihandle *canvas, int x, int y, unsigned char *unum, int *xn, int *yn);
extern void setUnitLayoutSize(Ihandle *canvas);
extern void setUnitLayoutImages(Ihandle *canvas);
extern void show_error(const char* message, int is_error);
extern void show_file_error(int error);
extern imImage* read_file(const char* filename);
extern int write_file(const char* filename, const imImage* image);
/* extracted from the SCROLLBAR attribute documentation */
extern void scroll_update(Ihandle* ih, int view_width, int view_height);
extern void scroll_calc_center(Ihandle* ih, float *x, float *y);
extern void scroll_center(Ihandle* ih, float old_center_x, float old_center_y);
extern void seq_update(Ihandle* ih, unsigned int seq_val);
extern void zoom_update(Ihandle* ih, double zoom_index);
extern void set_new_image(Ihandle* canvas, imImage* image, const char* filename, int dirty);
extern void check_new_file(Ihandle* dlg);
extern void open_file(Ihandle* ih, const char* filename);
extern void save_file(Ihandle* canvas);
extern void set_file_format(imImage* image, const char* filename);
extern void saveas_file(Ihandle* canvas, const char* filename);
extern int save_check(Ihandle* ih);
extern int select_file(Ihandle* parent_dlg, int is_open);
extern int select_file_isf(Ihandle* parent_dlg, unsigned char is_open);
extern void save_file_seq(Ihandle* ih, const char* filename);
extern void view_fit_rect(int canvas_width, int canvas_height, int image_width, int image_height, int *view_width, int *view_height);

extern void updateComportTxtList(Ihandle* comlist, char* txt);
extern void saveComPortConfig(Ihandle *ih);
extern void sendComportCanvasState(Ihandle *canvas);

extern void loadSceneImage(Ihandle *dialog);
#endif /* UI_UTILS_H_ */
