/*
 * utils.c
 *
 *  Created on: 2015. 8. 7.
 *      Author: jeki
 */

#include "utils.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"

const char* tmpfolder = "~tmp";
const char *tmpfilename = "tmpfile.tmp";

unsigned int getIntHexChar(const char* lineStr,unsigned int byteStart, unsigned int numBytes)
{
	unsigned int numOut = 0, numOutBuf;
	unsigned int Bytes = numBytes * 2;
	char numBuf[2];
	unsigned int i;

	for(i = 0; i < Bytes; i+=2)
	{
		numBuf[0] = lineStr[byteStart + i];
		numBuf[1] = lineStr[byteStart + i + 1];
		sscanf(numBuf,"%02X", &numOutBuf);
		if(i > 0) numOut = numOut << 8;
		numOut += numOutBuf;
	}
	return numOut;
}

unsigned int getIntHexCmpChar(const char* lineStr, const char* cmpChar, unsigned int byteStart, unsigned int numBytes)
{
	char numBuf[2];
	unsigned int Bytes = numBytes * 2;
	size_t len = strlen(cmpChar);
	size_t lenStr = strlen(lineStr);
	size_t lenCmp;
	unsigned int i, j, k;
	unsigned int numOutBuf;
	unsigned char unmatchflag = 0;
	unsigned int numOut = 0;

	if(lenStr > (len   + Bytes))
	{
		lenCmp = lenStr - len - Bytes;
		for(i = byteStart; i<lenCmp; i++)
		{
			if(lineStr[i] == cmpChar[0])
			{
				for(j = 0; j < len; j++)
				{
					if(lineStr[i+j] != cmpChar[j]) {unmatchflag = 1; break;}
				}

				if(!unmatchflag)
				{
					for(k = 0; k < Bytes; k+=2)
					{
						numBuf[0] = lineStr[i+j + k];
						numBuf[1] = lineStr[i+j + k + 1];
						sscanf(numBuf,"%02X", &numOutBuf);
						if(k > 0) numOut = numOut << 8;
						numOut += numOutBuf;
					}
					break;
				}
			}
		}
	}

	return numOut;
}

unsigned char getCheckSumCharArr(const unsigned char *charArray, unsigned int numStart, unsigned int numChar)
{
	unsigned char charOut = 0xff;
	unsigned int i = numStart;
	while (i < numChar)
	{
		charOut^=charArray[i];
		i++;
	}
	return charOut;
}

int sendComPortByte(int comnum, unsigned char byte)
{
	return RS232_SendByte(comnum, byte);
}

void comportEnableRTS(int comnum)
{
	RS232_enableRTS(comnum);
}

void comportDisableRTS(int comnum)
{
	RS232_disableRTS(comnum);
}

void sendComPortPuts(int comnum, char *txt)
{
//	RS232_enableRTS(comnum);
	RS232_cputs(comnum, txt);
//	RS232_disableRTS(comnum);
}

int getComPortPoll(int comnum, unsigned char *buf, int bufsize)
{
//	int i, cnt;
	int n;
	n = RS232_PollComport(comnum, buf, bufsize);

//	for(cnt = 0; cnt <5;cnt++)
//	{
//		if(n > 0)
//		{
//		  buf[n] = 0;   /* always put a "null" at the end of a string! */
//
//		  for(i=0; i < n; i++)
//		  {
//			if(buf[i] < 32)  /* replace unreadable control-codes by dots */
//			{
//			  buf[i] = '.';
//			}
//		  }
//
//		  printf("received %i bytes: %s\n", n, (char *)buf);
//		}
//		Sleep(100);
//	}
	return n;
}

unsigned char comPortisAvailable(int cport_nr, int bdrate)
{
	char mode[]={'8','N','1',0};
//	printf("comport num = %d\n", cport_nr);
//	printf("DSR-pin %d\n", RS232_IsDSREnabled(cport_nr));
//	printf("CTS-pin %d\n", RS232_IsCTSEnabled(cport_nr));
//	printf("DCD-pin %d\n", RS232_IsDCDEnabled(cport_nr));

	if(RS232_OpenComport(cport_nr, bdrate, mode))
	{
//		printf("Can not open comport\n");
		return 0;
	 }
	else
	{
		RS232_CloseComport(cport_nr);
//		printf("comport can be opened\n");
		return 1;
	}
}

