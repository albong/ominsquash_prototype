# Project: omnisquash
# Makefile created by Dev-C++ 5.9.2

CPP      = g++.exe -D__DEBUG__
CC       = gcc.exe -D__DEBUG__
WINDRES  = windres.exe
OBJ      = src/main.o src/init.o src/input.o src/entity.o src/graphics.o src/player.o src/collisions.o src/area.o src/room.o
LINKOBJ  = src/main.o src/init.o src/input.o src/entity.o src/graphics.o src/player.o src/collisions.o src/area.o src/room.o
LIBS     = -L"C:/Program Files (x86)/Dev-Cpp/MinGW64/lib32" -L"C:/Program Files (x86)/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32" -L"D:/SDL-1.2.15/lib" -static-libgcc -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -m32 -g3
INCS     = -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/include" -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include" -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.8.1/include" -I"D:/SDL-1.2.15/include"
CXXINCS  = -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/include" -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include" -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.8.1/include" -I"C:/Program Files (x86)/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.8.1/include/c++"
BIN      = omnisquash.exe
CXXFLAGS = $(CXXINCS) -m32 -g3
CFLAGS   = $(INCS) -m32 -g3
RM       = rm.exe -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LIBS)

src/main.o: src/main.c
	$(CC) -c src/main.c -o src/main.o $(CFLAGS)

src/init.o: src/init.c
	$(CC) -c src/init.c -o src/init.o $(CFLAGS)

src/input.o: src/input.c
	$(CC) -c src/input.c -o src/input.o $(CFLAGS)

src/entity.o: src/entity.c
	$(CC) -c src/entity.c -o src/entity.o $(CFLAGS)

src/graphics.o: src/graphics.c
	$(CC) -c src/graphics.c -o src/graphics.o $(CFLAGS)

src/player.o: src/player.c
	$(CC) -c src/player.c -o src/player.o $(CFLAGS)

src/collisions.o: src/collisions.c
	$(CC) -c src/collisions.c -o src/collisions.o $(CFLAGS)

src/area.o: src/area.c
	$(CC) -c src/area.c -o src/area.o $(CFLAGS)

src/room.o: src/room.c
	$(CC) -c src/room.c -o src/room.o $(CFLAGS)
