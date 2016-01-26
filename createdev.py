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

BUILD_DIR = os.path.dirname(os.path.realpath(__file__))
DEV_FILE = "omnisquash.dev"

def findFiles():
    """locate c and h files"""
    global PLATFORM
    files = []
    for (dirpath, dirnames, filenames) in os.walk(BUILD_DIR):
        for name in filenames:
            if len(name) > 2 and (name[-2:] == ".c" or name[-2:] == ".h"):
                folder = dirpath.replace(BUILD_DIR + "/","")
                files.append((folder.replace("/", "\\") + "\\" + name, folder))
    return files

def clean():
    """remove existing Dev-C++ files"""
    try:
        os.remove("Makefile.win")
        os.remove("omnisquash.layout")
    except OSError:
        pass

def writeHeader(oFile, folders):
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
    "UnitCount=44\n" +
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

#locate c and h files
files = findFiles()
folders = []
for fn,fo in files:
    if fo not in folders:
        folders.append(fo)
folders.sort()

#write out the file
with open(DEV_FILE, "w+") as oFile:
    writeHeader(oFile, folders)
    writeUnits(oFile, files)
