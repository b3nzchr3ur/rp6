###############################################################################
#
#   S i m p l e  u s a g e  i n s t r u c t i o n s:
#
#   Run from command line:
#
#   make        :   creates hex target
#   make clean  :   cleans up by removing all build products.
#
###############################################################################
# Target file name (without extension).
# This is the name of your main C source file! Do NOT append the ".c"!
# Example: Let's assume your main source file is "RP6Base_MyProgram.c", then
# you would write: TARGET = RP6Base_MyProgram
TARGET = main

###############################################################################
# Specify relative path to RP6 library files here.
# This is "../../RP6Lib" or "../RP6Lib" usually.

RP6_LIB_PATH=../../RP6Lib
RP6_LIB_PATH_BASE = $(RP6_LIB_PATH)/RP6base
RP6_LIB_PATH_COMMON = $(RP6_LIB_PATH)/RP6common

RP6_LIB_PATH_OTHERS= $(RP6_LIB_PATH_BASE) $(RP6_LIB_PATH_COMMON)

# List any extra directories to look for include files here.
EXTRAINCDIRS = $(RP6_LIB_PATH) $(RP6_LIB_PATH_OTHERS)
###############################################################################

#------------------------------------------------
# Main Source file is _automatically_ added here:
SRC = $(TARGET).c

#SRC += $(RP6_LIB_PATH)/RP6base/RP6RobotBaseLib.c
SRC += RP6RobotBaseLib_debug.c
SRC += $(RP6_LIB_PATH)/RP6common/RP6uart.c
#SRC += $(RP6_LIB_PATH)/RP6common/RP6I2CslaveTWI.c
#SRC += $(RP6_LIB_PATH)/RP6common/RP6I2CmasterTWI.c

include $(RP6_LIB_PATH_BASE)/makefile.base
