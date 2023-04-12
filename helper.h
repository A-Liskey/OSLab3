#pragma pack(push,1)

struct shortEntry {
    unsigned char Name[11];
    unsigned char Attr;
    unsigned char NTRes;
    unsigned char CrtTimeTenth;
    unsigned short CrtTime;
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

char* getCreatedDate(struct shortEntry entry) {
    
    //store month day and year starting at MS-DOS Epoch of 01/01/1980
    unsigned int month = 1, day = 1, year = 1980, hours = 0, minutes = 0, seconds = 0;

    day += entry.WrtDate    & 0b0000000000011111;
    month += (entry.WrtDate & 0b0000000111100000) >> 6;
    year += (entry.WrtDate  & 0b1111111000000000) >> 10;

}