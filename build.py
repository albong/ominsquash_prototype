#! /usr/bin/python

#################################################
#
# Used for building the Omnisquash engine
#
#################################################

import os
import platform
import subprocess

BUILD_DIR = os.path.dirname(os.path.realpath(__file__))
PLATFORM=""
CC=""
LIBS=""
INCS=""
CFLAGS=""
BIN=""

def configure():
    """configure the compilation options"""
    global PLATFORM
    global CC
    global LIBS
    global INCS
    global CFLAGS
    global BIN

    #determine the platform and set things accordingly
    if "cygwin" in platform.system().lower():
        PLATFORM = "CYGWIN"
        CC = "/cygdrive/c/Program\ Files/Dev-Cpp/MinGW64/bin/gcc.exe"
        LIBS = "-L\"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32\" "\
                "-L\"D:/SDL-1.2.15/lib\" "\
                "-static-libgcc "\
                "-lmingw32 "\
                "-lSDLmain "\
                "-lSDL "\
                "-lSDL_image "\
                "-lSDL_mixer "\
                "-lSDL_ttf "\
                "-m32 "\
                "-g3"
        INCS = "-I\"C:/Program Files/Dev-Cpp/MinGW64/include\" "\
                "-I\"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include\" "\
                "-I\"C:/Program Files/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include\" "\
                "-I\"D:/SDL-1.2.15/include\""
        BIN = "omnisquash.exe"
    elif "windows" in platform.system().lower() or "win32" in platform.system().lower():
        PLATFORM = "WINDOWS"
        CC = "c:\\Program Files\\Dev-Cpp\\MinGW64\\bin\\gcc.exe"
        LIBS = "-L\"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32\" "\
                "-L\"D:/SDL-1.2.15/lib\" "\
                "-static-libgcc "\
                "-lmingw32 "\
                "-lSDLmain "\
                "-lSDL "\
                "-lSDL_image "\
                "-lSDL_mixer "\
                "-lSDL_ttf "\
                "-m32 "\
                "-g3"
        INCS = "-I\"C:/Program Files/Dev-Cpp/MinGW64/include\" "\
                "-I\"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include\" "\
                "-I\"C:/Program Files/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include\" "\
                "-I\"D:/SDL-1.2.15/include\""
        BIN = "omnisquash.exe"
    elif "mac" in platform.system().lower() or "darwin" in platform.system().lower():
        PLATFORM = "MAC"
        CC = "gcc"
        LIBS = "LIBS"
        INCS = "INCS"
        BIN = "omnisquash"
    else:
        print "Platform could not be determined or is unsupported."
        print "Build cancelled."
        exit(1)

    LIBS = "-L\"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32\" -L\"D:/SDL-1.2.15/lib\" -static-libgcc -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -m32 -g3"
    INCS = "-I\"C:/Program Files/Dev-Cpp/MinGW64/include\" -I\"C:/Program\ Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include\" -I\"C:/Program Files/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include\" -I\"D:/SDL-1.2.15/include\""
    CFLAGS = INCS + " -m32 -g3"

def findIgnore():
    """read the file build.ignore if present and use the list to exclude files
    from the build"""
    toIgnore = []
    try:
        with open("build.ignore", "r") as fin:
            for line in fin:
                line = line.strip()
                if len(line) > 0 and line[0] != "#":
                    line.replace("/", os.sep)
                    toIgnore.append(BUILD_DIR + line)
    except IOError:
        pass
                
    return toIgnore
    
def findFiles(toIgnore):
    """locate c files"""
    global PLATFORM
    cFiles = []
    oFiles = []
    for (dirpath, dirnames, filenames) in os.walk(BUILD_DIR):
        if dirpath in toIgnore:
            continue
            
        for name in filenames:
            if len(name) > 2 and name[-2:] == ".c":
                cf = os.path.join(dirpath, name)
                of = os.path.join(dirpath, name[:-2] + ".o")
                
                if cf in toIgnore:
                    continue
                
                cFiles.append(cf)
                oFiles.append(of)
    return (cFiles, oFiles)

def clean(files):
    """remove existing object files"""
    for f in files:
        try:
            os.remove(f)
        except OSError:
            pass
    try:
        os.remove(os.path.join(BUILD_DIR, BIN))
    except OSError:
        pass

#################################################
# Script start
#################################################

os.chdir(BUILD_DIR)

#set compilation variables
configure()

#load files and directories to be ignored
toIgnore = findIgnore()

#locate files
cFiles, oFiles = findFiles(toIgnore)

#remove object files for c files
clean(oFiles)

#build the object files
for i in range(0, len(cFiles)):
    if PLATFORM == "CYGWIN":
        compileCmd = CC + " -c " + cFiles[i].replace(BUILD_DIR + "/","") + " -o " + oFiles[i].replace(BUILD_DIR + "/","") + " " + CFLAGS
        print compileCmd
        compileCmd = "eval \"" + compileCmd.replace("\"", "\\\"") + "\""
        subprocess.call(compileCmd, shell=True)
    else:
        compileCmd = CC + " -c " + cFiles[i] + " -o " + oFiles[i] + " " + CFLAGS
        print compileCmd
        subprocess.call(compileCmd)

#link the files into the executable
#linkCmd = CC + " " +  " ".join(oFiles) + " -o " + BIN + " " + LIBS
#print linkCmd
#subprocess.call(compileCmd, shell=True)
if PLATFORM == "CYGWIN":
    linkCmd = CC + " " +  " ".join([f.replace(BUILD_DIR + "/","") for f in oFiles]) + " -o " + BIN + " " + LIBS
    print linkCmd
    linkCmd = "eval \"" + linkCmd.replace("\"", "\\\"") + "\""
    subprocess.call(linkCmd, shell=True)
else:
    linkCmd = CC + " " +  " ".join(oFiles) + " -o " + BIN + " " + LIBS
    print linkCmd
    subprocess.call(linkCmd)

