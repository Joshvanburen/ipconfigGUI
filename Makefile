#Josh Van Buren
#Make file for ipconfiggui

#Variables
GPP = g++
SOURCE = ipconfigGUI.cpp
RCSOURCE = ipconfigGUI.rc
RCONAME = appRC.o
LIBS1 = -L/mingw32/lib   -Wl,--subsystem,windows -mwindows /mingw32/lib/libwx_mswu_xrc-3.0.a   
LIBS2 = /mingw32/lib/libwx_mswu_webview-3.0.a /mingw32/lib/libwx_mswu_qa-3.0.a /mingw32/lib/libwx_baseu_net-3.0.a /mingw32/lib/libwx_mswu_html-3.0.a /mingw32/lib/libwx_mswu_adv-3.0.a
LIBS3 = /mingw32/lib/libwx_mswu_core-3.0.a /mingw32/lib/libwx_baseu_xml-3.0.a /mingw32/lib/libwx_baseu-3.0.a -lpng -ljpeg -ltiff -lexpat
LIBS4 = -lwxregexu-3.0 -lz -lrpcrt4 -loleaut32 -lole32 -luuid -lwinspool -lwinmm -lshell32 -lcomctl32 -lcomdlg32 -ladvapi32 -lwsock32 -lgdi32 -lws2_32
EXTRALIBS = -liphlpapi
WXINC = -I/mingw32/lib/wx/include/msw-unicode-static-3.0 -I/mingw32/include/wx-3.0 -D_FILE_OFFSET_BITS=64 -D__WXMSW__ -mthreads -O2
OBJS = ipconfigGUI.o  appRC.o
PNAME = ipconfigGUI.exe
WINDRES = windres
WINDRESINC = --include-dir /mingw32/include/wx-3.0 --define __WIN32__ --define __GNUWIN32__


#Compile command
ipconfigGUI.exe : $(SOURCE) 
	$(WINDRES) $(WINDRESINC) -i $(RCSOURCE) -O coff -o $(RCONAME)
	$(GPP) -c $(SOURCE) $(WXINC)
	$(GPP) -o $(PNAME) $(OBJS) $(LIBS1) $(LIBS2) $(LIBS3) $(LIBS4) $(EXTRALIBS) -static -s
	
#Cleans up
.PHONY clean :
	rm *.o *.exe -f 