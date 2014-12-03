import os
import sys

rootdir = os.getcwd()
rootdir = rootdir.replace('\\', '/')
rootdir = rootdir + "/"
sourcePath = rootdir + ""

f_cm_all_files = open(rootdir + "file-list.pri", "w+")

# ------------------------------------------------------
src_file_list = []
# Collect files
for root, subFolders, files in os.walk(sourcePath):
    for file in files:
            path = os.path.join(root,file)
            src_file_list.append(path)
            
# ------------------------------------------------------

file_list = []

# ------------------------------------------------------

# write HEADERS
counter = 0
for file in src_file_list:
    if file.endswith('.h') ==1 or file.endswith('.hpp'):
        counter = counter + 1

if counter > 0:
    f_cm_all_files.write("HEADERS +=")
    for file in src_file_list:
        currFilter = os.path.relpath(file, sourcePath)
        currFilter = currFilter.replace('\\', '/')
        if file.endswith('.h') ==1 or file.endswith('.hpp'):
            f_cm_all_files.write( " \\  \n	" + currFilter)

# ------------------------------------------------------

# write SOURCES
counter = 0
for file in src_file_list:
    if file.endswith('.cpp') ==1 or file.endswith('.c'):
        counter = counter + 1

if counter > 0:    
    f_cm_all_files.write("\nSOURCES +=")
    for file in src_file_list:
        currFilter = os.path.relpath(file, sourcePath)
        currFilter = currFilter.replace('\\', '/')
        if file.endswith('.cpp') ==1 or file.endswith('.c'):
            f_cm_all_files.write( " \\  \n	" + currFilter)

# ------------------------------------------------------

# write FORMS
counter = 0
for file in src_file_list:
    if file.endswith('.ui') ==1:
        counter = counter + 1

if counter > 0:         
    f_cm_all_files.write("\nFORMS +=")
    for file in src_file_list:
        currFilter = os.path.relpath(file, sourcePath)
        currFilter = currFilter.replace('\\', '/')
        if file.endswith('.ui') ==1:
            f_cm_all_files.write( " \\  \n	" + currFilter)

# ------------------------------------------------------

# write RESOURCES
counter = 0
for file in src_file_list:
    if file.endswith('.qrc') ==1:
        counter = counter + 1

if counter > 0:    
    f_cm_all_files.write("\nRESOURCES +=")
    for file in src_file_list:
        currFilter = os.path.relpath(file, sourcePath)
        currFilter = currFilter.replace('\\', '/')
        if file.endswith('.qrc') ==1:
            f_cm_all_files.write( " \\  \n	" + currFilter)

# ------------------------------------------------------

f_cm_all_files.close()
