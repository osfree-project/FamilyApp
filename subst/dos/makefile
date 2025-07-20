#
# A Makefile for SUBST
# (c) osFree project,
#

PROJ = subst
TRGT = $(PROJ).exe
DESC = Substitute path to drive letter
#defines object file names in format $(p)objname$(e)
srcfiles = $(p)$(PROJ)$(e)
# defines additional options for C compiler
ADD_COPT  = -i=$(MYDIR)..$(SEP)..$(SEP)..$(SEP)DOS$(SEP)include -i=$(MYDIR)..$(SEP)include
STUB     = $(FILESDIR)$(SEP)os2$(SEP)mdos$(SEP)$(PROJ).exe
ADD_LINKOPT = lib dos_shared.lib libpath $(ROOT)build$(SEP)lib
UNI2H = 0
HEAPSIZE = 18k
STACKSIZE = 18k

!include $(%ROOT)tools/mk/appsfapi.mk

.c: $(MYPATH)..
