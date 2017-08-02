#ifndef _PARSER_H_
#define _PARSER_H_

#include "Mdfile.h"
#include <regex>


class Parser
{
    Mdfile* file;

public:
    Parser(Mdfile* files):file(files){}

    bool start();    //start parser
    bool isHeader(const std::string& in, std::string& out);
    bool isReference(const std::string& in, std::string& out);
    bool isBold(const std::string& in, std::string& out);
    bool isItalic(const std::string& in, std::string& out);
    bool isUnderline(const std::string& in, std::string& out);
    bool isHyperLink(const std::string& in, std::string& out);
    bool isImageLink(const std::string& in, std::string& out);
    bool isUnorderList(const std::string& in, std::string& out);
    bool isOrderList(const std::string& in, std::string& out);
    bool isCodeBlock(const std::string& in, std::string& out);
    bool isTable( std::string& in, std::string& out);
    bool multilineSyntax(const std::string& in, std::string& out);
    void strSplit(const std::string& tosplit, const std::string& split, std::vector<std::string>& result);

};
#endif
