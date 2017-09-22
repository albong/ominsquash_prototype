#ifndef ENTITY_TABLES_H
#define ENTITY_TABLES_H

#include "../src/entity.h"

//these correspond to the pointers used by the Entity struct
typedef void (*entity_construct_ptr_t)(Entity *);
typedef void (*entity_destruct_ptr_t)(Entity *);
typedef void (*entity_action_ptr_t)(void *, int);
typedef void (*entity_draw_ptr_t)(Entity *, double, double);
typedef void (*entity_interact_ptr_t)(Entity *);

void fillEntityTables(entity_construct_ptr_t **constructTable,
                    entity_destruct_ptr_t **destructTable,
                    entity_action_ptr_t **actionTable, 
                    entity_draw_ptr_t **drawTable, 
                    entity_interact_ptr_t **interactTable, 
                    size_t *tableSize);

#endif