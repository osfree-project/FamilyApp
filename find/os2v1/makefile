#
# A Makefile for ATTRIB
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

!include $(%ROOT)tools/mk/appsos2v1.mk
!include $(MYDIR)..$(SEP)port$(SEP)find.mk

.c: $(PORT_BASE)src;$(PORT_BASE)kitten;

.h: $(PORT_BASE)src;$(PORT_BASE)kitten;
