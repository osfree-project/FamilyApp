#
# A Makefile for FIND
# (c) osFree project,
#

PROJ = find
TRGT = $(PROJ).exe
DESC = Find string in file
#defines object file names in format $(p)objname$(e)
srcfiles = $(p)find$(e) $(p)find_str$(e) $(p)kitten$(e)
# defines additional options for C compiler
STUB     = $(FILESDIR)$(SEP)os2$(SEP)mdos$(SEP)$(PROJ).exe
ADD_COPT = -i=$(MYDIR)..$(SEP)include
UNI2H = 0
HEAPSIZE = 18k
STACKSIZE = 18k

!include $(%ROOT)tools/mk/appsfapi.mk
!include $(MYDIR)..$(SEP)port$(SEP)find.mk

.c: $(PORT_BASE)src;$(PORT_BASE)kitten;

.h: $(PORT_BASE)src;$(PORT_BASE)kitten;
