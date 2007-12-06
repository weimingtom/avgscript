TARGET_LIB = libcoolaudio.a
OBJS = libcoolaudio.o play_mp3_psp.o play_ogg_psp.o play_pcm_psp.o

INCDIR = 
CFLAGS = -G0 -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-rtti -fno-exceptions 
ASFLAGS = $(CFLAGS)

PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin
include $(PSPSDK)/lib/build.mak
install:
	cp $(TARGET_LIB) ../../lib/
	cp interface.h ../../include/
