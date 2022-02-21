#define INCL_BASE
#include <os2.h>
#include "newexe.h"

struct EXE_RELOC {
  unsigned short offset;
  unsigned short segment;
};

char hello[]                  ="\n\rosFree Segmented-EXE Header Utility Version 1.0\n\rCopyright (C) osFree contributors 2021.";
char fileInfo[]               ="\n\r    File Information     (in Hex)";
char magicNumber[]            ="\n\rMagic number:             ";
char bytesInLastPage[]        ="\n\rBytes in last page:       ";
char pagesInFile[]            ="\n\rPages in file:            ";
char relocations[]            ="\n\rRelocations:              ";
char paragraphsInHeader[]     ="\n\rParagraphs in header:     ";
char extraParagraphsNeeded[]  ="\n\rExtra paragraphs needed:  ";
char extraParagraphsWanted[]  ="\n\rExtra paragraphs wanted:  ";
char initialStackLocation[]   ="\n\rInitial stack location:   ";
char wordChecksum[]           ="\n\rWord checksum:            ";
char entryPoint[]             ="\n\rEntry point:              ";
char relocationTableAddress[] ="\n\rRelocation table address: ";
char overlayNumber[]          ="\n\rOverlay number:           ";
char reservedWords[]          ="\n\rReserved words:\n\r";
char newEXEHeaderAddress[]    ="\n\rNew .EXE header address:  ";
//  char memoryNeeded[]           ="\n\rMemory needed:            ";
 
BOOL is_extended_exe(struct exe_hdr hdr)
{
  if (hdr.e_lfanew!=0) return TRUE;
// todo: add check for NE, PE, LX, LE, P3, P4... Surf for more format signatures
  return FALSE;
}

BOOL is_NE(struct new_exe hdr)
{
  return TRUE;
}

void printHex(USHORT ptr)
{
  int i;
  char tmp[2];
  char buf[2];

  buf[0] = ptr & 0xFF;
  buf[1] = (ptr >> 8) & 0xFF;

  for (i = sizeof(ptr) - 1; i >= 0; i--) {
    tmp[0] = (buf[i] >> 4) & 0xf; 
    tmp[1] = buf[i] & 0xf;        

    tmp[0] += tmp[0] < 10 ? '0' : 'a' - 10;
    tmp[1] += tmp[1] < 10 ? '0' : 'a' - 10;

    VioWrtTTY(tmp, 2, 0);
  }
}


void DumpMZReloc(struct EXE_RELOC far *r, USHORT crlc)
{
  int i;

  char relocationTable[]    ="\n\rRelocation table:";

  VioWrtTTY(relocationTable, sizeof(relocationTable), 0);
  for (i = 0; i < crlc; i++) {
    VioWrtTTY("\n\r", 2, 0);
    printHex(r[i].segment);
    VioWrtTTY(":", 1, 0);
    printHex(r[i].offset);
  }
  
}

void DumpMZ(struct exe_hdr hdr)
{
    int i;

    VioWrtTTY(hello, sizeof(hello), 0);
    VioWrtTTY(fileInfo, sizeof(fileInfo), 0);
    VioWrtTTY(magicNumber, sizeof(magicNumber), 0);
    printHex(hdr.e_magic);
    VioWrtTTY(bytesInLastPage, sizeof(bytesInLastPage), 0);
    printHex(hdr.e_cblp);
    VioWrtTTY(pagesInFile, sizeof(pagesInFile), 0);
    printHex(hdr.e_cp);
    VioWrtTTY(relocations, sizeof(relocations), 0);
    printHex(hdr.e_crlc);
    VioWrtTTY(paragraphsInHeader, sizeof(paragraphsInHeader), 0);
    printHex(hdr.e_cparhdr);
    VioWrtTTY(extraParagraphsNeeded, sizeof(extraParagraphsNeeded), 0);
    printHex(hdr.e_minalloc);
    VioWrtTTY(extraParagraphsWanted, sizeof(extraParagraphsWanted), 0);
    printHex(hdr.e_maxalloc);
    VioWrtTTY(initialStackLocation, sizeof(initialStackLocation), 0);
    printHex(hdr.e_ss);
    VioWrtTTY(":", 1, 0);
    printHex(hdr.e_sp);
    VioWrtTTY(wordChecksum, sizeof(wordChecksum), 0);
    printHex(hdr.e_csum);
    VioWrtTTY(entryPoint, sizeof(entryPoint), 0);
    printHex(hdr.e_cs);
    VioWrtTTY(":", 1, 0);
    printHex(hdr.e_ip);
    VioWrtTTY(relocationTableAddress, sizeof(relocationTableAddress), 0);
    printHex(hdr.e_lfarlc);
    VioWrtTTY(overlayNumber, sizeof(overlayNumber), 0);
    printHex(hdr.e_ovno);
    VioWrtTTY(reservedWords, sizeof(reservedWords), 0);
    for (i = 0; i <= ERES1WDS; i++) {
      VioWrtTTY(" ", 1, 0);
      printHex(hdr.e_res[i]);
    }
    for (i = 0; i <= ERES2WDS; i++) {
      VioWrtTTY(" ", 1, 0);
      printHex(hdr.e_res2[i]);
    }
    VioWrtTTY(newEXEHeaderAddress, sizeof(newEXEHeaderAddress), 0);
    printHex(hdr.e_lfanew & 0xFFFF);
    printHex(hdr.e_lfanew>>16 & 0xFFFF);
  //  VioWrtTTY(memoryNeeded, sizeof(memoryNeeded), 0);
  //  printHex(hdr.e_lfarlc);
    return;
}

