#! /usr/bin/python

#################################################
#
# Used for creating the files necessary for Dev-
# C++ to load Omnisquash's files, which is
# useful for the graphical debugger
#
# Seems to work, despite protestations by Dev-
# C++ about missing files
#
#################################################

import os
import argparse

BUILD_DIR = os.path.dirname(os.path.realpath(__file__))
DEV_FILE = "omnisquash.dev"

def findFiles(toIgnore):
    """locate c and h files"""
    global PLATFORM
    files = []
    for (dirpath, dirnames, filenames) in os.walk(BUILD_DIR):
        if dirpath in toIgnore:
            continue
    
        for name in filenames:
            if len(name) > 2 and (name[-2:] == ".c" or name[-2:] == ".h"):
                if os.path.join(dirpath, name) in toIgnore:
                    continue
            
                folder = dirpath.replace(BUILD_DIR + "/","")
                files.append((folder.replace("/", "\\") + "\\" + name, folder))
    return files

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
    
def clean():
    """remove existing Dev-C++ files"""
    try:
        os.remove("Makefile.win")
    except OSError:
        pass

    try:
        os.remove("omnisquash.layout")
    except OSError:
        pass
        
    try:
        os.remove("omnisquash.dev")
    except OSError:
        pass
        
        
def writeHeader(oFile, folders, numUnits):
    """write the header for the project file"""
    oFile.write(
    "[Project]\n" +
    "FileName=omnisquash.dev\n" +
    "Name=omnisquash\n" +
    "Type=1\n" +
    "Ver=2\n" +
    "ObjFiles=\n" +
    "Includes=\n" +
    "Libs=\n" +
    "PrivateResource=\n" +
    "ResourceIncludes=\n" +
    "MakeIncludes=\n" +
    "Compiler=\n" +
    "CppCompiler=\n" +
    "Linker=-lmingw32_@@_-lSDLmain_@@_-lSDL_@@_-lSDL_image_@@_-lSDL_mixer_@@_-lSDL_ttf_@@_\n" +
    "IsCpp=0\n" +
    "Icon=\n" +
    "ExeOutput=\n" +
    "ObjectOutput=\n" +
    "LogOutput=\n" +
    "LogOutputEnabled=0\n" +
    "OverrideOutput=0\n" +
    "OverrideOutputName=omnisquash.exe\n" +
    "HostApplication=\n" +
    "UseCustomMakefile=0\n" +
    "CustomMakefile=\n" +
    "CommandLine=\n" +
    "Folders=" + ",".join(folders) + "\n" +
    "IncludeVersionInfo=0\n" +
    "SupportXPThemes=0\n" +
    "CompilerSet=4\n" +
    "CompilerSettings=0000000100000000001000000\n" +
    "UnitCount=" + str(numUnits) + "\n" +
    "\n" + 
    "[VersionInfo]\n" +
    "Major=1\n" +
    "Minor=0\n" +
    "Release=0\n" +
    "Build=0\n" +
    "LanguageID=1033\n" +
    "CharsetID=1252\n" +
    "CompanyName=\n" +
    "FileVersion=1.0.0.0\n" +
    "FileDescription=Developed using the Dev-C++ IDE\n" +
    "InternalName=\n" +
    "LegalCopyright=\n" +
    "LegalTrademarks=\n" +
    "OriginalFilename=\n" +
    "ProductName=\n" +
    "ProductVersion=1.0.0.0\n" +
    "AutoIncBuildNr=0\n" +
    "SyncProduct=1\n" +
    "\n")

def writeUnits(oFile, files):
    """write the file information to the project file"""
    counter = 1
    for fn,fo in files:
        oFile.write(
        "[Unit" + str(counter) +"]\n" +
        "FileName=" + fn + "\n" +
        "CompileCpp=0\n" + 
        "Folder=" + fo + "\n" +
        "Compile=1\n" + 
        "Link=1\n" +
        "Priority=1000\n" +
        "OverrideBuildCmd=0\n" +
        "BuildCmd=\n" +
        "\n")
        counter += 1
#################################################
# Script start
#################################################
#change directory to script location
os.chdir(BUILD_DIR)

#remove existing Dev-C++ files
clean()

parser = argparse.ArgumentParser(description=("Creates the project files needed by Dev-C++,"
    "based on the current folder structure.  Uses the build.ignore file.  Can also be used to clean old Dev-C++ files."))
parser.add_argument("-c", "--clean", help="clean Dev-C++ files and exit", action="store_true")
args = parser.parse_args()
if args.clean:
    exit(0)

#load files and directories to be ignored
toIgnore = findIgnore()

#locate c and h files
files = findFiles(toIgnore)
folders = []
for fn,fo in files:
    if fo not in folders:
        folders.append(fo)
folders.sort()

#write out the file
with open(DEV_FILE, "w+") as oFile:
    writeHeader(oFile, folders, len(files))
    writeUnits(oFile, files)
