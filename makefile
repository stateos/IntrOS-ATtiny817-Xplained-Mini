#**********************************************************#
#file     makefile
#author   Rajmund Szymanski
#date     18.05.2017
#brief    AVR makefile.
#**********************************************************#

GNUCC      := c:/sys/avr-gcc/bin/avr-
PROGRAM     = c:/sys/tools/atprogram/atprogram -t medbg -i updi -d $(TARGET)

#----------------------------------------------------------#

PROJECT    ?= $(notdir $(CURDIR))
DEFS       ?=
LIBS       ?=
DIRS       ?=
KEYS       ?=
INCS       ?=
OPTF       ?= s
TARGET     ?= attiny817
F_CPU      ?= 20000000

#----------------------------------------------------------#

ifeq  ($(TARGET),)
$(error Undefined TARGET!)
endif

ifneq ($(F_CPU),)
DEFS       += F_CPU=$(F_CPU)
endif

KEYS       += .gnucc .avr .$(TARGET) *
INCS       +=  ./.

#----------------------------------------------------------#

AS         := $(GNUCC)gcc -x assembler-with-cpp
CC         := $(GNUCC)gcc
CXX        := $(GNUCC)g++
COPY       := $(GNUCC)objcopy
DUMP       := $(GNUCC)objdump
SIZE       := $(GNUCC)size
LD         := $(GNUCC)g++
AR         := $(GNUCC)ar

RM         ?= rm -f

#----------------------------------------------------------#

DTREE       = $(foreach d,$(foreach k,$(KEYS),$(wildcard $1$k)),$(dir $d) $(call DTREE,$d/))

VPATH      := $(sort $(call DTREE,) $(foreach d,$(DIRS),$(call DTREE,$d/)))

#----------------------------------------------------------#

AS_EXT     := .S
C_EXT      := .c
CXX_EXT    := .cpp

INC_DIRS   := $(sort $(dir $(foreach d,$(VPATH),$(wildcard $d*.h $d*.hpp))))
LIB_DIRS   := $(sort $(dir $(foreach d,$(VPATH),$(wildcard $dlib*.a $d*.ld))))
OBJ_SRCS   :=              $(foreach d,$(VPATH),$(wildcard $d*.o))
AS_SRCS    :=              $(foreach d,$(VPATH),$(wildcard $d*$(AS_EXT)))
C_SRCS     :=              $(foreach d,$(VPATH),$(wildcard $d*$(C_EXT)))
CXX_SRCS   :=              $(foreach d,$(VPATH),$(wildcard $d*$(CXX_EXT)))
LIB_SRCS   :=     $(notdir $(foreach d,$(VPATH),$(wildcard $dlib*.a)))
ifeq ($(strip $(PROJECT)),)
PROJECT    :=     $(notdir $(CURDIR))
endif

AS_SRCS    := $(AS_SRCS:%.s=)

#----------------------------------------------------------#

BIN        := $(PROJECT).bin
EEP        := $(PROJECT).eep
ELF        := $(PROJECT).elf
HEX        := $(PROJECT).hex
LIB        := lib$(PROJECT).a
LSS        := $(PROJECT).lss
MAP        := $(PROJECT).map

OBJS       := $(AS_SRCS:%$(AS_EXT)=%.o)
OBJS       += $(C_SRCS:%$(C_EXT)=%.o)
OBJS       += $(CXX_SRCS:%$(CXX_EXT)=%.o)
DEPS       := $(OBJS:.o=.d)
LSTS       := $(OBJS:.o=.lst)

#----------------------------------------------------------#

COMMON_F    = -mmcu=$(TARGET)
COMMON_F   += -O$(OPTF) -ffunction-sections -fdata-sections
ifneq ($(filter USE_LTO,$(DEFS)),)
COMMON_F   += -flto
endif
COMMON_F   += -Wall -Wextra # -Wpedantic
COMMON_F   += -MD -MP
COMMON_F   += # -Wa,-amhls=$(@:.o=.lst)
COMMON_F   += # -g -ggdb
COMMON_F   += -B .dev/$(TARGET)

AS_FLAGS    =
C_FLAGS     = -std=gnu11
CXX_FLAGS   = -std=gnu++11 -fno-rtti -fno-exceptions
LD_FLAGS    = -Wl,-Map=$(MAP),--cref,--no-warn-mismatch,--gc-sections

#----------------------------------------------------------#

DEFS_F     := $(DEFS:%=-D%)
LIBS       += $(LIB_SRCS:lib%.a=%)
LIBS_F     := $(LIBS:%=-l%)
OBJS_ALL   := $(sort $(OBJ_SRCS) $(OBJS))
INC_DIRS   += $(INCS:%=%/)
INC_DIRS_F := $(INC_DIRS:%=-I%)
LIB_DIRS_F := $(LIB_DIRS:%=-L%)

AS_FLAGS   += $(COMMON_F) $(DEFS_F) $(INC_DIRS_F)
C_FLAGS    += $(COMMON_F) $(DEFS_F) $(INC_DIRS_F)
CXX_FLAGS  += $(COMMON_F) $(DEFS_F) $(INC_DIRS_F)
LD_FLAGS   += $(COMMON_F)

#----------------------------------------------------------#

all : $(LSS) print_elf_size

lib : $(LIB) print_size

$(ELF) : $(OBJS_ALL)
	$(info Linking target: $(ELF))
	$(LD) $(LD_FLAGS) $(OBJS_ALL) $(LIBS_F) $(LIB_DIRS_F) -o $@

$(LIB) : $(OBJS_ALL)
	$(info Building library: $(LIB))
	$(AR) -r $@ $?

$(OBJS) : $(MAKEFILE_LIST)

%.o : %$(AS_EXT)
	$(info Assembling file: $<)
	$(AS) $(AS_FLAGS) -c $< -o $@

%.o : %$(C_EXT)
	$(info Compiling file: $<)
	$(CC) $(C_FLAGS) -c $< -o $@

%.o : %$(CXX_EXT)
	$(info Compiling file: $<)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

$(BIN) : $(ELF)
	$(info Creating BIN image: $(BIN))
	$(COPY) -O binary $< $@

$(HEX) : $(ELF)
	$(info Creating HEX image: $(HEX))
	$(COPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

$(EEP) : $(ELF)
	$(info Creating EEP image: $(EEP))
	$(COPY) -O ihex -j .eeprom --set-section-flags ".eeprom=alloc,load" --change-section-lma ".eeprom=0" $< $@

$(LSS) : $(ELF)
	$(info Creating extended listing: $(LSS))
	$(DUMP) --demangle -S $< > $@

print_size :
	$(info Size of modules:)
	$(SIZE) -B -t --common $(OBJS_ALL)

print_elf_size : print_size
	$(info Size of target file:)
	$(SIZE) -B $(ELF)

GENERATED = $(BIN) $(EEP) $(ELF) $(HEX) $(LIB) $(LSS) $(MAP) $(DEPS) $(LSTS) $(OBJS)

clean :
	$(info Removing all generated output files)
	$(RM) $(GENERATED)

flash : all # $(HEX)
	$(info Programing device...)
	$(PROGRAM) program -c -fl -f $(ELF) --verify

eeprom : all # $(EEP)
	$(info Programing device...)
	$(PROGRAM) program -ee -f $(ELF) --verify

reset :
	$(info Reseting device...)
	$(PROGRAM) reset

.PHONY : all lib clean flash eeprom reset

-include $(DEPS)
