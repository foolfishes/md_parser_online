#include "Mdfile.h"

Mdfile::Mdfile(std::string fileName, std::string outFileName)
{
    mdFileName = fileName;
    convertFileName = outFileName;
    readStream = new std::ifstream(); 
    writeStream = new std::ofstream();
    if(!readFile())
        LOG("file read error!\n");
    if(outFileName!="")
    {
        if(!writeFile(outFileName))
            LOG("file write error!\n");
    }else
    {
        getOutFileName(convertFileName);
        if(!writeFile(convertFileName))
            LOG("file write error!\n");

    }

}
    
bool Mdfile::readFile()
{
    readStream->open(mdFileName.c_str());
    if(readStream->is_open())
        return true;
    else
        return false;
}

bool Mdfile::writeFile(std::string& fileName)
{
    writeStream->open(fileName.c_str());
    if(writeStream->is_open())
        return true;
    else
        return false;
}

bool Mdfile::closeReadStream()
{
    readStream->close();
    return true;
}

bool Mdfile::closeWriteStream()
{
    writeStream->close();
    return true;
}

bool Mdfile::getOutFileName(std::string& outName)
{
    int id = mdFileName.find('.');
    outName = mdFileName.substr(0,id+1) + "html";
	return true;

}

std::string& Mdfile::getConvertFileName()
{
    return convertFileName;
}

std::ifstream& Mdfile::getReadStream()
{
    return *readStream;

}

std::ofstream& Mdfile::getWriteStream()
{
    return *writeStream;
}

