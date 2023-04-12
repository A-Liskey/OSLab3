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

    // stores current entry
    union Entry entry;

    // Read all entries and print info until we reach a file where the first 2 bytes are ____
    while (true)
    {
        fread(&entry, 32, 1, fp);

        //create variables to store all file details

        //get file attributes
        
        //get all details unique to a short entry

        //check if entry is long
        if(false) {

            //if the entry is long, read the next entry until the end of the long entry is found
            while(true) {

            }
        }

        getCreatedDate(entry.shortEntry);

        //print entry
        printf("ENTRY HERE\n");
    }

    //print summary of files
    printf("SUMMARY HERE\n");

    fclose(fp);
}