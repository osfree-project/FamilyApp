# $Id: makefile,v 1.1 2004/08/16 06:27:05 prokushev Exp $

PORT_NAME = dual$(SEP)restore
PORT_TYPE = git
PORT_URL  = https://github.com/microsoft/MS-DOS
PORT_REV  = main
PORT_PATCHES  = 

!include $(%ROOT)tools/mk/port.mk
