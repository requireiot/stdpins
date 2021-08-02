# Name		: Makefile
# Project	: test stdpins
# Author	: Bernd Waldmann
# Created	: 23.09.2018 12:36:02
# Tabsize	: 4
# This Revision: $Id: Makefile 1215 2021-08-02 15:57:00Z  $

## ----- General Flags

PROJECT = test_stdpins
MCU = atmega328p
F_CPU = 8000000
ROMSIZE = 30000
RAMSIZE = 2048

## ----- source files
CPPSOURCES = main.cpp 
CSOURCES = 
ASOURCES = 

SHAREDPATH = ../../
include ../../mk.d/bw-avr-defines.mk
COMMON_Cxx += -Wa,-adhln=$(*F).s -g -fverbose-asm
include ../../mk.d/fuses-ATmega328p-intRC.mk
include ../../mk.d/bw-avr-rules.mk
