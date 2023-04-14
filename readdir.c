/*
Andrew Liskey 04/13/2023

This program will take in a directory from the command line and print out a directory listing.
*/


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include "helper.h"

int main(int argc, char *argv[], char *env[])
{

    //FILE *fp = fopen("Directory.dat", "r"); // DEBUG VERSION ONLY =======================
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("File not found\n");
        return 1;
    }

    //store directory summary values
    int numFiles = 0, totalSize = 0;

    // Read all entries and print info until we reach a file where the first byte is 0x00
    while (true)
    {
        // read next entry
        union Entry entry;
        fread(&entry, 32, 1, fp);

        // check Name[0] for special cases
        if (entry.shortEntry.Name[0] == 0x00)
        {
            // no entries remain after this one, so exit the loop
            break;
        }
        else if (entry.shortEntry.Name[0] == 0xE5)
        {
            // this entry is free, so go to next entry
            continue;
        }
        else if (entry.shortEntry.Name[0] <= 0x20)
        {
            // this entry starts with an illegal value, so go to next entry
            continue;
        }

        // create variables to store all entry details
        unsigned char *dateString = "", shortFileName[13] = {}, longFileName[256] = {};
        unsigned int size, clusterNum;

        // look at attributes to determine what to do with entry
        char attributes = entry.shortEntry.Attr;

        // if the entry has a long filename: find long filename and extension
        if (((attributes & 0x0F) == 0x0F))
        {

            // read all long entries into an array
            struct longEntry longEntries[11] = {}; 
            int numEntries = 0;
            while ((entry.longEntry.Attr) == 0x0F) 
            {
                longEntries[numEntries++] = entry.longEntry;
                fread(&entry, 32, 1, fp);
            } 

            for(int j = numEntries - 1; j >= 0; j--) {
                struct longEntry curEntry = longEntries[j];
                unsigned char nameSegment[26] = {};
                int nameIndex = 0;
                for (int i = 0; i < 10; i += 2)
                {
                    nameSegment[nameIndex++] = curEntry.Name1[i];
                }
                for (int i = 0; i < 12; i += 2)
                {
                    nameSegment[nameIndex++] = curEntry.Name2[i];
                }
                for (int i = 0; i < 4; i += 2)
                {
                    nameSegment[nameIndex++] = curEntry.Name3[i];
                }

                strncat(longFileName, nameSegment, nameIndex);
            }

            //add null terminator to end of string
            longFileName[strlen(longFileName)] = '\000';
        }
        else if ((attributes & 0x02) == 0x02)
        {
            // if entry is hidden: don't display (move on to next entry)
            continue;
        }
        else if ((attributes & 0x04) == 0x04)
        {
            // if entry is protected: don't display (move on to next entry)
            continue;
        }
        else if ((attributes & 0x08) == 0x08)
        {
            // if entry is a volume name: display volume name
            printf("Volume in drive X is %s\n\nDirectory of X:\\\n", entry.shortEntry.Name);
            continue;
        }
        else if ((attributes & 0x10) == 0x10)
        {
            // if entry is a subdirectory: ignore for this project

            // strncpy(shortFileName, entry.shortEntry.Name, 8);
            // dateString = getLastWriteDateAndTime(entry.shortEntry);
            // clusterNum = (entry.shortEntry.FstClusHI << 16) | (entry.shortEntry.FstClusLO);
            // size = entry.shortEntry.FileSize;

            // printf("%s  %5d  %-13s %s\n", dateString, clusterNum, "<DIR>", shortFileName);
            continue;
        }

        // build short filename with extension
        int nameIndex = 0;
        for (int i = 0; i < 12; i++)
        {
            if (entry.shortEntry.Name[i] == 0x20)
            {
                continue;
            }
            if (i == 8)
            {
                shortFileName[nameIndex++] = '.';
            }
            shortFileName[nameIndex++] = entry.shortEntry.Name[i];
        }
        // add null terminator to string
        shortFileName[nameIndex] = '\000';

        // get shortEntry details
        dateString = getLastWriteDateAndTime(entry.shortEntry);
        clusterNum = (entry.shortEntry.FstClusHI << 16) | (entry.shortEntry.FstClusLO);
        size = entry.shortEntry.FileSize;
        char sizeString[20] = {};
        intToStringWithCommas(size, sizeString);

        // print short entry details
        printf("%s  %7d  %13s %-12s ", dateString, clusterNum, sizeString, shortFileName);

        // if long file name exists: print long file name
        if (strlen(longFileName) != 0)
        {
            printf("%s", longFileName);
        }

        // indent line
        printf("\n");

        //update summary values
        numFiles++;
        totalSize += size;
    }

    // print summary of files
    char totalSizeString[20] = {};
    intToStringWithCommas(totalSize, totalSizeString);
    printf("\n%d File(s) %s Bytes\n", numFiles, totalSizeString);

    fclose(fp);
}