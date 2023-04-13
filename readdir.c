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

    FILE *fp = fopen("Directory.dat", "r"); // DEBUG VERSION ONLY =======================
    // FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("File not found\n");
        return 1;
    }

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
        unsigned char *dateString = "", shortFileName[13], *longFileName = "";
        unsigned int size, clusterNum;

        // look at attributes to determine what to do with entry
        char attributes = entry.shortEntry.Attr;

        // if the entry has a long filename: find long filename and extension
        if ((attributes & 0x0F) == 0x0F)
        {

            // read the next entry until the first long entry is found
            unsigned char *nameSegment;
            do
            {
                // read next entry and prepend name value and file extension
                fread(&entry, 32, 1, fp);
                nameSegment = malloc(26 + strlen(longFileName));

                // build name segment
                int nameIndex = 0;
                for (int i = 0; i < 10; i += 2)
                {
                    nameSegment[nameIndex++] = entry.longEntry.Name1[i];
                }
                for (int i = 0; i < 12; i += 2)
                {
                    nameSegment[nameIndex++] = entry.longEntry.Name2[i];
                }
                for (int i = 0; i < 4; i += 2)
                {
                    nameSegment[nameIndex++] = entry.longEntry.Name3[i];
                }

                // append name to namesegment
                longFileName = malloc(nameIndex);
                strncat(longFileName, nameSegment, nameIndex);
            } while ((entry.longEntry.Ord & 1) != 1);

            //add null terminator to string
            longFileName[strlen(longFileName)] = '\000';

            // go to next entry (will be a short entry)
            fread(&entry, 32, 1, fp);
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
        //add null terminator to string
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
    }

    // print summary of files
    printf("SUMMARY HERE\n");

    fclose(fp);
}