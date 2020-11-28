SAMPLE = Sample/1.Window/init.c
#SAMPLE = Sample/1.Window/create.c
#SAMPLE = Sample/1.Window/close.c
#SAMPLE = Sample/1.Window/caption.c
#SAMPLE = Sample/1.Window/resize.c
#SAMPLE = Sample/1.Window/polar.c
#SAMPLE = Sample/2.System/clipboard.c
#SAMPLE = Sample/2.System/debugf.c
#SAMPLE = Sample/2.System/delay.c
#SAMPLE = Sample/2.System/file.c
#SAMPLE = Sample/2.System/music.c
#SAMPLE = Sample/2.System/server.c
#SAMPLE = Sample/2.System/client.c
#SAMPLE = Sample/3.Canvas/bitmap.c
#SAMPLE = Sample/3.Canvas/clear.c
#SAMPLE = Sample/3.Canvas/figure.c
#SAMPLE = Sample/3.Canvas/pixel.c
#SAMPLE = Sample/3.Canvas/string.c
#SAMPLE = Sample/4.Bios/ascii.c
#SAMPLE = Sample/4.Bios/key.c
#SAMPLE = Sample/4.Bios/mouse.c
#SAMPLE = Sample/5.Console/write.c
#SAMPLE = Sample/6.Control/menu.c
#SAMPLE = Sample/6.Control/panel.c
#SAMPLE = Sample/6.Control/popup.c
#SAMPLE = Sample/7.Widget/drag.c
#SAMPLE = Sample/7.Widget/output.c
#SAMPLE = Sample/7.Widget/pic.c
#SAMPLE = Sample/7.Widget/submit.c
#SAMPLE = Sample/8.Graph/view.c
#SAMPLE = Sample/8.Graph/view.cpp
#SAMPLE = Sample/9.Crypto/aes.c
#SAMPLE = Sample/9.Crypto/des.c
#SAMPLE = Sample/9.Crypto/rsa.c
#SAMPLE = Sample/10.Data/json.c
#SAMPLE = Sample/10.Data/zip.c


LINK = g++
GCC = g++
AR = ar crv

FLAGS = 
#FLAGS = -D DEBUG
HEADER = -I pack/include/
ifdef FLAGS
	OUTDIR = Debug/
	vpath %.o Debug
else
	OUTDIR = Release/
	vpath %.o Release
endif
vpath %.cpp SGL/

SGLOBJ = frame.o geometry.o graph.o imfile.o data.o encryption.o
WINLIB = -lsgl -lgdi32 -lwsock32 -lcomdlg32 -lshlwapi -lwinmm

all: libsgl.a sample.exe

libsgl.a: $(SGLOBJ)
	$(AR) $(OUTDIR)$@ $^

.cpp.o:
	$(GCC) -c -o $(OUTDIR)$@ $< $(HEADER) $(FLAGS)
	
sample.exe:
	$(LINK) $(SAMPLE)\
	 -o $(OUTDIR)$@\
	 -L $(OUTDIR) $(WINLIB) $(HEADER) $(FLAGS)


.PHONY:clean
clean:
	rm -rf $(OUTDIR)*.o $(OUTDIR)libsgl.a $(OUTDIR)sample.exe

