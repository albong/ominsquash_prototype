#! /usr/bin/python

#################################################
#
# Used to create function tables needed by Omnisquash
#
#################################################

#just entities for now - find all c files, generate new entity_tables.c

import os

BUILD_DIR = os.path.dirname(os.path.realpath(__file__))
ENTITIES_DIR = "entities/"
ENTITY_TABLES = "entity_tables.c"

def clean():
    """remove existing table files"""
    try:
        os.remove("entities/entity_tables.c")
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
        hasAction = False
        hasDraw = False
        hasInteract = False
        ID = h[7:-2]
        
        try:
            with open(ENTITIES_DIR + h, "r") as fin:
                for line in fin:
                    #format the line for easier parsing
                    line = line.strip()
                    line = " ".join(line.split())
                    
                    #check for methods
                    if line.startswith("void entity_action_" + ID):
                        hasAction = True
                    if line.startswith("void entity_draw_" + ID):
                        hasDraw = True
                    if line.startswith("void entity_interact_" + ID):
                        hasInteract = True
                        
                result[ID] = (hasAction, hasDraw, hasInteract)
                
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
    tableSize = int(headerList[-1][7:-2]) + 1
    fout.write("void fillEntityTables(entity_action_ptr_t **actionTable, entity_draw_ptr_t **drawTable, entity_interact_ptr_t **interactTable, size_t *tableSize){\n")
    fout.write("    *tableSize = " + str(tableSize) + ";\n")
    fout.write("\n")
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
            hasAction, hasDraw, hasInteract = methodDict[ID]
        
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
    fout.write("    *actionTable = at;\n")
    fout.write("    *drawTable = dt;\n")
    fout.write("    *interactTable = it;\n")
    fout.write("}\n")
    

#################################################
# Script start
#################################################
#change directory to script location
os.chdir(BUILD_DIR)

#remove the old files
clean()

#get the files to not include
toIgnore = findIgnoreList()

#find all header files of each type with matching source file, determine contents
entityHeaders = findFiles(toIgnore, ENTITIES_DIR)
entityHeaderMethods = checkEntityHeaderMethods(entityHeaders)

#write the table file
try:
    with open(ENTITIES_DIR + ENTITY_TABLES, "w") as fout:
        writeEntityTableFile(fout, entityHeaders, entityHeaderMethods)
except IOError:
    print "Couldn't open " + ENTITY_TABLES + " for writing"
    exit(1)