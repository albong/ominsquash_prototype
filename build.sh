#!/bin/bash
# Project: omnisquash
# Makefile created by Dev-C++ 5.11

CPP="/cygdrive/c/Program\ Files/Dev-Cpp/MinGW64/bin/g++.exe"
CC="/cygdrive/c/Program\ Files/Dev-Cpp/MinGW64/bin/gcc.exe"
WINDRES="windres.exe"
OBJ="src/main.o src/input.o src/entity.o src/graphics.o src/player.o src/collisions.o src/area.o src/room.o src/weapon.o debug/hitbox_drawer.o weapons/sword.o src/enemy.o enemies/octorok.o enemies/enemyloader.o enemies/enemyconstants.o src/door.o entities/skull.o entities/entityconstants.o entities/entityloader.o"
LINKOBJ="src/main.o src/input.o src/entity.o src/graphics.o src/player.o src/collisions.o src/area.o src/room.o src/weapon.o debug/hitbox_drawer.o weapons/sword.o src/enemy.o enemies/octorok.o enemies/enemyloader.o enemies/enemyconstants.o src/door.o entities/skull.o entities/entityconstants.o entities/entityloader.o"
LIBS="-L\"C:/Program\ Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32\" -L\"D:/SDL-1.2.15/lib\" -static-libgcc -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -m32 -g3"
INCS="-I\"C:/Program\ Files/Dev-Cpp/MinGW64/include\" -I\"C:/Program\ Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include\" -I\"C:/Program\ Files/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include\" -I\"D:/SDL-1.2.15/include\""
CXXINCS="-I\"C:/Program Files/Dev-Cpp/MinGW64/include\" -I\"C:/Program\ Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include\" -I\"C:/Program\ Files/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include\" -I\"C:/Program\ Files/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include/c++\""
BIN="omnisquash2.exe"
CXXFLAGS="${CXXINCS} -m32 -g3"
CFLAGS="${INCS} -m32 -g3"
RM="rm.exe -f"

rm ${OBJ}

echo "${CC} -c src/main.c -o src/main.o ${CFLAGS}"
eval "${CC} -c src/main.c -o src/main.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c src/input.c -o src/input.o ${CFLAGS}"
eval "${CC} -c src/input.c -o src/input.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c src/entity.c -o src/entity.o ${CFLAGS}"
eval "${CC} -c src/entity.c -o src/entity.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c src/graphics.c -o src/graphics.o ${CFLAGS}"
eval "${CC} -c src/graphics.c -o src/graphics.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c src/player.c -o src/player.o ${CFLAGS}"
eval "${CC} -c src/player.c -o src/player.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c src/collisions.c -o src/collisions.o ${CFLAGS}"
eval "${CC} -c src/collisions.c -o src/collisions.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c src/area.c -o src/area.o ${CFLAGS}"
eval "${CC} -c src/area.c -o src/area.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c src/room.c -o src/room.o ${CFLAGS}"
eval "${CC} -c src/room.c -o src/room.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c src/weapon.c -o src/weapon.o ${CFLAGS}"
eval "${CC} -c src/weapon.c -o src/weapon.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c debug/hitbox_drawer.c -o debug/hitbox_drawer.o ${CFLAGS}"
eval "${CC} -c debug/hitbox_drawer.c -o debug/hitbox_drawer.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c weapons/sword.c -o weapons/sword.o ${CFLAGS}"
eval "${CC} -c weapons/sword.c -o weapons/sword.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c src/enemy.c -o src/enemy.o ${CFLAGS}"
eval "${CC} -c src/enemy.c -o src/enemy.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c enemies/octorok.c -o enemies/octorok.o ${CFLAGS}"
eval "${CC} -c enemies/octorok.c -o enemies/octorok.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c enemies/enemyloader.c -o enemies/enemyloader.o ${CFLAGS}"
eval "${CC} -c enemies/enemyloader.c -o enemies/enemyloader.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c enemies/enemyconstants.c -o enemies/enemyconstants.o ${CFLAGS}"
eval "${CC} -c enemies/enemyconstants.c -o enemies/enemyconstants.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c src/door.c -o src/door.o ${CFLAGS}"
eval "${CC} -c src/door.c -o src/door.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c entities/skull.c -o entities/skull.o ${CFLAGS}"
eval "${CC} -c entities/skull.c -o entities/skull.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c entities/entityconstants.c -o entities/entityconstants.o ${CFLAGS}"
eval "${CC} -c entities/entityconstants.c -o entities/entityconstants.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi

echo "${CC} -c entities/entityloader.c -o entities/entityloader.o ${CFLAGS}"
eval "${CC} -c entities/entityloader.c -o entities/entityloader.o ${CFLAGS}"
if [ $? -ne 0 ]; then
    exit 1
fi


echo "${CC} ${LINKOBJ} -o ${BIN} ${LIBS}"
eval "${CC} ${LINKOBJ} -o ${BIN} ${LIBS}"
