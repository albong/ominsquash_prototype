#include "area_reader.h"
#include "area.h"
#include "room.h"
#include "constants.h"

#include "../lib/cJSON/cJSON.h"
#include <stdio.h>
#include <string.h>

static cJSON *readFile(char *filename);

//read in a JSON file to a cJSON object
cJSON *readFileToJson(char *filename){
    //More or less copied and pasted from the cJSON examples
    
    cJSON *result;
    FILE *f;
    long len;
    char *data;

    // open in read binary mode
    f = fopen(filename,"rb");
    // get the length
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    data = (char*)malloc(len + 1);

    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    result = cJSON_Parse(data);
    //Should this be freed after parsing the json?
    free(data);
    return result;
}

Area *loadAreaFromFile(char *filename, Area *result){
    //init the area
    if (result == NULL){
        result = malloc(sizeof(Area));
    }
    init_Area(result);
    
    //read in the given file to a cJSON object
    cJSON *root = readFileToJson(filename);
    if (!root){
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        free(result);
        return NULL;
    }
    
    //fill in the object
    result->tilesetName = strdup(cJSON_GetObjectItem(root, "tilesheet")->valuestring);
    // result->numRooms = cJSON_GetObjectItem(root, "number of rooms")->valueint;
    
    //load up the rooms
    cJSON *roomList = cJSON_GetObjectItem(root, "rooms");
    result->numRooms = cJSON_GetArraySize(roomList);
    result->roomList = malloc(sizeof(Room *) * result->numRooms);
    
    Room *currRoom;
    cJSON *arrY, *arrX, *jsonRoom;
    size_t i;
    int j, k;
    for (i = 0; i < result->numRooms; i++){
        result->roomList[i] = init_Room(malloc(sizeof(Room)));
        currRoom = result->roomList[i];
        jsonRoom = cJSON_GetArrayItem(roomList, i);
        
        //allocate for the arrays of tiles and flags
        currRoom->tileIndices = (int *) malloc(sizeof(int) * ROOM_HEIGHT * ROOM_WIDTH);
        currRoom->flags = (uint32_t *) malloc(sizeof(uint32_t) * ROOM_HEIGHT * ROOM_WIDTH);
        
        //set the connecting rooms
        currRoom->connectingRooms[0] = cJSON_GetObjectItem(jsonRoom, "up connecting room")->valueint;
        currRoom->connectingRooms[1] = cJSON_GetObjectItem(jsonRoom, "down connecting room")->valueint;
        currRoom->connectingRooms[2] = cJSON_GetObjectItem(jsonRoom, "left connecting room")->valueint;
        currRoom->connectingRooms[3] = cJSON_GetObjectItem(jsonRoom, "right connecting room")->valueint;
        
        //set the tile indices
        arrY = cJSON_GetObjectItem(jsonRoom, "tile layout");
        for (j = 0; j < cJSON_GetArraySize(arrY); j++){
            arrX = cJSON_GetArrayItem(arrY, j);
            for (i = 0; i < cJSON_GetArraySize(arrX); i++){
                currRoom->tileIndices[i + (j * ROOM_WIDTH)] = cJSON_GetArrayItem(arrX, i)->valueint;
            }
        }
        
        //set the flags for the walls
        arrY = cJSON_GetObjectItem(jsonRoom, "wall layout");
        for (j = 0; j < cJSON_GetArraySize(arrY); j++){
            arrX = cJSON_GetArrayItem(arrY, j);
            for (i = 0; i < cJSON_GetArraySize(arrX); i++){
                if (cJSON_GetArrayItem(arrX, i)->valueint){
                    setFlag(currRoom, i + (j * ROOM_WIDTH), ROOMF_IMPASSABLE);
                }
            }
        }
        generateWallList(currRoom, TILE_SIZE);
        
        currRoom->numEntities = 0;
        currRoom->numEnemies = 0;
    }
    result->currentRoom = result->roomList[0];
    
    //free and return
    cJSON_Delete(root);
    return result;
}
