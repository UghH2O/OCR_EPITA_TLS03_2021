#Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= `pkg-config --cflags gtk+-3.0` -Wall -Wextra -g -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-3.0` -lSDL_image -lm -lSDL_gfx

SRC= main.c img_processing/processing.c operations/operations.c text_to_char/segmentation.c img_processing/rotation.c neural_network/OCR/network.c neural_network/OCR/execute.c neural_network/OCR/tools.c neural_network/OCR/train.c

OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: main

main: $(OBJ)


clean:
	$(RM) $(OBJ) $(DEP) main text.txt tmp.bmp

# END