void closeComPort(int cport_nr)
{
	RS232_CloseComport(cport_nr);
}

unsigned char openComPort(int cport_nr, int bdrate)
{
	char mode[]={'8','N','1',0};
	if(RS232_OpenComport(cport_nr, bdrate, mode))
	{
//		printf("Can not open comport\n");
		return 0;
	 }
	return 1;
}

unsigned char file_exists(const char * filename)
{
	FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void rename_seq_files_up(unsigned int seqnum_start, unsigned int seqnum_end)
{
	char file_name_old[20];
	char file_name_new[20];
	unsigned int i = seqnum_end;

	sprintf(file_name_old,"%s/seq_%d", tmpfolder, i);
	sprintf(file_name_new,"%s/seq_%d", tmpfolder, i+1);
	rename( file_name_old , file_name_new );
//	if ( result == 0 )
//		puts ( "File successfully renamed" );
//	else
//		perror( "Error renaming file" );

	while(i > seqnum_start)
	{
		i--;
		sprintf(file_name_old,"%s/seq_%d", tmpfolder, i);
		sprintf(file_name_new,"%s/seq_%d", tmpfolder, i+1);
		rename( file_name_old , file_name_new );
	}
}

void rename_seq_files_down(unsigned int seqnum_start, unsigned int seqnum_end)
{
	char file_name_old[20];
	char file_name_new[20];
	unsigned int i = seqnum_start;

	sprintf(file_name_old,"%s/seq_%d", tmpfolder, i);
	sprintf(file_name_new,"%s/seq_%d", tmpfolder, i-1);
	rename( file_name_old , file_name_new );
//	if ( result == 0 )
//		puts ( "File successfully renamed" );
//	else
//		perror( "Error renaming file" );

	while(i < seqnum_end)
	{
		i++;
		sprintf(file_name_old,"%s/seq_%d", tmpfolder, i);
		sprintf(file_name_new,"%s/seq_%d", tmpfolder, i-1);
		rename( file_name_old , file_name_new );
	}
}

void remove_seq_file(unsigned int seqnum)
{
	char file_name[15];
//	int status;
	sprintf(file_name,"%s/seq_%d", tmpfolder, seqnum);
	if(file_exists(file_name))
	{
//		status =
				remove(file_name);

//		if( status == 0 )
//		  printf("%s file deleted successfully.\n",file_name);
//		else
//		{
//		  printf("Unable to delete the file\n");
//		  perror("Error");
//		}
	}
}

void remove_seq_step_files(unsigned int file_max_num)
{
	unsigned int i;
//	int status;
	char file_name[15];

	for(i=0;i<file_max_num;i++)
	{
		sprintf(file_name,"%s/seq_%d", tmpfolder, i);
		if(file_exists(file_name))
		{
//			status =
					remove(file_name);
//
//			if( status == 0 )
//			  printf("%s file deleted successfully.\n",file_name);
//			else
//			{
//			  printf("Unable to delete the file\n");
//			  perror("Error");
//			}
		}
	}
}

void load_seq_step_file(unsigned int seq_val, char *bptr, unsigned int *delay)
{
	int i;
	char seqfile[15];
	char c;

	sprintf(seqfile,"%s/seq_%d", tmpfolder, seq_val);

	if(file_exists(seqfile))
	{
		FILE *pFile = fopen (seqfile,"rb");
		if (pFile!=NULL)
		{
			for (i=0;i<64;i++)
			{
				c = fgetc(pFile);
				bptr[i] = c-1;
			}
			fscanf(pFile,"%u", &(*delay));
			fclose (pFile);
		}
	}
	else
		*delay = 0;
}

void save_seq_step_file(unsigned int seq_val, char *bptr, unsigned int dwell_val)
{
	int i;
	char seqfile[15];

	sprintf(seqfile,"%s/seq_%d", tmpfolder, seq_val);

	FILE *pFile = fopen (seqfile,"wb");
	if (pFile!=NULL)
	{
		for (i=0;i<64;i++)
			fputc (bptr[i]+1 , pFile );
		fprintf(pFile,"%u", dwell_val);
		fclose (pFile);
	}
}

double round(double x)
{
  return (int)(x>0 ? x + 0.5 : x - 0.5);
}

const char* str_filetitle(const char *filename)
{
  /* Start at the last character */
  int len = (int)strlen(filename);
  int offset = len - 1;
  while (offset != 0)
  {
    if (filename[offset] == '\\' || filename[offset] == '/')
    {
      offset++;
      break;
    }
    offset--;
  }

  return filename + offset;
}

const char* str_fileext(const char *filename)
{
  /* Start at the last character */
  int len = (int)strlen(filename);
  int offset = len - 1;
  while (offset != 0)
  {
    if (filename[offset] == '\\' || filename[offset] == '/')
      break;

    if (filename[offset] == '.')
    {
      offset++;
      return filename + offset;
    }
    offset--;
  }
  return NULL;
}

int str_compare(const char *l, const char *r, int casesensitive)
{
  if (!l || !r)
    return 0;

  while (*l && *r)
  {
    int diff;
    char l_char = *l,
      r_char = *r;

    /* compute the difference of both characters */
    if (casesensitive)
      diff = l_char - r_char;
    else
      diff = tolower((int)l_char) - tolower((int)r_char);

    /* if they differ we have a result */
    if (diff != 0)
      return 0;

    /* otherwise process the next characters */
    ++l;
    ++r;
  }

  /* check also for terminator */
  if (*l == *r)
    return 1;

  if (*r == 0)
    return 1;  /* if second string is at terminator, then it is partially equal */

  return 0;
}

char *ulongToStr(unsigned long _num)
{
	char *retval=(char *)malloc(11);
	sprintf(retval,"%lu",_num);
	return retval;
}

char *uintToStr(unsigned int num)
{
	char *retval=(char *)malloc(6);
	sprintf(retval,"%u",num);
	return retval;
}

char *ucharToStr(unsigned char num)
{
	char *retval=(char *)malloc(4);
	sprintf(retval,"%u",(unsigned int)num);
	return retval;
}

char *cmdView(char *cmdpath, char *strFile)
{
	char *cmdprefix = "start \"\" ";
	char *strcmd = NULL;
	size_t len;
	unsigned char cnt = 0;

	len = strlen(cmdpath) + strlen(cmdprefix); cnt+=2;
	if(strFile != NULL) { len += strlen(strFile); cnt+=5;}

	strcmd = malloc(len+(size_t)cnt);

	strncpy(strcmd,cmdprefix,strlen(cmdprefix) + 1);
	strcat(strcmd,"\""); strcat(strcmd,cmdpath); strcat(strcmd,"\"");
	strcat(strcmd," ");
	if(strFile != NULL)
	{
		strcat(strcmd,"\"");
		strcat(strcmd,strFile);
		strcat(strcmd,"\"");
	}

	free(cmdpath); free(strFile);
	return strcmd;
}

char *cmdcat(char *cmdpath, char *strFin, char *strFout, char *strFtbl, char *strAddr)
{
	char *strcmd = NULL;
	size_t len;
	unsigned char cnt = 0;

	len = strlen(cmdpath); cnt++;
	if(strFin != NULL) { len += strlen(strFin); cnt+=3;}
	if(strFout != NULL) { len += strlen(strFout); cnt+=3;}
	if(strFtbl != NULL) { len += strlen(strFtbl); cnt+=3;}
	if(strAddr != NULL) { len += strlen(strAddr); cnt++;}

	strcmd = malloc(len+(size_t)cnt);

	strncpy(strcmd,cmdpath,strlen(cmdpath) + 1); strcat(strcmd," ");
	if(strFin != NULL) {strcat(strcmd,"\""); strcat(strcmd,strFin); strcat(strcmd,"\""); strcat(strcmd," ");}
	if(strFout != NULL) {strcat(strcmd,"\""); strcat(strcmd,strFout);strcat(strcmd,"\""); strcat(strcmd," ");}
	if(strFtbl != NULL) {strcat(strcmd,"\""); strcat(strcmd,strFtbl);strcat(strcmd,"\""); strcat(strcmd," ");}
	if(strAddr != NULL) strcat(strcmd,strAddr);

	free(cmdpath); free(strFin); free(strFout); free(strFtbl); free(strAddr);
	return strcmd;
}

char *getDateTimeNow(void)
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	return asctime (timeinfo);
}

