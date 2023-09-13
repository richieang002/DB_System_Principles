#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED

#include <cstddef>
#include <string>
#include <iostream>
#include <cmath>
#include <cstring>

struct Record {
    unsigned int GAME_DATE_EST; // 4 Bytes
    unsigned int TEAM_ID_home; // 4 Bytes
    float FG_PCT_home; // 4 Bytes
    float FT_PCT_home; // 4 Bytes
    float FG3_PCT_home; // 4 Bytes
    unsigned short int PTS_home; // 2 Bytes
    unsigned short int AST_home; // 2 Bytes
    unsigned short int REB_home; // 2 Bytes
    bool HOME_TEAM_WINS; // 1 Byte + 1 Byte Padding
    unsigned char NULLChecker;
    // Total 28 Bytes
};

class Disk {
private:
    unsigned int blockSize;
    unsigned int diskSize;
    unsigned int blockIndex;
    unsigned int recordIndex;
    unsigned char *diskAddressPtr;
    unsigned int maxRecordsPerBlock;
    unsigned int maxBlocksPerDisk;
    unsigned int diskSizeUsed;

public:
    // constructor
    Disk(unsigned int inDiskSize, unsigned int inBlockSize);

    void printDiskState();

    Record *getRecord(unsigned int inBlockIndex, unsigned int inRecordIndex);

    void printRecord(Record *record);

    Record *insertRecord(Record *record);

    void printBlock(unsigned int inBlockIndex);

    unsigned int getBlockIndex(Record *record);

    int getNumBlocksUsed();

    int getNumRecordsUsed();

    int getNumRecordsByBlock(unsigned int blockIndex);

    int getDiskUsage();
};

#endif // STORAGE_H_INCLUDED
