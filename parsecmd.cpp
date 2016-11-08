//
//  parsecmd.cpp
//  myls
//
//  Created by 王落凡 on 2016/10/27.
//  Copyright © 2016年 王落凡. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parsecmd.h"

#define ARGU_LEN 20

void clean(char* args)
{
    if(args != NULL)
    {
        free(args);
        args = NULL;
    }
    
    return ;
}

int ParseCmds(int argc, const char* argv[], char*& args)
{
    clean(args);
    
    args = (char*)malloc(sizeof(char) * ARGU_LEN);
    memset(args, 0, sizeof(char) * ARGU_LEN);
    
    int cmd_index = 0, argsCount = 0;
    for(int i = 1; i != argc; ++i)
    {
        const char* strp = *(argv + i);
        
        if((*strp) != '-')
            break ;
        
        ++argsCount;
        size_t arglen = strlen(strp);
        for(int j = 1; j != arglen; ++j)
            args[cmd_index++] = *(strp + j);
    }
    
    return argsCount;
}
