#Makefile

CC=gcc

CPPFLAGS=
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3 -lm
LDFLAGS=
LDLIBS= 

SRC= useNetwork.c trainNetwork.c executeNetwork.c tools.c main.c

OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: main

main: $(OBJ)


clean:
	$(RM) $(OBJ) $(DEP) main

# END
