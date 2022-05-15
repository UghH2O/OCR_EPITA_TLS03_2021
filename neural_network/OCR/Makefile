#Makefile

CC=gcc

CPPFLAGS=
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= -lm

SRC= execute.c tools.c train.c main.c

OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: main

main: $(OBJ)


clean:
	$(RM) $(OBJ) $(DEP) main

# END
