#include "entity_tables.h"

#include "entity_00000.h"

#include <stdio.h>

// void fillEntityTables(entity_action_ptr_t *actionTable, size_t *tableSize){
void fillEntityTables(entity_action_ptr_t **actionTable, entity_draw_ptr_t **drawTable, entity_interact_ptr_t **interactTable, size_t *tableSize){
    *tableSize = 1;
    
    // *actionTable = malloc(sizeof(entity_action_ptr_t) * 1);
    // *drawTable = malloc(sizeof(entity_draw_ptr_t) * 1);
    // *interactTable = malloc(sizeof(entity_interact_ptr_t) * 1);
    entity_action_ptr_t *at = malloc(sizeof(entity_action_ptr_t) * 1);
    entity_draw_ptr_t *dt = malloc(sizeof(entity_action_ptr_t) * 1);
    entity_interact_ptr_t *it = malloc(sizeof(entity_action_ptr_t) * 1);
    
    at[00000] = NULL;
    dt[00000] = NULL;
    it[00000] = &entity_interact_00000;
    
    *actionTable = at;
    *drawTable = dt;
    *interactTable = it;
}