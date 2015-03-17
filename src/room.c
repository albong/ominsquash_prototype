#include "room.h"

int getFlag(Room *room, int index, int flagNum){
    return (room->flags[index] >> flagNum) & 1;
}
