#pragma pack(push,1)

struct shortEntry {
    unsigned char Name[11];
    unsigned char Attr;
    unsigned char NTRes;
    unsigned char CrtTimeTenth;
    unsigned short CrtTime;
    unsigned short CrtDate;
    unsigned short LstAccDate;
    unsigned short FstClusHI;
    unsigned short WrtTime;
    unsigned short WrtDate;
    unsigned short FstClusLO;
    unsigned int FileSize;
};

struct longEntry
{
    unsigned char Ord;
    unsigned char Name1[10];
    unsigned char Attr;
    unsigned char Type;
    unsigned char ChkSum;
    unsigned char Name2[12];
    unsigned short FstClusLO;
    unsigned char Name3[4];
};


union Entry
{
    struct shortEntry shortEntry;
    struct longEntry longEntry;
};

char* getCreatedDateAndTime(struct shortEntry entry) {
    
    //store month day and year starting at MS-DOS Epoch of 01/01/1980
    unsigned int month = 1, day = 1, year = 1980, hours = 0, minutes = 0, seconds = 0;

    //extract bit ranges for each parameter and shift to get integer values
    day   +=  entry.WrtDate & 0b0000000000011111;
    month += (entry.WrtDate & 0b0000000111100000) >> 5;
    year  += (entry.WrtDate & 0b1111111000000000) >> 9;

    seconds +=  entry.WrtTime & 0b0000000000011111;
    minutes += (entry.WrtTime & 0b0000011111100000) >> 5;
    hours   += (entry.WrtTime & 0b1111100000000000) >> 11;

    //determine if AM or PM
    char* midday;
    if(hours % 12 >= 1) {
        midday = "PM";
        hours /= 12;
    }
    else {
        midday = "AM";
    }

    //create date and time string from calculated values
    char* str = malloc(21);
    snprintf(str, 21, "%02d/%02d/%02d  %02d:%02d %s", month, day, year, hours, minutes, midday);
    return str;
}