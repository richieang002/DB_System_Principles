#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include "storage/storage.h"

using namespace std;

void readFileToDisk(Disk *disk) {
    ifstream file("src/games.csv");
    if (file.fail()) {
        cout << "File failed to open" << endl;
        return;
    }
    cout << "File is opened" << endl;

    Record record = Record();
    string line;

    getline(file, line); // Get rid of header row
    while (getline(file, line))
    {
        // Read row data
        istringstream rowData(line);
        string temp;
        unsigned char NULLChecker = 0;
        unsigned int gameDate = 0;

        // Handle Date Data
        getline(rowData, temp, ',');
        istringstream datestring(temp);
        string segment;
        // Day
        getline(datestring, segment, '/');
        gameDate += stoi(segment) * 1000000;
        // Month
        getline(datestring, segment, '/');
        gameDate += stoi(segment) * 10000;
        // Year
        getline(datestring, segment, '/');
        gameDate += stoi(segment);
        record.GAME_DATE_EST = gameDate;

        // Team ID
        getline(rowData, temp, ',');
        if (temp == "") {
            NULLChecker = NULLChecker | (1 << 0);
            record.TEAM_ID_home = 0;
        }else {
            record.TEAM_ID_home = (unsigned int)stoul(temp);
        }

        // Points Home
        getline(rowData, temp, ',');
        if (temp == "") {
            NULLChecker = NULLChecker | (1 << 1);
            record.PTS_home = 0;
        } else {
            record.PTS_home = (unsigned short int)stoul(temp);
        }
        // Field Goal %
        getline(rowData, temp, ',');
        if (temp == "") {
            NULLChecker = NULLChecker | (1 << 2);
            record.FG_PCT_home = 0;
        } else {
            record.FG_PCT_home = stof(temp);
        }

        // Free Throw %
        getline(rowData, temp, ',');
        if (temp == "") {
            NULLChecker = NULLChecker | (1 << 3);
            record.FT_PCT_home = 0;
        } else {
            record.FT_PCT_home = stof(temp);
        }

        // Field Goal 3Pt %
        getline(rowData, temp, ',');
        if (temp == "") {
            NULLChecker = NULLChecker | (1 << 4);
            record.FG3_PCT_home = 0;
        } else {
            record.FG3_PCT_home = stof(temp);
        }

        // Assists
        getline(rowData, temp, ',');
        if (temp == "") {
            NULLChecker = NULLChecker | (1 << 5);
            record.AST_home = 0;
        } else {
            record.AST_home = (unsigned short int)stoul(temp);
        }

        // Rebounds
        getline(rowData, temp, ',');
        if (temp == "") {
            NULLChecker = NULLChecker | (1 << 6);
            record.REB_home = 0;
        } else {
            record.REB_home = (unsigned short int)stoul(temp);
        }

        // Win
        getline(rowData, temp, ',');
        if (temp == "") {
            NULLChecker = NULLChecker | (1 << 7);
            record.HOME_TEAM_WINS = 0;
        } else {
            record.HOME_TEAM_WINS = stoul(temp) == 0 ? false : true;
        }

        cout << "NULL_CHECKER on adding: " << +NULLChecker << endl;
        record.NULLChecker = NULLChecker;

        disk->insertRecord(&record);
    }
    file.close();
}

int main()
{
    // Set block size to 200B and disk size to 100MB
    const unsigned int FIXED_BLOCKSIZE = 400;
    const unsigned int DISKSIZE = 100 * pow(2, 20);
    Disk disk = Disk(DISKSIZE, FIXED_BLOCKSIZE);

    disk.printBlock(0);
    readFileToDisk(&disk);

    disk.printBlock(0);
    cout << disk.getNumRecordsByBlock(1) << endl;
    cout << disk.getNumRecordsUsed() << endl;
}