char *getDatestr(void)
{
	time_t rawtime;
	struct tm * timeinfo;
	char *buffer = (char *)malloc(13);

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	strftime (buffer,13,"%y%m%d%H%M%S",timeinfo);

	return buffer;
}

char * extract_filetype(char *str)
{
    int     ch = '.';
    size_t  len;
    char   *pdest;
    char   *inpfile = NULL;

    // Search backwards for last backslash in filepath
    pdest = strrchr(str, ch);

    // if backslash not found in filepath
    if(pdest == NULL )
    {
//    	printf( "Result:\t%c not found\n", ch );
    	pdest = str;  // The whole name is a file in current path?
    }
    else
    {
    	pdest++; // Skip the backslash itself.
    }

    // extract filename from file path
    len = strlen(pdest);
    inpfile = malloc(len+1);  // Make space for the zero.
    strncpy(inpfile, pdest, len+1);  // Copy including zero.
    return inpfile;
}

char * extract_filename(char *str)
{
    int     ch = '\\';
    size_t  len;
    char   *pdest;
    char   *inpfile = NULL;

    if(str!=NULL)
    {
		// Search backwards for last backslash in filepath
		pdest = strrchr(str, ch);

		// if backslash not found in filepath
		if(pdest == NULL )
		{
//			printf( "Result:\t%c not found\n", ch );
			pdest = str;  // The whole name is a file in current path?
		}
		else
		{
			pdest++; // Skip the backslash itself.
		}

		// extract filename from file path
		len = strlen(pdest);
		inpfile = malloc(len+1);  // Make space for the zero.
		strncpy(inpfile, pdest, len+1);  // Copy including zero.
    }
    return inpfile;
}

