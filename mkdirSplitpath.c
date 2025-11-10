#include "types.h"

extern struct NODE *root;
extern struct NODE *cwd;

void mkdir(char pathName[]){
    if (!pathName || strcmp(pathName, "/") == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64];
    char dirName[1024];

    struct NODE* parent = splitPath(pathName, baseName, dirName);
    if (!parent) return;

    if (baseName[0] == '\0') {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    struct NODE* scan = parent->childPtr;
    while (scan) {
        if (strcmp(scan->name, baseName) == 0) {
            printf("MKDIR ERROR: directory %s already exists\n", baseName);
            return;
        }
        scan = scan->siblingPtr;
    }

    struct NODE* i = (struct NODE*)malloc(sizeof(struct NODE));
    if (!i) return;

    strncpy(i->name, baseName, sizeof(i->name) - 1);
    i->name[sizeof(i->name) - 1] = '\0';
    i->fileType   = 'D';
    i->childPtr   = NULL;
    i->siblingPtr = NULL;
    i->parentPtr  = parent;

    if (!parent->childPtr) {
        parent->childPtr = i;
    } else {
        struct NODE* tail = parent->childPtr;
        while (tail->siblingPtr) tail = tail->siblingPtr;
        tail->siblingPtr = i;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}

struct NODE* splitPath(char* pathName, char* baseName, char* dirName){
    if (!pathName || !baseName || !dirName) return NULL;

    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        baseName[0] = '\0';
        return root;
    }

    const char* slash = strrchr(pathName, '/');

    if (!slash) {
        dirName[0] = '\0';
        strcpy(baseName, pathName);
    } else if (slash == pathName) {
        strcpy(dirName, "/");
        strcpy(baseName, pathName + 1);
    } else {
        size_t length = (size_t)(slash - pathName);
        strncpy(dirName, pathName, length);
        dirName[length] = '\0';
        strcpy(baseName, slash + 1);
    }

    if (dirName[0] == '\0') return cwd;     
    if (strcmp(dirName, "/") == 0) return root; 

    struct NODE* current = (dirName[0] == '/') ? root : cwd;

    char temp[1024];
    strncpy(temp, dirName, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    char* currentDirectory = strtok(temp, "/");
    while (currentDirectory != NULL) {
        struct NODE* child = current->childPtr;
        while (child != NULL) {
            if (strcmp(child->name, currentDirectory) != 0 || child->fileType != 'D') {
                child = child->siblingPtr;
            } else {
                break; 
            }
        }
        if (child == NULL) {
            printf("ERROR: directory %s does not exist\n", currentDirectory);
            return NULL;
        }
        current = child;                    
        currentDirectory = strtok(NULL, "/");
    }
    return current;
}
