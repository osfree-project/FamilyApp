#include <stdio.h>
#include <string.h>

#define INCL_DOSMISC
#include <cmd_shared.h> /* comand line tools' shared functions and defines */

#define INCL_VDM
#include <os2vdm.h> /* system header */

#include <dos.h>
#include <string.h>

// @todo extend bseerr
#define MSG_BAD_SYNTAX 1003
#define MSG_SUBST_LIST 1105

/*
Under  OS/2 VDM we use its JOIN/SUBST logic. 

Under Windows 95+ VDM we use following
2171AABH00 INT 21 - Windows95 - LONG FILENAME - CREATE SUBST
2171AABH01 INT 21 - Windows95 - LONG FILENAME - TERMINATE SUBST
2171AABH02 INT 21 - Windows95 - LONG FILENAME - QUERY SUBST

Under pure DOS we use classic

Category: DOS kernel

INT 21 - Windows95 - LONG FILENAME - CREATE SUBST

	AX = 71AAh
	BH = 00h
	BL = drive number (00h = default, 01h = A:, etc.)
	DS:DX -> ASCIZ pathname to associate with drive letter
Return: CF clear if successful
	CF set on error
	    AX = error code (see #01680)
		7100h if function not supported
SeeAlso: AX=71AAh/BH=01h,AX=71AAh/BH=02h,INT 2F/AX=1000h,#01643
 */

APIRET APIENTRY LfnSubstCreate(CHAR cDrive, PSZ pszPath)
{
  union REGPACK rp;

  rp.w.ax=0x71AA;
  rp.h.bh=0x00;        // create
  rp.h.bl=cDrive - 'A';
  rp.w.ds=FP_SEG(pszPath);
  rp.w.dx=FP_OFF(pszPath);

  intr(0x21, &rp);

  return rp.x.ax; // error
}

/*
Category: DOS kernel

INT 21 - Windows95 - LONG FILENAME - TERMINATE SUBST

	AX = 71AAh
	BH = 01h
	BL = drive number (01h = A:, etc.)
Return: CF clear if successful
	CF set on error
	    AX = error code (see #01680)
		7100h if function not supported
Note:	the specified drive number may not be 00h (default), and presumably not
	  the current drive either
SeeAlso: AX=71AAh/BH=00h,AX=71AAh/BH=02h,INT 2F/AX=1000h,#01643

Category: DOS kernel

*/

APIRET APIENTRY LfnSubstTerminate(CHAR cDrive)
{
  union REGPACK rp;

  rp.w.ax=0x71AA;
  rp.h.bh=0x01;        // queterminatery
  rp.h.bl=cDrive - 'A';

  intr(0x21, &rp);

  return rp.x.ax; // error
}

/*
INT 21 - Windows95 - LONG FILENAME - QUERY SUBST

	AX = 71AAh
	BH = 02h
	BL = drive number (01h = A:, etc.)
	DS:DX -> buffer for ASCIZ pathname associated with drive letter
Return: CF clear if successful
	    DS:DX buffer filled
	CF set on error
	    AX = error code (see #01680)
		7100h if function not supported
Note:	the specified drive number may not be 00h (default drive)
SeeAlso: AX=71AAh/BH=00h,AX=71AAh/BH=01h,INT 2F/AX=1000h,#01643

*/

APIRET APIENTRY LfnSubstQuery(CHAR cDrive, PSZ pszPath)
{
  union REGPACK rp;

  rp.w.ax=0x71AA;
  rp.h.bh=0x02;        // query
  rp.h.bl=cDrive - 'A';
  rp.w.ds=FP_SEG(pszPath);
  rp.w.dx=FP_OFF(pszPath);

  intr(0x21, &rp);

  return rp.x.ax; // error
}

APIRET syntax(void)
{
	cmd_ShowSystemMessage(cmd_MSG_DISKCOPY_HELP,0L);
	return 0;
}

void List()
{
        unsigned        drive;                  /* 0-based drive number */
        char            driveLet[3];            /* Drive letter e.g. D: */
        unsigned        msgLen;                 /* Length of message */
        char far        *varPtr[2];             /* Pointers to variable text */
        char            path[260];               /* Buffer where directory */
                                                /* of a join will be placed */

        driveLet[1] = ':';      /* Init. constant part of driveLet */
        driveLet[2] = '\0';

        for (drive = 0; drive < 26; drive++)
        {
		if (lfn) 
		{
			LfnSubstQuery(drive, (char far *) path);
		} else {
	                VdmSubstQuery(drive, 260, path);
//			@todo: Pure DOS subst
		}

                if (path[0])
                {
                        driveLet[0] = drive + 'A';

                        varPtr[0] = (char far *) driveLet;
                        varPtr[1] = (char far *) path;

/*                        DosGetMessage (
                                (char far * far *) varPtr,
                                2,
                                (char far *) msgBuf,
                                SIZE_OF_MSGBUF,
                                ,
                                MSGFILE,
                                (unsigned far *) &msgLen);
                        DosPutMessage (STDOUT, msgLen, (char far *) msgBuf);*/
			cmd_ShowSystemMessage(MSG_SUBST_LIST, 0);
                }
        }
}

#define PROTECTMODE 1
//**************************************************************************
// Start of program
//**************************************************************************
int main (int argc, char* argv[], char* envp[])
{
	int argi;
	int iteration;
	char drive[3] = {0};
	char path[256] = {0};
	char mode;

	DosGetMachineMode((char far *)&mode);
	if (mode == PROTECTMODE)
	{
        	if ((argc == 1) && ((argv[0][0] == '/') || (argv[0][0] == '-'))
	                        && ((argv[0][1] == '?') || (argv[0][1] == 'h') || (argv[0][1] == 'H'))
	                        && (!argv[0][2]) )
	        {
			syntax();
		}
		cmd_ShowSystemMessage(MSG_REAL_MODE_ONLY,0L);
		exit(0);
	}

	for (argi = 1; argi < argc; argi++)
	{
		if ((argv[argi][0] == '-') || (argv[argi][0] == '/'))
		{
			switch (argv[argi][1])
			{
			case 'h':
			case 'H':
			case '?':
				return syntax();
			default:
				cmd_ShowSystemMessage(MSG_BAD_SYNTAX,0L);
				return 0;//NO_ERROR;
			}
		} else {
			iteration++;
			if (iteration==1)
			{
				strncpy(drive, argv[argi], sizeof(drive));
			} else if (iteration==2)
			{
				strncpy(path, argv[argi], sizeof(path));
				VdmSubstSet(drive[0], path);
				return 0;
			}
		}
	}
	return syntax();
}
