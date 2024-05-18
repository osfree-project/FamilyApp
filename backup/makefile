#
# A Makefile for BACKUP
# (c) osFree project,
#

PROJ = BACKUP
TRGT = $(PROJ).exe
DESC = Disk backup tool
#defines object file names in format $(p)objname$(e)
srcfiles = $(p)BACKUP$(e) $(p)_PARSE$(e) $(p)_MSGRET$(e)
# defines additional options for C compiler
#STUB     = $(FILESDIR)$(SEP)os2$(SEP)mdos$(SEP)$(PROJ).exe
UNI2H = 0
JWASM = 1

!include $(%ROOT)tools/mk/appsos2v1.mk
!include $(MYDIR)backup.mk

ADD_COPT = -i=$(PORT_BASE)v4.0$(SEP)src$(SEP)H
ADD_ASMOPT = -I=$(PORT_BASE)v4.0$(SEP)src$(SEP)INC

.c: $(PORT_BASE)v4.0$(SEP)src$(SEP)CMD$(SEP)BACKUP
.asm: $(PORT_BASE)v4.0$(SEP)src$(SEP)CMD$(SEP)BACKUP

.h: $(PORT_BASE)v4.0$(SEP)src$(SEP)CMD$(SEP)BACKUP
.h: $(PORT_BASE)v4.0$(SEP)src$(SEP)H
.inc: $(PORT_BASE)v4.0$(SEP)src$(SEP)INC

