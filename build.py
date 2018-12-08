#! /usr/bin/python2

#################################################
#
# Used for building the Omnisquash engine
#
#################################################

import os
import platform
import subprocess
import argparse

BUILD_DIR = os.path.dirname(os.path.realpath(__file__))
PLATFORM=""
CC=""
LIBS="" #used for linking only
INCS="" #used for compilation only
CFLAGS="" #combined with INCS for compilation only
BIN=""
ENVIRONMENT_MODIFY = os.environ.copy()

FORCE = False
LINK = False
VERBOSE = False
SILENT = False
CLEAN = False
CREATE_TABLES = False

IGNORE_FILENAME = ".build.ignore"
CHANGED_FILENAME = ".build.changed"

def configure():
    """configure the compilation options"""
    global PLATFORM
    global CC
    global LIBS
    global INCS
    global CFLAGS
    global BIN
    global ENVIRONMENT_MODIFY

    #determine the platform and set things accordingly
    if "cygwin" in platform.system().lower():
        PLATFORM = "CYGWIN"
        CC = "/cygdrive/c/MinGW/bin/gcc.exe"
        LIBS = "-L\"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32\" "\
                "-L\"D:/SDL2-2.0.8/i686-w64-mingw32/lib\" "\
                "-static-libgcc "\
                "-lmingw32 "\
                "-lSDL2main "\
                "-lSDL2 "\
                "-lSDL2_image "\
                "-lSDL2_mixer "\
                "-m32 "\
                "-g3"
        INCS = "-I\"C:/Program Files/Dev-Cpp/MinGW64/include\" "\
                "-I\"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include\" "\
                "-I\"C:/Program Files/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include\" "\
                "-I\"D:/SDL2-2.0.8/i686-w64-mingw32/include/\" "
        BIN = "omnisquash.exe"
    elif "windows" in platform.system().lower() or "win32" in platform.system().lower():
        #
        # No slashes at the end of paths, they'll be read as escape characters for the quotations
        #
        PLATFORM = "WINDOWS"
        CC = "C:\\MinGW\\bin\\gcc.exe"
        LIBS = "-L\"C:\\MinGW\\lib\" "\
                "-L\"D:\\omnisquash\\SDL2-2.0.8\\i686-w64-mingw32\\lib\" "\
                "-static-libgcc "\
                "-lmingw32 "\
                "-lSDL2main "\
                "-lSDL2 "\
                "-lSDL2_image "\
                "-lSDL2_ttf "\
                "-lSDL2_mixer "\
                "-m32 "\
                "-g3"
        INCS = "-I\"C:\\MinGW\\include\" "\
                "-I\"D:\\omnisquash\\SDL2-2.0.8\\i686-w64-mingw32\\include\" "
        BIN = "omnisquash.exe"
        ENVIRONMENT_MODIFY["PATH"] += "C:\\MinGW\\bin;"
    elif "mac" in platform.system().lower() or "darwin" in platform.system().lower():
        PLATFORM = "MAC"
        CC = "gcc"
        LIBS = "LIBS"
        INCS = "INCS"
        BIN = "omnisquash"
    elif "linux" in platform.system().lower():
        PLATFORM = "LINUX"
        CC = "gcc"
        LIBS = "-static-libgcc "\
                "-lSDL2main "\
                "-lSDL2 "\
                "-lSDL2_image "\
                "-lSDL2_mixer "\
                "-lm "\
                "-m32 "\
                "-g3"
        BIN = "omnisquash"        
    else:
        print "Platform could not be determined or is unsupported."
        print "Build cancelled."
        exit(1)


#    LIBS = "-L\"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32\" -L\"D:/SDL-1.2.15/lib\" -static-libgcc -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -m32 -g3"
#    INCS = "-I\"C:/Program Files/Dev-Cpp/MinGW64/include\" -I\"C:/Program\ Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include\" -I\"C:/Program Files/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include\" -I\"D:/SDL-1.2.15/include\""
    CFLAGS = INCS + " -m32 -g3"

def removeFile(path):
    try:
        os.remove(path)
    except OSError:
        # print "Could not remove " + path
        pass
    
def findIgnore():
    """read the file build.ignore if present and use the list to exclude files
    from the build"""
    toIgnore = []
    try:
        with open(IGNORE_FILENAME, "r") as fin:
            for line in fin:
                line = line.strip()
                if len(line) > 0 and line[0] != "#":
                    toIgnore.append(BUILD_DIR + line.replace("/", os.sep))
    except IOError:
        pass
                
    return toIgnore

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