char *extract_dir(char *str)
{
    int     ch = '\\';
    size_t  dlen,lenstr,lenpdest;

    char   *filestr = NULL;
    char   *pdest = NULL;
    char   *inpfile = NULL;

    lenstr = strlen(str);
    filestr = (char *) malloc(lenstr + 1);
    memcpy(filestr,str,lenstr + 1);
    if(filestr!=NULL)
    {
		// Search backwards for last backslash in filepath
		pdest = strrchr(filestr, ch);

		// if backslash not found in filepath
		if(pdest == NULL )
		{
//			printf( "Result:\t%c not found\n", ch );
			pdest = filestr;  // The whole name is a file in current path?
		}
		else
		{
			pdest++; // Skip the backslash itself.
		}

		// extract filename from file path
		lenpdest = strlen(pdest);
		dlen = lenstr - lenpdest;
		inpfile = malloc(dlen);  // Make space for the zero.
		strncpy(inpfile, filestr, dlen);  // Copy including zero.
		free(pdest);free(filestr);
    }
    return inpfile;
}

char *getFileParam(char *fileName, unsigned int row, unsigned char col)
{
	char *retval=NULL;
	FILE * pFile;
	char *buffer;
	char *pch;

	size_t slen;
	unsigned long fileLen;
	unsigned int rowcnt=0;
	unsigned char colcnt = 0;
	unsigned char breakflag = 0;


	pFile = fopen (fileName , "r");
	if (pFile == NULL) perror ("Error opening file");
	else
	{
		//Get file length
		fseek(pFile, 0, SEEK_END);
		fileLen=ftell(pFile);
		fseek(pFile, 0, SEEK_SET);

		//Allocate memory
		buffer=(char *)malloc(fileLen+1);

		//set pointer back to the start of the file
		rewind(pFile);
		while(!breakflag)
		{
			if ( fgets (buffer , fileLen , pFile) != NULL )
			{
				breakflag = 0;
				rowcnt++;
				if(rowcnt > row)
				{
					pch = strtok (buffer," \t");
					while (pch != NULL)
					{
						colcnt++;
						if(colcnt > col)
						{
							slen = strlen(pch);
							retval = (char *)malloc(slen+1);
							memcpy(retval,pch,slen+1);
							break;
						}
						pch = strtok (NULL," \t");
					}
					breakflag = 1;
				}
			}
			else
			{
				if(rowcnt < row) perror ("file reach EOF");
				breakflag = 1;
			}
		}
		free(buffer);
		fclose (pFile);
	}
	return retval;
}

