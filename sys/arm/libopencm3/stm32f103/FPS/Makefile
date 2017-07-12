##
##

VPATH		=  ../common
#CFLAGS		+= -save-temps -fverbose-asm

BINARY		=  fps
OBJS		+= u8x8cb.o timer.o delay.o

DEFS		+= -I../common

U8G2_DIR	=  ../../../../../csrc
U8G2SRC		:= $(wildcard $(U8G2_DIR)/*.c)
OBJS		+= $(U8G2SRC:.c=.o)
DEFS		+= -I$(U8G2_DIR)

DEVICE		=  stm32f103c8t6
OPENCM3_DIR	=  $(HOME)/src/stm32/libopencm3
LDFLAGS		+= --specs=nano.specs

include ../libopencm3.target.mk