class FileDetails:
    """Class for storing and tracking all the relevant details for the source and header files used"""
    
    def __init__(self, name, path):
        self.name = name
        self.path = path
        self.header = (name[-2:] == ".h")
        self.absolute = os.path.join(path, name)
        self.modifiedDate = int(os.path.getmtime(self.absolute))
        self.originalDate = -1
        self.changed = False
        self.recompile = False
        self.dependencies = []
        self.compiled = False
        
    def checkModified(self, times):
        if self.absolute in times:
            self.originalDate = int(times[self.absolute])
            if times[self.absolute] != self.modifiedDate:
                self.changed = True
                self.recompile = True
    
    def modifiedRecord(self):
        result = ""
        if self.recompile and self.compiled:
            result = str(self.modifiedDate) + "," + self.absolute
        else:
            result = str(self.originalDate) + "," + self.absolute
            
        return result

    def setRecompile(self):
        self.recompile = True
        
    def needRecompile(self):
        return self.recompile
    
    def setCompiled(self):
        self.compiled = True
        
    def compileSuccess(self):
        return self.compiled
    
    def getPath(self):
        return self.path
    
    def getAbsolutePath(self):
        return self.absolute
    
    def isMyAbsolutePath(self, path):
        if path.strip() == self.absolute:
            return True
        else:
            return False
    
    def isHeader(self):
        return self.header
    
    def setDependencies(self, deps):
        self.dependencies = list(deps)
    
    def getDependencies(self):
        return list(self.dependencies)
    
    def cFile(self):
        """Perform platform specific alterations to the name of the file so that
            when called below the compile command creation is streamlined"""
        result = ""
        
        if self.header:
            result = ""
        elif PLATFORM == "CYGWIN":
            result = self.absolute.replace(BUILD_DIR + "/","")
        else:
            result = self.absolute
        return result
    
    def oFile(self):
        result = ""
        
        if self.header:
            result = ""
        elif PLATFORM == "CYGWIN":
            result = self.absolute[:-1] + "o"
            result = result.replace(BUILD_DIR + "/","")
        else:
            result = self.absolute[:-1] + "o"
        return result
    
    def __str__(self):
        result = ""
        result += self.absolute
        result += " - " + str(self.header)
        result += " - " + str(self.modifiedDate)
        result += " - " + str(self.changed)
        return result
    
    def __repr__(self):
        return self.name
    
def determineRecompile(toIgnore):
    """Determine based on modification date, if the files need to be recompiled.
    Return lists of c FileDetails and h FileDetails.
    
    Essentially the algorithm is this:
        -First, find all files and mark them as requiring recompile if their last
        edit date is newer than what is in the change file.
        -Second, determine what header files depend (possibly through a series of
        inclusions) on changed header files.
        -Third, determine what c files need to be recompiled because they include
        (possibly through a series of inclusions) a header file that has been changed.
        -Fourth, for files not requiring recompilation, verify that an object file
        actually exists.
    """
    
    recompileAll = FORCE
    
    #read in the file modification list
    times = {}
    try:
        with open(CHANGED_FILENAME, "r") as fin:
            for line in fin:
                line = line.strip()
                data = line.split(",")
                times[data[1]] = int(data[0])
    except IOError:
        recompileAll = True
        
    #find all the files and create FileDetails for them, ignoring those in the ignore file
    cFiles = []
    hFiles = []
    for (dirpath, dirnames, filenames) in os.walk(BUILD_DIR):
        if dirpath in toIgnore:
            continue
        
        for name in filenames:
            if len(name) > 2 and (name[-2:] == ".c" or name[-2:] == ".h"):
                f = FileDetails(name, dirpath)
                f.checkModified(times)
                
                if f.getAbsolutePath() in toIgnore:
                    continue
                else:
                    if f.isHeader():
                        hFiles.append(f)
                    else:
                        cFiles.append(f)
    
    #if the modified list failed to exist, all must be recompiled, return immediately
    if recompileAll:
        for f in cFiles + hFiles:
            f.setRecompile()
        return cFiles, hFiles

    #determine the dependencies for the header files
    # hDeps = {}
    for f in hFiles:
        # hDeps[f] = determineDependencies(f, hFiles)
        #pizza - maybe make determinedeps part of the class?
        f.setDependencies(determineDependencies(f, hFiles))

    #repeatedly check if header files depend on changed header files until list length stabilizes
    unchangedHeaders = list(hFiles)
    length = -1
    while len(unchangedHeaders) != length:
        length = len(unchangedHeaders)
        updatedUCH = []
        for h in unchangedHeaders:
            for f in h.getDependencies():
                if f.needRecompile():
                    h.setRecompile()
                    break
            if not h.needRecompile():
                updatedUCH.append(h)
        unchangedHeaders = updatedUCH
        
    #if any c file depends on a header file marked as requiring recompile, set that c file to recompile
    #otherwise check to see if the object files exist for c files, set those to recompile
    for f in cFiles:
        if f.needRecompile():
            continue
            
        deps = determineDependencies(f, hFiles)
        f.setDependencies(deps)
        for h in deps:
            if h.needRecompile():
                f.setRecompile()
                break
                
        if not os.path.isfile(f.oFile()):
            f.setRecompile()

    #return the lists of c and h files
    return cFiles, hFiles
    
