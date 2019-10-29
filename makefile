HEADERS =
OBJECTS = build/main.o build/s2650.o build/sound.o build/vram.o build/lang.o build/vmachine.o build/emu.o
CPPFILES =
# SOURCES = src/main.c src/mem.c src/cpu.c src/hardware.c src/keypad.c
ASM =
CFLAGS = -DHRT_WITH_LIBHEART -Wall -march=armv4t -Wno-switch -Wno-multichar -ffast-math -mlong-calls -mcpu=arm7tdmi -mtune=arm7tdmi -marm -faggressive-loop-optimizations -fverbose-asm 
ARCH =  -mthumb-interwork
SPECS = -specs=gba_mb.specs
PREFIX = C:\devkitPro\devkitARM\bin\arm-none-eabi-

default: advcadia.gba

build/s2650.o: src/s2650.c $(HEADERS)
	$(PREFIX)gcc -Ofast $(CFLAGS) -Iinc -mthumb $(ARCH) -c $< -o $@

build/%.o: src/%.c $(HEADERS)
	$(PREFIX)gcc -Os $(CFLAGS) -Iinc -mthumb $(ARCH) -c $< -o $@
	
build/%.o: src/%.cpp $(HEADERS)
	$(PREFIX)g++ $(CFLAGS) $(ARCH) -c $< -o $@
	
build/%.out: src/%.s
	$(PREFIX)as -mthumb-interwork -mcpu=arm7tdmi $< -o $@
	
build/main.elf: $(OBJECTS)
	$(PREFIX)gcc $(SPECS) -mthumb $(ARCH) $(OBJECTS) -lheart -lm -o build/main.elf
	
advcadia.gba: build/main.elf
	$(PREFIX)objcopy -O binary build/main.elf advcadia.gba
	C:/devkitPro/devkitARM/bin/gbafix advcadia.gba -tADVCADIA -cacdia -v1
	

clean:
	-rm build/*.o build/*.out build/main.elf
	-rm advcadia.gba