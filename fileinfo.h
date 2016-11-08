//
//  fileinfo.h
//  myls
//
//  Created by 王落凡 on 2016/10/27.
//  Copyright © 2016年 王落凡. All rights reserved.
//

#ifndef fileinfo_h
#define fileinfo_h

/*
 #define	DT_UNKNOWN	 0
 #define	DT_FIFO		 1
 #define	DT_CHR		 2
 #define	DT_DIR		 4
 #define	DT_BLK		 6
 #define	DT_REG		 8
 #define	DT_LNK		10
 #define	DT_SOCK		12
 #define	DT_WHT		14
 */

typedef struct FileInfo
{
    int fileType;
    int numberOfLinks;
    unsigned long fileSize;
    int abbreviatedMonth;
    int abbreviatedMin;
    int abbreviatedDayOfMonth;
    int abbreviatedHour;
    char* ownerName;
    char* groupName;
    char* fileName;
    char* fileProp;
    FileInfo* nextFile;
}FileInfo;


#endif /* fileinfo_h */
