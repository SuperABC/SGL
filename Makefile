SAMPLE = Sample/1.Window/init.c

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