cd ..
call fapi
cd tools
PATH C:\WATCOM\BINNT64;C:\WATCOM\BINNT;%PATH%
SET INCLUDE=C:\WATCOM\H;C:\WATCOM\H\NT;C:\WATCOM\H\NT\DIRECTX;C:\WATCOM\H\NT\DDK;%INCLUDE%
SET WATCOM=C:\WATCOM
SET EDPATH=C:\WATCOM\EDDAT
SET WHTMLHELP=C:\WATCOM\BINNT\HELP
SET WIPFC=C:\WATCOM\WIPFC
wcl -bt=os2v1 -fm -lr -ms -0 exehdr.c ..\fapi.lib %watcom%\lib286\os2\os2.lib -I%watcom%\h\os21x
jwasm -0 -mz fapistub.asm 
