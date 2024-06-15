#
# A Makefile for TREE
# (c) osFree project,
# author, date
#

PROJ = tree
TRGT = $(PROJ).exe
DESC = Graphical tree display
#defines object file names in format $(p)objname$(e)
srcfiles = $(p)tree$(e)
# defines additional options for C compiler
ADD_COPT = -i=$(MYDIR)..$(SEP)include
STUB=$(FILESDIR)$(SEP)os2$(SEP)mdos$(SEP)os2stub.exe
UNI2H = 1

!include $(%ROOT)tools/mk/appsfapi.mk

.c: $(MYPATH)..
