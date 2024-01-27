#
# make macros for
# win16 apps
#

!ifndef __appsw16_mk__
!define __appsw16_mk__
#  -i=$(ROOT)$(SEP)DOS$(SEP)WIN16$(SEP)include 

ADD_COPT = -bt=windows -i=. -i=$(WATCOM)$(SEP)h$(SEP)win $(ADD_COPT)

!ifneq NOLIBS 1
ADD_LINKOPT += path $(WATCOM)$(SEP)lib286 &
  path $(WATCOM)$(SEP)lib286$(SEP)win &
  lib windows.lib, shell.lib #clibs.lib,windows.lib,shell.lib
!endif

#  file $(WATCOM)$(SEP)lib286$(SEP)dos$(SEP)cstart_t.obj

!ifndef DEST
DEST     = os2$(SEP)mdos$(SEP)winos2
!endif

!include $(%ROOT)/tools/mk/all.mk

!ifdef DLL
ADD_COPT = -mc -zu -zc -bd $(ADD_COPT)
TARGETS  = $(PATH)$(PROJ).dll # $(PATH)$(PROJ).sym
dllopt = dll initinstance memory
!else
TARGETS  = $(PATH)$(PROJ).exe # $(PATH)$(PROJ).sym
dllopt =
!endif

RC       = winrc.exe
RCOPT    = -I$(MYDIR) -I$(PATH) -I. -I$(MYDIR)..$(SEP)..$(SEP)include -I$(MYDIR)..$(SEP)include
# RCOPT    = -I $(MYDIR);$(PATH);.;$(MYDIR)..$(SEP)..$(SEP)include;$(MYDIR)..$(SEP)include

#.res: $(PATH)

#.rc:  $(MYDIR)

.rc.res: .AUTODEPEND
 @$(SAY) WINRES   $^. $(LOG)
 @$(RC) $(RCOPT) -o $^@ $[@ $(LOG2)

!ifdef RESOURCE
deps = $(RESOURCE)
!endif

!ifdef OBJS
#$(OBJS):: $(MYDIR)makefile
!endif

$(PATH)$(PROJ).lnk: $(deps) $(OBJS) $(MYDIR)makefile .always
 @%create $^@
 @%append $^@ SYSTEM windows $(dllopt)
# @%append $^@ FORMAT windows $(dllopt)
 @%append $^@ NAME $^*
 @%append $^@ OPTION DESCRIPTION '$(FILEVER)  $(DESC)'
#!ifdef DEBUG
# @%append $^@ DEBUG $(DEBUG)
#!endif
 @%append $^@ option nocaseexact
!ifdef DLL
 @%append $^@ option oneautodata
!endif
 @%append $^@ libpath $(%WATCOM)/lib286
 @%append $^@ libpath $(%WATCOM)/lib286/win
!ifdef DLL
!ifndef NODEFAULTLIBS
!ifndef NOLIBS
 @%append $^@ libfile libentry.obj
!endif
!endif
!endif
!ifdef NODEFAULTLIBS
 @%append $^@ OPTION NODEFAULTLIBS
!endif
!ifdef HEAPSIZE
 @%append $^@ OPTION HEAP=$(HEAPSIZE)
!endif
!ifndef DLL
!ifdef STACKSIZE
 @%append $^@ OPTION ST=$(STACKSIZE)
!else
 @%append $^@ OPTION ST=8k
!endif
!endif
!ifdef RESOURCE
 @%append $^@ OPTION RESOURCE=$(RESOURCE)
!endif
!ifdef IMPORTS
 @%append $^@ IMPORT $(IMPORTS)
!endif
!ifdef EXPORTS
 @%append $^@ EXPORT $(EXPORTS)
!endif
!ifdef OPTIONS
 @%append $^@ OPTION $(OPTIONS)
!endif
!ifdef ALIASES
 alias $(ALIASES)
!endif
 @%append $^@ OPTION MAP=$^*.wmp
 $(ADDFILES_CMD)

!ifeq DLL 1
$(PATH)$(PROJ).dll: $(PATH)$(PROJ).lnk
!else
$(PATH)$(PROJ).exe: $(PATH)$(PROJ).lnk
!endif
 @$(SAY) LINK     $^. $(LOG)
 $(verbose)$(LINKER) $(LINKOPT) @$[@ $(LOG2)

!endif
