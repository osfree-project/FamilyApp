#
# A Makefile for BACKUP
# (c) osFree project,
#

PROJ = RESTORE
TRGT = $(PROJ).exe
DESC = Disk restore tool
#defines object file names in format $(p)objname$(e)
srcfiles = $(p)rtt$(e)
# defines additional options for C compiler
#STUB     = $(FILESDIR)$(SEP)os2$(SEP)mdos$(SEP)$(PROJ).exe
UNI2H = 0

!include $(%ROOT)tools/mk/appsos2v1.mk
!include $(MYDIR)restore.mk

ADD_COPT = -i=$(PORT_BASE)v4.0$(SEP)src$(SEP)H -i=$(PORT_BASE)v4.0$(SEP)src$(SEP)CMD$(SEP)RESTORE
ADD_ASMOPT = -I=$(PORT_BASE)v4.0$(SEP)src$(SEP)INC

.c: $(PORT_BASE)v4.0$(SEP)src$(SEP)CMD$(SEP)RESTORE
.asm: $(PORT_BASE)v4.0$(SEP)src$(SEP)CMD$(SEP)RESTORE

.h: $(PORT_BASE)v4.0$(SEP)src$(SEP)CMD$(SEP)RESTORE
.h: $(PORT_BASE)v4.0$(SEP)src$(SEP)H
.inc: $(PORT_BASE)v4.0$(SEP)src$(SEP)INC
