TARGET = lesson5
OBJS = 	test.o  
INCDIR = ../../include 

CFLAGS = -G0 -w -O2
CXXFLAGS = $(CFLAGS) -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =../../lib
LDFLAGS =
LIBS +=  -lSDL -lSDL_ttf -lnge  -lSDLmain -lcoolaudio -lfreetype -lSDL_image -ljpeg -lpng -lpspmpeg -lpspaudiocodec -lpspaudio -lz -lg -lstdc++ -lm -lpsppower -lpspgum_vfpu -lpspgu

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = lesson5MP3

PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin
CFLAGS += $(shell $(PSPBIN)/sdl-config --cflags)
LIBS += $(shell $(PSPBIN)/sdl-config --libs)
include $(PSPSDK)/lib/build.mak

