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
    struct shortEntry;
    struct longEntry;
};

