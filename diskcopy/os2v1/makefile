#
# A Makefile for DISKCOPY
# (c) osFree project,
#

PROJ = diskcopy
TRGT = $(PROJ).exe
DESC = Copy diskette
#defines object file names in format $(p)objname$(e)
srcfiles = $(p)diskcopy$(e) $(p)smallcode$(e)
# defines additional options for C compiler
STUB     = $(FILESDIR)$(SEP)os2$(SEP)mdos$(SEP)$(PROJ).exe
ADD_COPT = -ml -i=$(MYDIR)..$(SEP)include
UNI2H = 0

!include $(%ROOT)tools/mk/appsos2v1.mk

.c: $(MYPATH)..
