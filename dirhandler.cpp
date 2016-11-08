//
//  dirhandler.cpp
//  myls
//
//  Created by 王落凡 on 2016/10/27.
//  Copyright © 2016年 王落凡. All rights reserved.
//

#include "dirhandler.h"
#include "fileinfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <uuid/uuid.h>
#include <time.h>
#include <sys/time.h>

#define LS_ITEMS_PER_LINE 2

int compare(const void* ch1, const void* ch2)
{
    return (*(char*)ch1) - (*(char*)ch2);
}

void printLong(struct FileInfo* info)
{
    printf("%s %d %s %s %lu %d %d %d:%d %s\n", info->fileProp, info->numberOfLinks,
           info->ownerName, info->groupName, info->fileSize, info->abbreviatedMonth,
           info->abbreviatedDayOfMonth, info->abbreviatedHour, info->abbreviatedMin, info->fileName);
    return ;
}

void printShort(char* fileName)
{
    int printIndex = 0;
    bool bNeedTab = false;
    ++printIndex;
    printf(bNeedTab ? "    %s" : "%s", fileName), bNeedTab = true;
    if(printIndex % LS_ITEMS_PER_LINE == 0)
        printf("\n"), bNeedTab = false;
    
    return ;
}

void printFileListWithArgs(char* args, struct FileInfo* fileHead)
{
    
    if(args != NULL)
        qsort(args, strlen(args), sizeof(char), compare);
    
    struct FileInfo* p = fileHead;
    while(p != NULL)
    {
        if(args == NULL)
        {
            if(p->fileName[0] != '.')
                printShort(p->fileName);
        }
        else if(args[0] == 'a' && args[1] != 'l')
        {
            printShort(p->fileName);
        }
        else if(args[0] == 'l')
        {
            if(p->fileName[0] != '.')
                printLong(p);
        }
        else if(args[0] == 'a' && args[1] == 'l')
        {
            printLong(p);
        }
        
        p = p->nextFile;
    }
    
    printf("\n");
    return ;
}

char* getFilePropStr(mode_t mode)
{
    char* prop = (char*)malloc(sizeof(char) * BUFSIZ);
    if(S_ISREG(mode))
        prop[0] = '-';
    else if(S_ISBLK(mode))
        prop[0] = 'b';
    else if(S_ISCHR(mode))
        prop[0] = 'c';
    else if(S_ISDIR(mode))
        prop[0] = 'd';
    else if(S_ISLNK(mode))
        prop[0] = 'l';
    else if(S_ISWHT(mode))
        prop[0] = 'w';
    else if(S_ISFIFO(mode))
        prop[0] = 'p';
    else if(S_ISSOCK(mode))
        prop[0] = 's';
    
    prop[1] = ((mode & S_IRUSR) == 0 ? '-' : 'r');
    prop[2] = ((mode & S_IWUSR) == 0 ? '-' : 'w');
    prop[3] = ((mode & S_IXUSR) == 0 ? '-' : 'x');
    prop[4] = ((mode & S_IRGRP) == 0 ? '-' : 'r');
    prop[5] = ((mode & S_IWGRP) == 0 ? '-' : 'w');
    prop[6] = ((mode & S_IXGRP) == 0 ? '-' : 'x');
    prop[7] = ((mode & S_IROTH) == 0 ? '-' : 'r');
    prop[8] = ((mode & S_IWOTH) == 0 ? '-' : 'w');
    prop[9] = ((mode & S_IXOTH) == 0 ? '-' : 'x');
    
    return prop;
}

void getFileDetails(struct FileInfo*& info)
{
    struct stat detail;
    
    if(lstat(info->fileName, &detail) == -1)
    {
        perror("lstat error");
        return ;
    }
    
    info->numberOfLinks = detail.st_nlink;
    info->fileSize = detail.st_size;
    struct passwd* _passwd = getpwuid(detail.st_uid);
    info->ownerName = (char*)malloc(sizeof(char) * (strlen(_passwd->pw_name) + 1));
    strcpy(info->ownerName, _passwd->pw_name);
    struct group* _group = getgrgid(detail.st_gid);
    info->groupName = (char*)malloc(sizeof(char) * (strlen(_group->gr_name) + 1));
    strcpy(info->groupName, _group->gr_name);
    struct tm* _tm = localtime(&detail.st_mtimespec.tv_sec);
    info->abbreviatedMonth = _tm->tm_mon + 1;
    info->abbreviatedDayOfMonth = _tm->tm_mday;
    info->abbreviatedHour = _tm->tm_hour;
    info->abbreviatedMin = _tm->tm_min;
    
    info->fileProp = getFilePropStr(detail.st_mode);
    return ;
}

void showFilesWithArgs(const char* path, char* args)
{
    if(chdir(path) == -1)
    {
        perror("chdir error");
        return ;
    }
    
    DIR* dir = opendir(path);
    if(dir == NULL)
    {
        perror("open dir error");
        return ;
    }
    
    struct dirent* _dirent = NULL;
    struct FileInfo* fileHeadPtr = NULL;
    while((_dirent = readdir(dir)) != NULL)
    {
        struct FileInfo* p = (struct FileInfo*)malloc(sizeof(struct FileInfo));
        p->fileName = (char*)malloc(sizeof(char) * (_dirent->d_namlen + 1));
        strcpy(p->fileName, _dirent->d_name);
        p->fileType = _dirent->d_type;
        
        if(fileHeadPtr == NULL)
        {
            fileHeadPtr = p;
            fileHeadPtr->nextFile = NULL;
        }
        else
        {
            p->nextFile = fileHeadPtr->nextFile;
            fileHeadPtr->nextFile = p;
        }
        
        if(args != NULL && strlen(args) != 0)
            getFileDetails(p);
    }
    
    if(args == NULL || strlen(args) == 0)
        printFileListWithArgs(NULL, fileHeadPtr);
    else
        printFileListWithArgs(args, fileHeadPtr);
    
    if(dir != NULL)
        closedir(dir);
    return ;
}
