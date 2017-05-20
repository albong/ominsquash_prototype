#include "enemy_tables.h"


#include "enemy_00000.h"

void fillEnemyTables(enemy_entity_action_ptr_t **eActionTable,
                    enemy_draw_ptr_t **drawTable,
                    enemy_interact_ptr_t **interactTable, 
                    enemy_takeDamage_ptr_t **takeDamageTable, 
                    enemy_collidePlayer_ptr_t **collidePlayerTable, 
                    enemy_action_ptr_t **actionTable, 
                    size_t *tableSize){
    *tableSize = 1;

    enemy_entity_action_ptr_t *e_at = malloc(sizeof(enemy_entity_action_ptr_t) * 1);
    enemy_draw_ptr_t *dt = malloc(sizeof(enemy_draw_ptr_t) * 1);
    enemy_interact_ptr_t *it = malloc(sizeof(enemy_interact_ptr_t) * 1);
    enemy_takeDamage_ptr_t *tdt = malloc(sizeof(enemy_takeDamage_ptr_t) * 1);
    enemy_collidePlayer_ptr_t *cpt = malloc(sizeof(enemy_collidePlayer_ptr_t) * 1);
    enemy_action_ptr_t *at = malloc(sizeof(enemy_action_ptr_t) * 1);
    
    e_at[00000] = NULL;
    dt[00000] = &enemy_draw_00000;
    it[00000] = NULL;
    tdt[00000] = &enemy_takeDamage_00000;
    cpt[00000] = NULL;
    at[00000] = &enemy_action_00000;

    *eActionTable = e_at;
    *drawTable = dt;
    *interactTable = it;
    *takeDamageTable = tdt;
    *collidePlayerTable = cpt;
    *actionTable = at;
    
}