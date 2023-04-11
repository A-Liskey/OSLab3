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

    char *fileName = argv[1];
    fileName = "Directory.dat"; // DEBUG PARAMETER ONLY =======================

    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("File not found\n");
        return 1;
    }

    // stores current entry as an array of 32 bytes
    unsigned char currentEntry[32];

    // // print root directory name
    // fread(currentEntry, 1, 32, fp);
    // printf("Directory of %.*s\n", 11, currentEntry);

    // Read all entries and print info
    while (fread(currentEntry, 1, 32, fp) == 32)
    {
        // check if current entry is a long entry
        if (currentEntry[11] == 1)
        {
            //if current entry is long, process as a long entry
        }
        else {
            //if current entry is not long, process as a short entry
        }

        //print entry
        printf("ENTRY HERE\n");
    }

    fclose(fp);
}