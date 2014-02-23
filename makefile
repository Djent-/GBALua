#===========================================================================
#================================ Defines ==================================
#===========================================================================
#COMPILE_MULTIBOOT=		-DCOMPILE_MULTIBOOT

OBJCOPY=				arm-thumb-elf-objcopy
CC=						arm-thumb-elf-gcc

LINKSCRIPT=				crtls/lnkscript
CRT=					crtls/crt0.o
BIN2C=					Tools/bin2c
LUAC=					Tools/LuaC
GBALUAAPP=				Tools/GBALuaAppend
GCC2VC=					Tools/GCC2VC
BUILDCNT=				Tools/BuildCnt

DEPENDFILE=				Makedepend


ifdef COMPILE_MULTIBOOT

MAPFILE=				GBALuaCore.mb.map
TARGET_ELF=				GBALuaCore.mb.elf
TARGET_BIN=				GBALuaCore.mb.gba
GBALUAAPPFLAGS=			-o Example.gba -mb -m 

else

MAPFILE=				GBALuaCore.map
TARGET_ELF=				GBALuaCore.elf
TARGET_BIN=				GBALuaCore.gba
GBALUAAPPFLAGS=			-o Example.gba -m 

endif



LUAAPP=					LuaCode.bin
LUAFILES=				LuaCode/LuaApp.lua\
						LuaCode/Regs.lua\
						LuaCode/MemMap.lua\

DATAFILES=				Media/GBALUAFONT_TILES.bin\
						Media/GBALUAFONT_PALETTE.bin\
						Media/LUALOGO_TILES.bin\
						Media/LUALOGO_PALETTE.bin\
						Media/LUALOGO_MAP.bin\
						Media/BKG_TILES.bin\
						Media/BKG_PALETTE.bin\
						Media/BKG_MAP.bin\

UNOPTIMIZEDCFILES=	


UNOPTIMIZEDOBJFILES=	$(UNOPTIMIZEDCFILES:.c=.o)

OPTIMIZEDCFILES=		Media/SmallFont.c\
						Lua/lapi.c\
						Lua/lcode.c\
						Lua/ldebug.c\
						Lua/ldo.c\
						Lua/lfunc.c\
						Lua/lgc.c\
						Lua/llex.c\
						Lua/lmem.c\
						Lua/lobject.c\
						Lua/lparser.c\
						Lua/lstate.c\
						Lua/lstring.c\
						Lua/ltable.c\
						Lua/ltm.c\
						Lua/lundump.c\
						Lua/lvm.c\
						Lua/lzio.c\
						Mem.c\
						Misc.c\
						GBADebug.c\
						Main.c\


OPTIMIZEDOBJFILES=	$(OPTIMIZEDCFILES:.c=.o)

CFLAGS=				-Wall\
					-mthumb-interwork\
					-mthumb\
					-D_POSIX_SOURCE\
					-ffreestanding\
					$(COMPILE_MULTIBOOT)\


OPTIMIZEDCFLAGS=	$(CFLAGS)\
					-O2

UNOPTIMIZEDCFLAGS=	$(CFLAGS)\
					-O1


LDFLAGS=			-Map $(MAPFILE)\
					-nostartfiles\
					-mthumb-interwork\
					-T$(LINKSCRIPT)\
#					-nostdlib\
#					-nodefaultlibs\

#===========================================================================
#=============================== Targets ===================================
#===========================================================================
.PHONY: all clean depend


$(TARGET_BIN): $(TARGET_ELF)
	@echo ------------Rebuilding binary $@---------
	@$(BUILDCNT)
	@$(OBJCOPY) -O binary $< $@
	@cat -B $(LUAFILES) > $(LUAAPP)
	@$(GBALUAAPP) $(GBALUAAPPFLAGS) $@ $(LUAAPP) $(DATAFILES)
	@echo Done.

$(TARGET_ELF): $(OPTIMIZEDOBJFILES) $(UNOPTIMIZEDOBJFILES)
	@echo > $(MAPFILE)
	@echo -----------Linking $@---------------------
	@$(CC) -Wl,$(LDFLAGS) -o $@ $(CRT) $(OPTIMIZEDOBJFILES) $(UNOPTIMIZEDOBJFILES)
	@echo Done.

