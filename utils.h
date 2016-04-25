/*
 * utils.h
 *
 *  Created on: 2015. 8. 7.
 *      Author: jeki
 */

#ifndef UTILS_H_
#define UTILS_H_

//standard lib
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

extern const char* tmpfolder;

extern unsigned int getIntHexChar(const char* lineStr,unsigned int byteStart, unsigned int numBytes);
extern unsigned int getIntHexCmpChar(const char* lineStr, const char* cmpChar, unsigned int byteStart, unsigned int numBytes);
extern unsigned char getCheckSumCharArr(const unsigned char *charArray, unsigned int numStart, unsigned int numChar);

extern int sendComPortByte(int comnum, unsigned char c);
extern void comportDisableRTS(int comnum);
extern void comportEnableRTS(int comnum);
extern void sendComPortPuts(int comnum, char *txt);
extern int getComPortPoll(int comnum, unsigned char *buf, int bufsize);
extern unsigned char comPortisAvailable(int cport_nr, int bdrate);
extern void closeComPort(int cport_nr);
extern unsigned char openComPort(int cport_nr, int bdrate);

extern void rename_seq_files_up(unsigned int seqnum_start, unsigned int seqnum_end);
extern void rename_seq_files_down(unsigned int seqnum_start, unsigned int seqnum_end);
extern void remove_seq_file(unsigned int seqnum);
extern void remove_seq_step_files(unsigned int file_max_num);
extern void load_seq_step_file(unsigned int seq_val, char *bptr, unsigned int *delay);
extern void save_seq_step_file(unsigned int seq_val, char *bptr, unsigned int dwell_val);
extern unsigned char file_exists(const char * filename);
extern char *getDatestr(void);
extern double round(double x);
extern const char* str_filetitle(const char *filename);
extern const char* str_fileext(const char *filename);
extern int str_compare(const char *l, const char *r, int casesensitive);
extern char *getFileStrLine(const char *fileName, unsigned int row);
extern char *getTmpFileName(void);
#endif /* UTILS_H_ */
