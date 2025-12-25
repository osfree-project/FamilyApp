#
# A Makefile for ATTRIB
# (c) osFree project,
#

PROJ = attrib
TRGT = $(PROJ).exe
DESC = Changes file attributes
#defines object file names in format $(p)objname$(e)
srcfiles = $(p)attrib$(e)
# defines additional options for C compiler
STUB     = $(FILESDIR)$(SEP)os2$(SEP)mdos$(SEP)$(PROJ).exe
ADD_COPT = -i=$(MYDIR)..$(SEP)include #-ad
UNI2H = 0
#HEAPSIZE = 18k
STACKSIZE = 18k

!include $(%ROOT)tools/mk/appsfapi.mk

.c: $(MYPATH)..
