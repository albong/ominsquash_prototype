#ifndef ENEMY_TABLES_H
#define ENEMY_TABLES_H

#include "../src/enemy.h"

//these correspond to the pointers used by the Enemy/Entity struct
typedef void (*enemy_entity_action_ptr_t)(void *, int);
typedef void (*enemy_draw_ptr_t)(Entity *, double, double);
typedef void (*enemy_interact_ptr_t)(Entity *);
typedef int (*enemy_takeDamage_ptr_t)(Enemy *, int);
typedef void (*enemy_collidePlayer_ptr_t)(Enemy *, int);
typedef void (*enemy_action_ptr_t)(Enemy *, int);

void fillEnemyTables(enemy_entity_action_ptr_t **eActionTable,
                    enemy_draw_ptr_t **drawTable,
                    enemy_interact_ptr_t **interactTable, 
                    enemy_takeDamage_ptr_t **takeDamageTable, 
                    enemy_collidePlayer_ptr_t **collidePlayerTable, 
                    enemy_action_ptr_t **actionTable, 
                    size_t *tableSize);

#endif