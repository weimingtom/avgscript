TARGET_LIB = libnge.a
OBJS = vram.o GuSystem.o libnge.o libnge_global.o 

INCDIR = /usr/local/pspdev/psp/include/freetype2 /usr/local/pspdev/psp/include
CFLAGS = -G0 -w
CXXFLAGS = $(CFLAGS) -fno-rtti
ASFLAGS = $(CFLAGS)

PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin
CFLAGS += $(shell $(PSPBIN)/sdl-config --cflags)
LIBS += $(shell $(PSPBIN)/sdl-config --libs)
include $(PSPSDK)/lib/build.mak
install:
	cp libnge.a ../lib/
	cp *.h ../include/
