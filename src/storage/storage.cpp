#include "storage.h"

using namespace std;

Disk::Disk(unsigned int inDiskSize, unsigned int inBlockSize) {
    this->diskSize = inDiskSize;
    this->blockSize = inBlockSize;
    diskSizeUsed = 0;
    blockIndex = 0;
    recordIndex = 0;
    diskAddressPtr = new unsigned char[diskSize]();
    maxRecordsPerBlock = floor(blockSize/ sizeof(Record));
    maxBlocksPerDisk = floor(diskSize / blockSize);

    cout << endl << "<<<<<<<< Initialising Disk Storage >>>>>>>>" << endl;
    cout << "===========================================" << endl;
    cout << "Disk Storage Allocated at " << reinterpret_cast<void *>(diskAddressPtr) << " on Main Memory" << endl;
    cout << "Disk Size: " << diskSize / pow(2, 20) << " MB" << endl;
    cout << "Block Size: " << blockSize << " Bytes" << endl;
    cout << "Record Size: " << sizeof(Record) << " Bytes" << endl;
    cout << "Max No. of Records Per Block: " << maxRecordsPerBlock << endl;
    cout << "Max No. of Blocks Per Disk: " << maxBlocksPerDisk << endl;
    cout << "===========================================" << endl;
}

void Disk::printDiskState() {
    cout << endl << "<<<<<<<< Disk Storage Details >>>>>>>>" << endl;
    cout << "===========================================" << endl;
    cout << "Storage Used: " << diskSizeUsed / pow(2, 20) << " / " << diskSize / pow(2, 20) << " MB" << endl;
    cout << "Blocks Allocated: " << blockIndex + 1<< endl;
    cout << "Records Allocated: " << recordIndex + blockIndex * maxRecordsPerBlock << endl;
    cout << "===========================================" << endl;
}

Record *Disk::getRecord(unsigned int inBlockIndex, unsigned int inRecordIndex) {
    if (inBlockIndex > blockIndex) return nullptr;
    if (inBlockIndex == blockIndex && inRecordIndex > recordIndex) return nullptr;

    return reinterpret_cast<Record *>(diskAddressPtr + (inBlockIndex * blockSize) + (inRecordIndex * sizeof(Record)));
}

void Disk::printRecord(Record *record) {

    if (record == nullptr) {
        cout << "Record is empty" << endl;
        return;
    }

    // Convert integer to date string format
    string gameDate = std::to_string(record->GAME_DATE_EST / 1000000 % 100) + "/" + std::to_string(record->GAME_DATE_EST / 10000 % 100) + "/" + std::to_string(record->GAME_DATE_EST % 10000);
    string output;

    cout << "~~ [Home Team] Record Details ~~" << endl;
    cout << "Game Date: " << gameDate << endl;
    output = (record->NULLChecker >> 0) & 1 ? "NULL" : to_string(record->TEAM_ID_home);
    cout << "Team ID: " << output << endl;
    output = (record->NULLChecker >> 1) & 1 ? "NULL" : to_string(record->PTS_home);
    cout << "Points: " << output << endl;
    output = (record->NULLChecker >> 2) & 1 ? "NULL" : to_string(record->FG_PCT_home);
    cout << "Field Goal %: " << output << endl;
    output = (record->NULLChecker >> 3) & 1 ? "NULL" : to_string(record->FT_PCT_home);
    cout << "Free Throw %: " << output << endl;
    output = (record->NULLChecker >> 4) & 1 ? "NULL" : to_string(record->FG3_PCT_home);
    cout << "Field Goal 3Pts %: " << output << endl;
    output = (record->NULLChecker >> 5) & 1 ? "NULL" : to_string(record->AST_home);
    cout << "Assists: " << output << endl;
    output = (record->NULLChecker >> 6) & 1 ? "NULL" : to_string(record->REB_home);
    cout << "Rebounds: " << output << endl;
    output = (record->NULLChecker >> 7) & 1 ? "NULL" : to_string(record->HOME_TEAM_WINS);
    cout << "Wins: " << output << endl;
}

Record *Disk::insertRecord(Record *record) {
    if (blockIndex >= maxBlocksPerDisk)
    {
        return nullptr;
    }

    Record *newRecord = getRecord(blockIndex, recordIndex);

    newRecord->GAME_DATE_EST = record->GAME_DATE_EST;
    newRecord->TEAM_ID_home = record->TEAM_ID_home;
    newRecord->PTS_home = record->PTS_home;
    newRecord->FG_PCT_home = record->FG_PCT_home;
    newRecord->FT_PCT_home = record->FT_PCT_home;
    newRecord->FG3_PCT_home = record->FG3_PCT_home;
    newRecord->AST_home = record->AST_home;
    newRecord->REB_home = record->REB_home;
    newRecord->HOME_TEAM_WINS = record->HOME_TEAM_WINS;
    newRecord->NULLChecker = record->NULLChecker;

    if (blockIndex == 0 && recordIndex == 0)
    {
        diskSizeUsed = diskSizeUsed + blockSize;
    }

    recordIndex++;

    if (recordIndex == maxRecordsPerBlock)
    {
        recordIndex = 0;
        blockIndex++;
        diskSizeUsed = diskSizeUsed + blockSize;
    }

    return newRecord;
}

void Disk::printBlock(unsigned int inBlockIndex) {
    unsigned short int recordsUsed = maxRecordsPerBlock;

    if (inBlockIndex > blockIndex) {
        cout << "ALERT => Block index " << inBlockIndex << " does not exist in Disk" << endl;
        return;
    }
    if (inBlockIndex == blockIndex) {
        recordsUsed = recordIndex;
    }

    cout << "\n~~ Block " << inBlockIndex << " Details ~~" << endl;
    cout << "Records Used: " << recordsUsed << " / " << maxRecordsPerBlock << endl;
    cout << "===========================================" << endl;

    for (unsigned int i = 0; i < maxRecordsPerBlock; i++){
        cout << "Record " << i + 1 << ":" << endl;
        if (i >= recordsUsed) {
            cout << "Record is empty" << endl;
        }
        else {
            printRecord(getRecord(inBlockIndex, i));
        }
        cout << endl;
    }
    cout << "===========================================" << endl;
}

int Disk::getNumBlocksUsed() {
    return blockIndex;
}

int Disk::getNumRecordsUsed() {
    return blockIndex * maxRecordsPerBlock + recordIndex;
}

int Disk::getNumRecordsByBlock(unsigned int inBlockIndex) {
    if (inBlockIndex < blockIndex) return maxRecordsPerBlock;
    if (inBlockIndex > blockIndex) return 0;
    return recordIndex;
}

