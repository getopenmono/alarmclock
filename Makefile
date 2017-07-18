# Makefile created by monomake, Fre 10 Feb 2017 14:20:39 CET
# Project: alarmclock

MONO_PATH=$(subst \,/,$(shell monomake path --bare))
include $(MONO_PATH)/predefines.mk

TARGET=alarmclock

include $(MONO_PATH)/mono.mk