char *getFileStrLine(const char *fileName, unsigned int row)
{
	char *retval=NULL;
	FILE * pFile;
	char *buffer;
	char *pch;

	size_t slen = 0;
	unsigned long fileLen;
	unsigned int rowcnt=0;
	unsigned char breakflag = 0;


	pFile = fopen (fileName , "r");
	if (pFile == NULL) perror ("Error opening file");
	else
	{
		//Get file length
		fseek(pFile, 0, SEEK_END);
		fileLen=ftell(pFile);
		fseek(pFile, 0, SEEK_SET);

		//Allocate memory
		buffer=(char *)malloc(fileLen+1);

		//set pointer back to the start of the file
		rewind(pFile);
		while(!breakflag)
		{
			if ( fgets (buffer , fileLen , pFile) != NULL )
			{
				breakflag = 0;
				rowcnt++;
				if(rowcnt > row)
				{
					slen = strlen(buffer);
					retval = (char *)malloc(slen+1);
					memcpy(retval,buffer,slen+1);
					breakflag = 1;
				}
			}
			else
			{
				if(rowcnt < row) perror ("file reach EOF");
				breakflag = 1;
			}
		}
		free(buffer);
		fclose (pFile);
	}
	if(retval != NULL)
	{
		pch = strstr (retval,"\n");
		strncpy (pch,"",1);
	}
	return retval;
}

char *getTmpFileName(void)
{
	size_t strFileLen = strlen(tmpfolder) + strlen(tmpfilename) + 4;
	char *strFileName = NULL;

	strFileName = (char*)malloc(strFileLen);
	sprintf(strFileName,"%s/%s",tmpfolder, tmpfilename);
	return strFileName;
}

unsigned int getFileRowLen(char *fileName)
{
	unsigned int retval=0;
	FILE * pFile;
	char c;


	pFile = fopen (fileName , "r");
	if (pFile==NULL) perror ("Error opening file");
	else
	{
		do {
			c = fgetc (pFile);
			if (c == '\n') retval++;
		} while (c != EOF);
		fclose (pFile);
	}

	if(retval > 0) retval++;
	return retval;
}

unsigned int strArrayNum(const char *str, char *delim)
{
	char *buf=NULL;
	char *strArr;
	size_t len;
	unsigned int retval=0;

	len = strlen(str);
	buf=(char *)malloc(len + 1);
	memcpy(buf,str,len+1);

	strArr = strtok (buf,delim);
	while (strArr != NULL)
	{
		retval++;
		strArr = strtok (NULL,delim);
	}
	free(buf);free(strArr);
	return retval;
}

char *strArrayStr(const char *str, char *delim, unsigned int _ptr)
{
	char *buf=NULL;
	char *strArr;
	size_t len;
	unsigned int retval=0;

	len = strlen(str);
	buf=(char *)malloc(len + 1);
	memcpy(buf,str,len+1);

	strArr = strtok (buf,delim);
	while (strArr != NULL)
	{
		retval++;
		if(retval > _ptr) break;
		strArr = strtok (NULL,delim);
	}
	return strArr;
}
