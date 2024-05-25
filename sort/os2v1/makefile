#
# A Makefile for SORT
# (c) osFree project,
# author, date
#

PROJ = sort
TRGT = $(PROJ).exe
DESC = Sort strings in file
#defines object file names in format objname.$(O)
srcfiles = $(p)sort$(e) $(p)kitten$(e)
# defines additional options for C compiler
ADD_COPT = -i=$(MYDIR) -i=$(MYDIR)..$(SEP)include
#STUB=$(FILESDIR)$(SEP)os2$(SEP)mdos$(SEP)$(PROJ).exe
#UNI2H = 1

!include $(%ROOT)tools/mk/appsos2v1.mk
!include $(MYDIR)../port/sort.mk

.c: $(PORT_BASE)src;$(PORT_BASE)kitten;$(PORT_BASE)tnyprntf;

.h: $(PORT_BASE)src;$(PORT_BASE)kitten;$(PORT_BASE)tnyprntf;