def determineDependencies(fd, headers):
    """Parse the given FileDetails and determine what header files it includes"""
    #read the file and pull out any non-commented lines with "#include"
    includes = []
    try:
        with open(fd.getAbsolutePath(), "r") as fin:
            #to be honest, it probably makes sense to just strip out the parts of the line
            #that are commented out and then check what remains, rather than mucking about
            #with comparing indices.
            blockComment = False
            for line in fin:
                line = line.strip()
                
                #remove block comments - PIZZA NEEDS TO BE TESTED
                if blockComment and "*/" not in line:
                    continue
                elif blockComment and "*/" in line:
                    line = line[line.find("*/")+1:]
                    blockComment = False
                elif "/*" in line:
                    if "*/" not in line:
                        blockComment = True
                        line = line[:line.find("/*")]
                    else:
                        while "/*" in line and "*/" in line:
                            line = line[:line.find("/*")]+line[line.find("*/")+1:]
                            line = line.strip()
                        if "/*" in line:
                            blockComment = True
                            line = line[:line.find("/*")]
                
                #remove commented section of line
                if "//" in line:
                    line = line[:line.find("//")]
                
                #determine if include is contained in line
                if "#include" in line:
                    includes.append(line)
    except IOError:
        print "Error reading file " + fd.getAbsolutePath
        exit(1)
    
    #go through the lines and remove any includes that are C libraries or SDL
    dependencies = []
    for line in includes:
        source = line[8:].strip() #remove "#include"
        if source[0] == "<":
            continue
        source = source.replace("\"", "")
        if source[0:4] == "SDL2":
            continue
        dependencies.append(os.path.normpath(source))
    
    #match the dependencies to file detail objects
    results = []
    for d in dependencies:
        absPath = None
        if "/" in d:
            absPath = os.path.split(fd.getPath())[0] + d[2:].replace("/", os.path.sep)
        else:
            absPath = os.path.join(fd.getPath(), d)
            
        for h in headers:
            if h.isMyAbsolutePath(absPath):
                results.append(h)
    
    return results

def setHeaderCompileSuccess(cFiles, hFiles):
    #determine if a header file "compiled" successfully if all c files that include it compiled
    #successfully - pizza - whether or not this is the best way to do this is debatable.
    #if the whole project builds fine then we will just mark all h files as compiling and not call this
    for h in hFiles:
        compFailed = False
        for c in cFiles:
            if not c.compileSuccess() and h in c.getDependencies():
                compFailed = True
                break
        if not compFailed:
            h.setCompiled()
    
def writeNewModifiedList(fileList):
    """Attempt to create a new file with the last modified date for all source files"""
    try:
        with open(CHANGED_FILENAME, "w+") as fout:
            for f in fileList:
                fout.write(f.modifiedRecord() + "\n")
    except IOError:
        pass

def handleArguments():
    global FORCE
    global LINK 
    global VERBOSE
    global SILENT
    global CLEAN
    global CREATE_TABLES
    
    descr = "Build the omnisquash engine. Files/directories to be ignored should\
            be included in .build.ignore. Data used for smarter compilation stored in\
            .build.changed"
    parser = argparse.ArgumentParser(description=descr)
    parser.add_argument("-f", "--force", help="recompile entire project", action="store_true")
    parser.add_argument("-l", "--link", help="try to relink without recompiling", action="store_true")
    parser.add_argument("-v", "--verbose", help="print compile commands", action="store_true")
    parser.add_argument("-s", "--silent", help="only show compiler output", action="store_true")
    parser.add_argument("-c", "--clean", help="don't compile, remove all .o files", action="store_true")
    parser.add_argument("-t", "--tables", help="recreate the function tables before building", action="store_true")
    args = parser.parse_args()
    
    #first, check that no incompatible arguments have been given
    if (args.force and (args.link or args.clean)) or (args.link and args.clean) or (args.verbose and args.silent):
        print "Incompatible options"
        exit(1)
    
    #store arguments in global variables
    FORCE = args.force
    LINK = args.link
    VERBOSE = args.verbose
    SILENT = args.silent
    CLEAN = args.clean
    CREATE_TABLES = args.tables
    
