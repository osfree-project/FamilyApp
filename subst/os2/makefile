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
STUB     = $(FILESDIR)$(SEP)os2$(SEP)mdos$(SEP)$(PROJ).exe
ADD_COPT = -i=$(MYDIR)..$(SEP)include
UNI2H = 0

!include $(%ROOT)tools/mk/appsos2_cmd.mk

.c: $(MYPATH)..
