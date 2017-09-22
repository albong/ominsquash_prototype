#! /usr/bin/python

#################################################
#
# Used to create function tables needed by Omnisquash
#
# Certainly could be improved and duplication removed, but the current format is clear and easy to read
#
#################################################

import os

BUILD_DIR = os.path.dirname(os.path.realpath(__file__))
ENTITIES_DIR = "entities/"
ENTITY_TABLES = "entity_tables.c"
WEAPONS_DIR = "weapons/"
WEAPON_TABLES = "weapon_tables.c"
ENEMIES_DIR = "enemies/"
ENEMY_TABLES = "enemy_tables.c"

def clean():
    """remove existing table files"""
    try:
        os.remove(ENTITIES_DIR + ENTITY_TABLES)
        os.remove(WEAPONS_DIR + WEAPON_TABLES)
        os.remove(ENEMIES_DIR + ENEMY_TABLES)
    except OSError:
        pass

def findIgnoreList():
    """read the file build.ignore if present and use the list to exclude files
    from the build"""
    toIgnore = []
    try:
        with open(".build.ignore", "r") as fin:
            for line in fin:
                line = line.strip()
                if len(line) > 0 and line[0] != "#":
                    line.replace("/", os.sep)
                    toIgnore.append(BUILD_DIR + line)
    except IOError:
        pass
                
    return toIgnore

def findFiles(toIgnore, folderName):
    """locate c and h files, return lists of header files"""
    files = []
    for (dirpath, dirnames, filenames) in os.walk(BUILD_DIR + "/" + folderName):
        if dirpath in toIgnore:
            continue
    
        for name in filenames:
            if len(name) > 2 and (name[-2:] == ".c" or name[-2:] == ".h"):
                if os.path.join(dirpath, name) not in toIgnore:
                    files.append(name)
    
    #take from list only header files that have a corresponding source file
    headerFiles = []
    for f in files:
        if f[-2:] == ".h" and f[:-2]+".c" in files:
            headerFiles.append(f)
    return sorted(headerFiles)
    
def checkEntityHeaderMethods(headerList):
    """Check each header file line by line to see if it contains lines of the correct format
    returns a dict of tuples ID : (hasAction, hasDraw, hasInteract)"""
    result = {}
    
    for h in headerList:
        hasConstruct = False
        hasDestruct = False
        hasAction = False
        hasDraw = False
        hasInteract = False
        ID = h[len("entity_"):-2]
        
        try:
            with open(ENTITIES_DIR + h, "r") as fin:
                for line in fin:
                    #format the line for easier parsing
                    line = line.strip()
                    line = " ".join(line.split())
                    
                    #check for methods
                    if line.startswith("void entity_construct_" + ID):
                        hasConstruct = True
                    if line.startswith("void entity_destruct_" + ID):
                        hasDestruct = True
                    if line.startswith("void entity_action_" + ID):
                        hasAction = True
                    if line.startswith("void entity_draw_" + ID):
                        hasDraw = True
                    if line.startswith("void entity_interact_" + ID):
                        hasInteract = True
                        
                result[ID] = (hasConstruct, hasDestruct, hasAction, hasDraw, hasInteract)
                
        except IOError:
            print "Error reading file " + ENTITIES_DIR + h
            exit(1)
            
    return result
    