all:	clean depend $(TARGET_BIN)

clean:
	@echo ------------Cleaning up--------------------
	@$(RM) -f $(OPTIMIZEDOBJFILES)
	@$(RM) -f $(UNOPTIMIZEDOBJFILES)
	@$(RM) -f $(DEPENDFILE)
	@$(RM) -f $(MAPFILE)
	@$(RM) -f $(TARGET_ELF)
	@$(RM) -f $(TARGET_BIN)
	@$(RM) -f $(LUAAPP)
	@echo Done.

depend:
	@echo -----------Making Up Dependencies----------
	@$(CC) $(OPTIMIZEDCFLAGS) -M $(OPTIMIZEDCFILES) $(UNOPTIMIZEDCFILES) > $(DEPENDFILE)
	@echo Done.
	@echo ------------Precompiling Lua---------------

$(DEPENDFILE): 
	@echo -----------Writing DependFile--------------
	@$(CC) $(OPTIMIZEDCFLAGS) -M $(OPTIMIZEDCFILES) $(UNOPTIMIZEDCFILES) > $(DEPENDFILE)
	@echo Done.
	@echo ------------Compiling Project...-----------


dist:	src-dist dev-kit-dist

dev-kit-dist:
	@rm -d -r -f dev-kit-dist
	@mkdir dev-kit-dist
	@mkdir dev-kit-dist/Tools
	@mkdir dev-kit-dist/LuaCode
	@mkdir dev-kit-dist/Example
	@mkdir dev-kit-dist/Example/Example1
	@mkdir dev-kit-dist/Example/Example2
	@mkdir dev-kit-dist/Example/Example3
	@mkdir dev-kit-dist/Example/Example4
	@mkdir dev-kit-dist/Example/Example5
	@mkdir dev-kit-dist/Documentation
	@cp Tools/PCX2Agb.exe dev-kit-dist/Tools
	@cp Tools/LuaC.exe dev-kit-dist/Tools
	@cp Tools/GBALuaAppend.exe dev-kit-dist/Tools
	@cp Site/Contact.html dev-kit-dist/Documentation
	@cp Site/Documentation.html dev-kit-dist/Documentation
	@cp Site/Download.html dev-kit-dist/Documentation
	@cp Site/FAQ.html dev-kit-dist/Documentation
	@cp Site/Improvements.html dev-kit-dist/Documentation
	@cp Site/index.html dev-kit-dist/Documentation
	@cp Site/Install.html dev-kit-dist/Documentation
	@cp Site/History.html dev-kit-dist/Documentation
	@cp Site/GBALua.gif dev-kit-dist/Documentation
	@cp LuaCode/Regs.lua dev-kit-dist/LuaCode
	@cp LuaCode/MemMap.lua dev-kit-dist/LuaCode
	@cp LuaCode/Joy.lua dev-kit-dist/LuaCode
	@cp Lua/COPYRIGHT dev-kit-dist
	@cp Example/Example1/build.bat dev-kit-dist/Example/Example1
	@cp Example/Example1/Example1.lua dev-kit-dist/Example/Example1
	@cp Example/Example1/Example1.txt dev-kit-dist/Example/Example1
	@cp Example/Example1/GBALuaFont.pcx dev-kit-dist/Example/Example1
	@cp Example/Example2/build.bat dev-kit-dist/Example/Example2
	@cp Example/Example2/Example2.lua dev-kit-dist/Example/Example2
	@cp Example/Example2/Example2.txt dev-kit-dist/Example/Example2
	@cp Example/Example2/GBALuaFont.pcx dev-kit-dist/Example/Example2
	@cp Example/Example3/build.bat dev-kit-dist/Example/Example3
	@cp Example/Example3/Example3.lua dev-kit-dist/Example/Example3
	@cp Example/Example3/Example3.txt dev-kit-dist/Example/Example3
	@cp Example/Example3/GBALuaFont.pcx dev-kit-dist/Example/Example3
	@cp Example/Example3/LuaLogo.pcx dev-kit-dist/Example/Example3
	@cp Example/Example3/Bkg.pcx dev-kit-dist/Example/Example3
	@cp Example/Example4/build.bat dev-kit-dist/Example/Example4
	@cp Example/Example4/Example4.lua dev-kit-dist/Example/Example4
	@cp Example/Example4/Example4.txt dev-kit-dist/Example/Example4
	@cp Example/Example4/Menu.pcx dev-kit-dist/Example/Example4
	@cp Example/Example4/Corvette.pcx dev-kit-dist/Example/Example4
	
	@cp Example/Example5/build.bat dev-kit-dist/Example/Example5
	@cp Example/Example5/Example5.lua dev-kit-dist/Example/Example5
	@cp Example/Example5/Example5.txt dev-kit-dist/Example/Example5
	@cp Example/Example5/Zero.pcx dev-kit-dist/Example/Example5
	@cp Example/Example5/One.pcx dev-kit-dist/Example/Example5
	@cp Example/Example5/Two.pcx dev-kit-dist/Example/Example5
	@cp Example/Example5/Three.pcx dev-kit-dist/Example/Example5
	@cp Example/Example5/Four.pcx dev-kit-dist/Example/Example5
	@cp Example/Example5/Five.pcx dev-kit-dist/Example/Example5
	@cp Example/Example5/Six.pcx dev-kit-dist/Example/Example5
	@cp Example/Example5/Seven.pcx dev-kit-dist/Example/Example5
	@cp Example/Example5/Eight.pcx dev-kit-dist/Example/Example5
	@cp Example/Example5/Nine.pcx dev-kit-dist/Example/Example5
	@cp GBALuaCore.gba dev-kit-dist
	@cp GBALuaCore.mb.gba dev-kit-dist

