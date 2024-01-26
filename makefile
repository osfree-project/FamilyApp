#
# A main Makefile for FamilyApp project
# (c) osFree project.
# valerius, 2006/10/30
#

# TOOLS target comes first because all of them required for build process.
# DOS target comes before OS2 target because many DOS executables will be joined with
# some of OS/2 executables as dos stubs.
# SOM target comes before OS2 target because used to build WPS classes

DIRS = tools fapi cmd

!include $(%ROOT)tools/mk/all.mk