def writeEntityTableFile(fout, headerList, methodDict):
    """write the entity tables file"""
    #first, write the header
    fout.write("#include \"entity_tables.h\"\n\n")
    fout.write("\n")
    for h in headerList:
        fout.write("#include \"" + h + "\"\n")
    fout.write("\n")
    
    #now write the beginning of the method
    tableSize = int(headerList[-1][len("entity_"):-2]) + 1
    fout.write("void fillEntityTables(entity_construct_ptr_t **constructTable, entity_destruct_ptr_t **destructTable, entity_action_ptr_t **actionTable, entity_draw_ptr_t **drawTable, entity_interact_ptr_t **interactTable, size_t *tableSize){\n")
    fout.write("    *tableSize = " + str(tableSize) + ";\n")
    fout.write("\n")
    fout.write("    entity_construct_ptr_t *ct = malloc(sizeof(entity_construct_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("    entity_destruct_ptr_t *det = malloc(sizeof(entity_destruct_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("    entity_action_ptr_t *at = malloc(sizeof(entity_action_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("    entity_draw_ptr_t *dt = malloc(sizeof(entity_draw_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("    entity_interact_ptr_t *it = malloc(sizeof(entity_interact_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("\n")
    
    #populate the tables
    for i in range(0, tableSize):
        ID = str(i).zfill(5)
        hasAction = False
        hasDraw = False
        hasInteract = False
        
        if ID in methodDict:
            hasConstruct, hasDestruct, hasAction, hasDraw, hasInteract = methodDict[ID]
        
        #construct
        if hasConstruct:
            fout.write("    ct[" + ID + "] = &entity_construct_" + ID + ";\n")
        else:
            fout.write("    ct[" + ID + "] = NULL;\n")
            
        #destruct
        if hasDestruct:
            fout.write("    det[" + ID + "] = &entity_destruct_" + ID + ";\n")
        else:
            fout.write("    det[" + ID + "] = NULL;\n")
        
        #action
        if hasAction:
            fout.write("    at[" + ID + "] = &entity_action_" + ID + ";\n")
        else:
            fout.write("    at[" + ID + "] = NULL;\n")
        
        #draw
        if hasDraw:
            fout.write("    dt[" + ID + "] = &entity_draw_" + ID + ";\n")
        else:
            fout.write("    dt[" + ID + "] = NULL;\n")
        
        #interact
        if hasInteract:
            fout.write("    it[" + ID + "] = &entity_interact_" + ID + ";\n")
        else:
            fout.write("    it[" + ID + "] = NULL;\n")
    fout.write("\n")
    
    #write the end of the method
    fout.write("    *constructTable = ct;\n")
    fout.write("    *destructTable = det;\n")
    fout.write("    *actionTable = at;\n")
    fout.write("    *drawTable = dt;\n")
    fout.write("    *interactTable = it;\n")
    fout.write("}\n")
    
def checkWeaponHeaderMethods(headerList):
    """Check each header file line by line to see if it contains lines of the correct format
    returns a dict of tuples; ID : (hasCreate, hasCollide)"""
    result = {}
    
    for h in headerList:
        hasCreate = False
        hasCollide = False
        ID = h[len("weapon_"):-2]
        
        try:
            with open(WEAPONS_DIR + h, "r") as fin:
                for line in fin:
                    #format the line for easier parsing
                    line = line.strip()
                    line = " ".join(line.split())
                    
                    #check for methods
                    if line.startswith("Weapon *weapon_create_" + ID):
                        hasCreate = True
                    if line.startswith("void weapon_collide_" + ID):
                        hasCollide = True
                    
                result[ID] = (hasCreate, hasCollide)
                
        except IOError:
            print "Error reading file " + WEAPONS_DIR + h
            exit(1)
            
    return result

def writeWeaponTableFile(fout, headerList, methodDict):
    """write the entity tables file"""
    #first, write the header
    fout.write("#include \"weapon_tables.h\"\n\n")
    fout.write("\n")
    for h in headerList:
        fout.write("#include \"" + h + "\"\n")
    fout.write("\n")
    
    #now write the beginning of the method
    tableSize = int(headerList[-1][len("weapon_"):-2]) + 1
    fout.write("void fillWeaponTables(weapon_create_ptr_t **createTable, weapon_collide_ptr_t **collideTable, size_t *tableSize){\n")
    fout.write("    *tableSize = " + str(tableSize) + ";\n")
    fout.write("\n")
    fout.write("    weapon_create_ptr_t *createT = malloc(sizeof(weapon_create_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("    weapon_collide_ptr_t *collideT = malloc(sizeof(weapon_collide_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("\n")
    
    #populate the tables
    for i in range(0, tableSize):
        ID = str(i).zfill(5)
        hasCreate = True
        hasCollide = True

        if ID in methodDict:
            hasCreate, hasCollide = methodDict[ID]
        
        #create
        if hasCreate:
            fout.write("    createT[" + ID + "] = &weapon_create_" + ID + ";\n")
        else:
            fout.write("    createT[" + ID + "] = NULL;\n")
        
        #collide
        if hasCollide:
            fout.write("    collideT[" + ID + "] = &weapon_collide_" + ID + ";\n")
        else:
            fout.write("    collideT[" + ID + "] = NULL;\n")
        
    fout.write("\n")
    
    #write the end of the method
    fout.write("    *createTable = createT;\n")
    fout.write("    *collideTable = collideT;\n")
    fout.write("}\n")

def checkEnemyHeaderMethods(headerList):
    """Check each header file line by line to see if it contains lines of the correct format
    returns a dict of tuples ID : (hasEAction, hasDraw, hasInteract, hasTakeDamage, hasCollide, hasAction)"""
    result = {}
    
    for h in headerList:
        hasEConstruct = False
        hasEDestruct = False
        hasEAction = False
        hasDraw = False
        hasInteract = False
        hasTakeDamage = False
        hasCollide = False
        hasAction = False
        ID = h[len("enemy_"):-2]
        
        try:
            with open(ENEMIES_DIR + h, "r") as fin:
                for line in fin:
                    #format the line for easier parsing
                    line = line.strip()
                    line = " ".join(line.split())
                    
                    #check for methods
                    if line.startswith("void enemy_entity_construct_" + ID):
                        hasEConstruct = True
                    if line.startswith("void enemy_entity_destruct_" + ID):
                        hasEDestruct = True
                    if line.startswith("void enemy_entity_action_" + ID):
                        hasEAction = True
                    if line.startswith("void enemy_draw_" + ID):
                        hasDraw = True
                    if line.startswith("void enemy_interact_" + ID):
                        hasInteract = True
                    if line.startswith("int enemy_takeDamage_" + ID):
                        hasTakeDamage = True
                    if line.startswith("void enemy_collidePlayer_" + ID):
                        hasCollide = True
                    if line.startswith("void enemy_action_" + ID):
                        hasAction = True
                        
                result[ID] = (hasEConstruct, hasEDestruct, hasEAction, hasDraw, hasInteract, hasTakeDamage, hasCollide, hasAction)
                
        except IOError:
            print "Error reading file " + ENTITIES_DIR + h
            exit(1)
            
    return result    

def writeEnemyTableFile(fout, headerList, methodDict):
    """write the entity tables file"""
    #first, write the header
    fout.write("#include \"enemy_tables.h\"\n\n")
    fout.write("\n")
    for h in headerList:
        fout.write("#include \"" + h + "\"\n")
    fout.write("\n")
    
    #now write the beginning of the method
    tableSize = int(headerList[-1][len("enemy_"):-2]) + 1
    fout.write("void fillEnemyTables(enemy_entity_construct_ptr_t **eConstructTable,\n")
    fout.write("                    enemy_entity_destruct_ptr_t **eDestructTable,\n")
    fout.write("                    enemy_entity_action_ptr_t **eActionTable,\n")
    fout.write("                    enemy_draw_ptr_t **drawTable,\n")
    fout.write("                    enemy_interact_ptr_t **interactTable,\n")
    fout.write("                    enemy_takeDamage_ptr_t **takeDamageTable,\n")
    fout.write("                    enemy_collidePlayer_ptr_t **collidePlayerTable,\n")
    fout.write("                    enemy_action_ptr_t **actionTable,\n")
    fout.write("                    size_t *tableSize){\n")
    fout.write("    *tableSize = " + str(tableSize) + ";\n")
    fout.write("\n")
    
    fout.write("    enemy_entity_construct_ptr_t *e_ct = malloc(sizeof(enemy_entity_action_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("    enemy_entity_destruct_ptr_t *e_det = malloc(sizeof(enemy_entity_action_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("    enemy_entity_action_ptr_t *e_at = malloc(sizeof(enemy_entity_action_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("    enemy_draw_ptr_t *dt = malloc(sizeof(enemy_draw_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("    enemy_interact_ptr_t *it = malloc(sizeof(enemy_interact_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("    enemy_takeDamage_ptr_t *tdt = malloc(sizeof(enemy_takeDamage_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("    enemy_collidePlayer_ptr_t *cpt = malloc(sizeof(enemy_collidePlayer_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("    enemy_action_ptr_t *at = malloc(sizeof(enemy_action_ptr_t) * " + str(tableSize) + ");\n")
    fout.write("\n")
    
    #populate the tables
    for i in range(0, tableSize):
        ID = str(i).zfill(5)
        hasEAction = False
        hasDraw = False
        hasInteract = False
        hasTakeDamage = False
        hasCollide = False
        hasAction = False
        
        if ID in methodDict:
            hasEConstruct, hasEDestruct, hasEAction, hasDraw, hasInteract, hasTakeDamage, hasCollide, hasAction = methodDict[ID]
        
        #entity_construct
        if hasEConstruct:
            fout.write("    e_ct[" + ID + "] = &enemy_entity_construct_" + ID + ";\n")
        else:
            fout.write("    e_ct[" + ID + "] = NULL;\n")
        
        #entity_destruct
        if hasEDestruct:
            fout.write("    e_det[" + ID + "] = &enemy_entity_destruct_" + ID + ";\n")
        else:
            fout.write("    e_det[" + ID + "] = NULL;\n")
        
        #entity_action
        if hasEAction:
            fout.write("    e_at[" + ID + "] = &enemy_entity_action_" + ID + ";\n")
        else:
            fout.write("    e_at[" + ID + "] = NULL;\n")
        
        #draw
        if hasDraw:
            fout.write("    dt[" + ID + "] = &enemy_draw_" + ID + ";\n")
        else:
            fout.write("    dt[" + ID + "] = NULL;\n")
        
        #interact
        if hasInteract:
            fout.write("    it[" + ID + "] = &enemy_interact_" + ID + ";\n")
        else:
            fout.write("    it[" + ID + "] = NULL;\n")
            
        #takeDamage
        if hasTakeDamage:
            fout.write("    tdt[" + ID + "] = &enemy_takeDamage_" + ID + ";\n")
        else:
            fout.write("    tdt[" + ID + "] = NULL;\n")
            
        #collidePlayer
        if hasCollide:
            fout.write("    cpt[" + ID + "] = &enemy_collidePlayer_" + ID + ";\n")
        else:
            fout.write("    cpt[" + ID + "] = NULL;\n")
            
        #action
        if hasAction:
            fout.write("    at[" + ID + "] = &enemy_action_" + ID + ";\n")
        else:
            fout.write("    at[" + ID + "] = NULL;\n")
    fout.write("\n")
    
    #write the end of the method
    fout.write("    *eConstructTable = e_ct;\n")
    fout.write("    *eDestructTable = e_det;\n")
    fout.write("    *eActionTable = e_at;\n")
    fout.write("    *drawTable = dt;\n")
    fout.write("    *interactTable = it;\n")
    fout.write("    *takeDamageTable = tdt;\n")
    fout.write("    *collidePlayerTable = cpt;\n")
    fout.write("    *actionTable = at;\n")
    fout.write("}\n")
    
    
#################################################
# Script start
#################################################
#change directory to script location
os.chdir(BUILD_DIR)

#remove the old files - this is important, since this prevents picking up the table files when
#searching for files to put in the table files
clean()

#get the files to not include
toIgnore = findIgnoreList()

#find all header files of each type with matching source file, determine contents
entityHeaders = findFiles(toIgnore, ENTITIES_DIR)
entityHeaderMethods = checkEntityHeaderMethods(entityHeaders)

weaponHeaders = findFiles(toIgnore, WEAPONS_DIR)
weaponHeaderMethods = checkWeaponHeaderMethods(weaponHeaders)

enemyHeaders = findFiles(toIgnore, ENEMIES_DIR)
enemyHeaderMethods = checkEnemyHeaderMethods(enemyHeaders)

#write the table files
try:
    with open(ENTITIES_DIR + ENTITY_TABLES, "w") as fout:
        writeEntityTableFile(fout, entityHeaders, entityHeaderMethods)
except IOError:
    print "Couldn't open " + ENTITY_TABLES + " for writing"
    exit(1)

try:
    with open(WEAPONS_DIR + WEAPON_TABLES, "w") as fout:
        writeWeaponTableFile(fout, weaponHeaders, weaponHeaderMethods)
except IOError:
    print "Couldn't open " + WEAPON_TABLES + " for writing"
    exit(1)
    
try:
    with open(ENEMIES_DIR + ENEMY_TABLES, "w") as fout:
        writeEnemyTableFile(fout, enemyHeaders, enemyHeaderMethods)
except IOError:
    print "Couldn't open " + ENEMY_TABLES + " for writing"
    exit(1)
    