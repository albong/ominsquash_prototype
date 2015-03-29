#include "room.h"

int getFlag(Room *room, int index, int flagNum){
    return (room->flags[index] >> flagNum) & 1;
}

void setFlag(Room *room, int index, int flagNum){
    room->flags[index] |= 1 << flagNum;
}

void clearFlag(Room *room, int index, int flagNum){
    room->flags[index] &= ~(1 << flagNum);
}

void toggleFlag(Room *room, int index, int flagNum){
    room->flags[index] ^= 1 << flagNum;
}