#################################################
# Script start
#################################################

#read in command line arguments, act appropriately
handleArguments()

#set compilation variables
os.chdir(BUILD_DIR)
configure()

#if FORCE is set, recreate the table files too
if FORCE or CREATE_TABLES:
    if not SILENT:
        print "Recreating function tables"
    if PLATFORM == "CYGWIN" or PLATFORM == "LINUX":
        ret = subprocess.call("./createtablefiles.py", shell=True)
        if ret != 0:
            print "Function table creation failed"
            exit(1)
    else:
        print "Not yet implemented, please run createtablefiles.py manually."
    
#load files and directories to be ignored
toIgnore = findIgnore()

#locate all source files and determine what should be recompiled based on inclusion and modification date
cFileList, hFileList = determineRecompile(toIgnore)

#track compilation success
compilationFailed = False
linkingFailed = False

#recompile those files that need recompiling
for f in cFileList:
    #if we're just linking, break out
    if LINK:
        break

    #if we're just cleaning, just remove file and continue
    elif CLEAN:
        removeFile(f.oFile())
        continue
        
    elif f.needRecompile():
        #remove old object file - PIZZA do we need an absolute path here?
        removeFile(f.oFile())
        
        #generate and print the compile command
        compileCmd = CC + " -c " + f.cFile() + " -o " + f.oFile() + " " + CFLAGS
        if VERBOSE:
            print compileCmd
        elif not SILENT:
            print "Compiling " + f.cFile() + ""
        
        #make platform specific changes to, and then run, compile command here
        ret = -1
        if PLATFORM == "CYGWIN" or PLATFORM == "LINUX":
            compileCmd = "eval \"" + compileCmd.replace("\"", "\\\"") + "\""
            ret = subprocess.call(compileCmd, shell=True)
        elif PLATFORM == "WINDOWS":
            #idk what this was for?
            #maybe I figured you wouldn't have manually made the variables for each environment?
            # compileCmd = compileCmd.replace("\"", "\\\"") + "\""
            ret = subprocess.call(compileCmd, env=ENVIRONMENT_MODIFY)
        else:
            ret = subprocess.call(compileCmd)
            
        #check return code, update filedetails here if compile succeeded, otherwise break
        if ret != 0:
            compilationFailed = True
            break
        else:
            f.setCompiled()

#if compilation failed, don't bother linking
if compilationFailed:
    print "Build failed"
    
#link the object files
elif not CLEAN:
    #remove the old binary
    removeFile(os.path.join(BUILD_DIR, BIN))

    #generate the linking command
    oFiles = " ".join([f.oFile() for f in cFileList])
    linkCmd = CC + " " +  oFiles + " -o " + BIN + " " + LIBS
    
    if VERBOSE:
        print linkCmd
    elif not SILENT:
        print "Linking"

    #make platform specific changes to, and then run, linking command here
    if PLATFORM == "CYGWIN" or PLATFORM == "LINUX":
        linkCmd = "eval \"" + linkCmd.replace("\"", "\\\"") + "\""
        ret = subprocess.call(linkCmd, shell=True)
    elif PLATFORM == "WINDOWS":
        ret = subprocess.call(linkCmd, env=ENVIRONMENT_MODIFY)
    else:
        ret = subprocess.call(linkCmd)

    #if linking failed, I guess don't write out successful object file creation?
    if ret != 0:
        linkingFailed = True
        print "Build failed"

#depending on how compilation went, write a new change file and exit
if CLEAN:
    exit(0)
elif not compilationFailed and not linkingFailed:
    for h in hFileList:
        h.setCompiled()
    writeNewModifiedList(cFileList + hFileList)
    exit(0)
else:
    setHeaderCompileSuccess(cFileList, hFileList)
    writeNewModifiedList(cFileList + hFileList)
    exit(1)