void DumpNE(struct new_exe hdr)
{
  char magicNumber[]            ="\n\rMagic number:             ";
  char version[]                ="\n\rVersion:                  ";
/*
struct new_exe {
    unsigned short  ne_magic;
    unsigned char   ne_ver;
    unsigned char   ne_rev;
    unsigned short  ne_enttab;
    unsigned short  ne_cbenttab;
    long            ne_crc;
    unsigned short  ne_flags;
    unsigned short  ne_autodata;
    unsigned short  ne_heap;
    unsigned short  ne_stack;
    long            ne_csip;
    long            ne_sssp;
    unsigned short  ne_cseg;
    unsigned short  ne_cmod;
    unsigned short  ne_cbnrestab;
    unsigned short  ne_segtab;
    unsigned short  ne_rsrctab;
    unsigned short  ne_restab;
    unsigned short  ne_modtab;
    unsigned short  ne_imptab;
    long            ne_nrestab;
    unsigned short  ne_cmovent;
    unsigned short  ne_align;
    unsigned short  ne_cres;
    unsigned char   ne_exetyp;
    unsigned char   ne_flagsothers;
    char            ne_res[NERESBYTES];
};
*/
  VioWrtTTY(magicNumber, sizeof(magicNumber), 0);
  printHex(hdr.ne_magic);
  VioWrtTTY(version, sizeof(version), 0);
  printHex(hdr.ne_ver);
  VioWrtTTY(".", 1, 0);
  printHex(hdr.ne_rev);
  return;
}
 
void main(void)
{
  struct exe_hdr mz_hdr;
  struct new_exe ne_hdr;
  HFILE FileHandle;
  USHORT rc;
  USHORT Action;
  USHORT BytesRead;     /* Bytes read (returned) */
  ULONG Local;
  struct EXE_RELOC *r;
  PSEL sel;

  rc = DosOpen("exehdr.exe",            /* File path name */
               &FileHandle,             /* File handle */
               &Action,                 /* Action taken */
               0,                       /* File primary allocation */
               FILE_ARCHIVED,           /* File attribute */
               FILE_OPEN,               /* Open Function type */
               0,                       /* Open mode of the file */
               0);                      /* Reserved (must be zero) */
  rc = DosRead(FileHandle, &mz_hdr, sizeof(mz_hdr), &BytesRead);
  DumpMZ(mz_hdr);
  rc = DosAllocSeg(sizeof(struct EXE_RELOC)*mz_hdr.e_crlc, sel, 0);
  rc = DosChgFilePtr(FileHandle,  /* File handle */
                          mz_hdr.e_lfarlc,   /* Distance to move in bytes */
                          FILE_BEGIN,    /* Method of moving */
                          &Local);     /* New pointer location */
  rc = DosRead(FileHandle, MAKEP(sel,0), sizeof(struct EXE_RELOC)*mz_hdr.e_crlc, &BytesRead);
  DumpMZReloc(MAKEP(sel,0), mz_hdr.e_crlc);
  if (is_extended_exe(mz_hdr))
  {
    rc = DosRead(FileHandle, &ne_hdr, sizeof(ne_hdr), &BytesRead);
    if (is_NE(ne_hdr)) DumpNE(ne_hdr);
  }
  DosClose(FileHandle);
}

