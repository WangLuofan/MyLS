//
//  main.cpp
//  myls
//
//  Created by 王落凡 on 2016/10/27.
//  Copyright © 2016年 王落凡. All rights reserved.
//
#include <stdio.h>
#include "parsecmd.h"
#include "dirhandler.h"

int main(int argc, const char * argv[])
{
    char* args = NULL;
    
    int argsCount = ParseCmds(argc, argv, args);
    
    int paths = argc - 1 - argsCount;
    if(paths == 0)
        showFilesWithArgs(".", args);
    else
        for(int i = argsCount + 1; i != argc; ++i)
        {
            if(paths > 1)
                printf("%s:\n", argv[i]);
            showFilesWithArgs(argv[i], args);
        }
    
    return 0;
}
