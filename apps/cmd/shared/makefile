# $Id: makefile,v 1.3 2004/08/21 14:49:03 prokushev Exp $
#
# Makefile for Watcom/LIBC interface layer

PROJ = cmd_shared
TRGT = $(PROJ).lib
ADD_COPT = -d2 -db -i=$(MYDIR)..$(SEP)include
UNI2H = 0
32_BITS = 0

# For performance/compatibility reasons, we build a "_System" and
# "_Optlink"-like libraries, corresponding to "-5r" and "-5s" options of
# Watcom compiler. $(p)segord$(e)
srcfiles   =  $(p)cmd_ExecFSEntry$(e) $(p)cmd_Messages$(e) $(p)cmd_setcurrentdisk$(e) &
             $(p)cmd_QueryCurrentDisk$(e) $(p)cmd_QueryFSName$(e) $(p)cmd_ShowVolumeInfo$(e)

!include $(%ROOT)tools/mk/libsos2v1.mk

TARGETS = $(PATH)$(PROJ).lib subdirs

$(PATH)$(PROJ).lib: $(OBJS) $(OBJS16)
 @$(MAKE) $(MAKEOPT) library=$(PATH)$(PROJ).lib library



