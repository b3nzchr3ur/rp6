###############################################################################
#
#   S i m p l e  u s a g e  i n s t r u c t i o n s:
#
#   Run from command line:
#
#   make        :   creates hex target
#   make clean  :   cleans up by removing all build products.
#
##############################################################################
# Target file name (without extension).
# This is the name of your main C source file! Do NOT append the ".c"!
# Example: Let's assume your main source file is "RP6Base_MyProgram.c", then
# you would write: TARGET = RP6Base_MyProgram
TARGET = main

###############################################################################
# Specify relative path to RP6 library files here.
# This is "../../RP6Lib" or "../RP6Lib" usually.

RP6_LIB_PATH= ../../RP6Lib
RP6_LIB_PATH_CONTROL= $(RP6_LIB_PATH)/RP6control
RP6_LIB_PATH_COMMON= $(RP6_LIB_PATH)/RP6common

RP6_LIB_PATH_OTHERS= $(RP6_LIB_PATH_CONTROL) $(RP6_LIB_PATH_COMMON)

CODE_TO_TEST_PATH = ../assignment_leds
UNITY_LIB_PATH = ../../unity

EXTRAINCDIRS = $(RP6_LIB_PATH) $(RP6_LIB_PATH_OTHERS) $(UNITY_LIB_PATH) $(CODE_TO_TEST_PATH)

#------------------------------------------------
# Main Source file is _automatically_ added here:
SRC = $(TARGET).c
SRC += led_values_updater_test.c
SRC += level_check_test.c

SRC += $(CODE_TO_TEST_PATH)/led_values_updater.c
SRC += $(CODE_TO_TEST_PATH)/level_check.c

#Additional files for the unit-testing framework and logging
SRC += $(UNITY_LIB_PATH)/unity.c

#This is where all details of the build are defined:
include $(UNITY_LIB_PATH)/makefile.unit


