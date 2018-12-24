#! /usr/bin/python2

#################################################
#
# Used to read and clean the text data files, and then create a file detailing
# byte locations, making it easier to read with C.
#
#################################################

import os
import json

BUILD_DIR = os.path.dirname(os.path.realpath(__file__))
TEXT_FILES_DIR = "data/text"
OUTPUT_EXTENSION = ".manifest"


#################################################
# Script start
#################################################
#change directory to script location
os.chdir(os.path.join(BUILD_DIR, TEXT_FILES_DIR))

#get a list of text files
textFileNames = []
for name in os.listdir("."):
    if os.path.isfile(name) and not name[-len(OUTPUT_EXTENSION):] == OUTPUT_EXTENSION:
        textFileNames.append(name)
#process each file
for name in textFileNames:
    fileContents = None
    try:
        with open(name, "rb") as fin:
            fileContents = fin.read()
    except IOError as err:
        print("Couldn't read " + name)
        print(err)

    #try to remove any excess newlines
    fileContents = fileContents.strip()
    while "\n\n\n" in fileContents:
        fileContents = fileContents.replace("\n\n\n", "\n\n")

    #rewrite the clean file
    try:
        with open(name, "wb") as fout:
            fout.write(fileContents)
    except IOError as err:
        print("Couldn't write " + name)
        print(err)

    #pull out ids, and start/end bytes
    idList = []
    startPosition = []
    stopPosition = []
    searchFromIndex = 0

    while searchFromIndex < len(fileContents):
        newlineIndex = fileContents.find("\n", searchFromIndex)
        idList.append(int(fileContents[searchFromIndex: newlineIndex]))
        
        startPosition.append(newlineIndex + 1)
        
        newlineIndex = fileContents.find("\n", newlineIndex + 1)
        
        if newlineIndex != -1:
            stopPosition.append(newlineIndex)
            searchFromIndex = fileContents.find("\n\n", newlineIndex) + 2
        else:
            stopPosition.append(len(fileContents) - 1)
            searchFromIndex = len(fileContents)
       
    #generate the output json
    outputDict = {}
    outputDict["ids"] = idList
    outputDict["start byte"] = startPosition
    outputDict["end byte"] = stopPosition
    try:
        with open(name + OUTPUT_EXTENSION, "w") as fout:
            json.dump(outputDict, fout)
    except IOError as err:
        print("Couldn't write " + name)
        print(err)
