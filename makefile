#
# A main Makefile for FamilyApp project
# (c) osFree project.
#

# fapi is a library for static linking
# api is a library for dynamic linking
# They must come first because all tools here depends on them.

DIRS = fapi attrib comp diskcopy find sort tree xcopy # backup fc restore

!include $(%ROOT)tools/mk/all.mk
