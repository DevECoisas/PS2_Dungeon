EE_BIN = main.elf
EE_OBJS = src/main.o src/object.o src/image.o
EE_LIBS := -L${PS2DEV}/gsKit/lib -L${PS2SDK}/ports/lib -lSDL2main -lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lpatches -lgskit -ldmakit -lps2_drivers -lmodplug -laudsrv -lpad -lc 
EE_INCS := -I${PS2SDK}/ports/include -I./src/Ports/PS2
EE_CFLAGS += -DPS2 -Dmain=SDL_main -DHAVE_SDL2

all: $(EE_BIN) 

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

run: $(EE_BIN)
	ps2client -h $(PS2_IP) execee host:$(EE_BIN)

reset:
	ps2client -h $(PS2_IP) reset

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal_cpp