src-dist:
	@rm -d -r -f src-dist
	@mkdir src-dist
	@mkdir src-dist/Lua
	@mkdir src-dist/Media
	@mkdir src-dist/Tools
	@mkdir src-dist/Tools/PCX2Agb
	@mkdir src-dist/Tools/GBALuaAppend
	@mkdir src-dist/crtls
	@cp Media/SmallFont.h src-dist/Media
	@cp Media/SmallFont.pcx src-dist/Media
	@cp --parents $(OPTIMIZEDCFILES) src-dist
	@cp Lua/*.h src-dist/Lua
	@cp BuildCnt.h src-dist/
	@cp GBASys.h src-dist/
	@cp GBADebug.h src-dist/
	@cp GBAIrq.h src-dist/
	@cp GBAJoy.h src-dist/
	@cp GBAReg.h src-dist/
	@cp Lua/COPYRIGHT src-dist/Lua
	@cp makefile src-dist/
	@cp Mem.h src-dist/
	@cp Misc.h src-dist/
	@cp crtls/crt0.s src-dist/crtls
	@cp crtls/lnkscript src-dist/crtls
	@cp crtls/crtls.txt src-dist/crtls
	@cp Tools/GBALuaAppend/Main.cpp src-dist/Tools/GBALuaAppend
	@cp Tools/PCX2Agb/main.cpp src-dist/Tools/PCX2Agb
	@cp Tools/PCX2Agb/main.h src-dist/Tools/PCX2Agb
	@cp Tools/PCX2Agb/DecodePCX.cpp src-dist/Tools/PCX2Agb
	@cp Tools/PCX2Agb/DecodePCX.h src-dist/Tools/PCX2Agb
	@cp Tools/PCX2Agb/FileWrite.cpp src-dist/Tools/PCX2Agb
	@cp Tools/PCX2Agb/FileWrite.h src-dist/Tools/PCX2Agb
	@cp Tools/PCX2Agb/Params.cpp src-dist/Tools/PCX2Agb
	@cp Tools/PCX2Agb/Params.h src-dist/Tools/PCX2Agb
	@cp Tools/BuildCnt.cpp src-dist/Tools


#===========================================================================
#================================ Rules ====================================
#===========================================================================
$(OPTIMIZEDOBJFILES): %.o: %.c
	@echo $<
	@$(CC) $(OPTIMIZEDCFLAGS) -c $< -o $@ 2>&1 | $(GCC2VC)

$(UNOPTIMIZEDOBJFILES): %.o: %.c
	@echo $<
	@$(CC) $(UNOPTIMIZEDCFLAGS) -c $< -o $@ 2>&1 | $(GCC2VC)

.SUFFIXES: .s .c .o .a

#===========================================================================
#=============================== Includes ==================================
#===========================================================================




include $(DEPENDFILE)

