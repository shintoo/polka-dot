CC=gcc
SRC= src/main.c src/polka-dot.c
HDR= src/polka-dot.h
LDFLAGS= -std=c11

all: polka-dot

polka-dot: $(SRC) $(HDR)
	$(CC) -o polka-dot $(SRC) $(LDFLAGS)
