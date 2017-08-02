#ifndef _MDFILE_H_
#define _MDFILE_H_

#include <iostream>
#include <fstream>
#include <string>
#include "../ThreadPool/Debug.h"
//#define LOG(...) printf(__VA_ARGS__)
//using namespace std;


class Mdfile{
    std::string mdFileName;
    std::string convertFileName;
    //string outFielName;     //file to write out
    std::ifstream* readStream;
    std::ofstream* writeStream;

public:
    Mdfile(std::string fileName, std::string outFileName);
    
    bool readFile();
    bool writeFile(std::string& fileName);
    bool closeReadStream();
    bool closeWriteStream();
    bool getOutFileName(std::string& outName);
    std::string& getConvertFileName();
    std::ifstream& getReadStream();
    std::ofstream& getWriteStream();
    
};

#endif

