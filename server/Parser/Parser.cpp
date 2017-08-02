#include "Parser.h"

const char* htmlhead = "<html>\n<head>\n<meta http-equiv=\"content-type\" content="
                       "\"text/html;charset=utf-8\">\n</head>\n";
std::regex re_header("(^#{1,6})\\s(.*)");     //the match result will contain (#..) and (...) two group
std::regex re_underline("^[\\*-]{3,}");     //***-- alse effective
std::regex re_italic("\\*(.*)\\*");
std::regex re_bold("\\*\\*(.*)\\*\\*");
std::regex re_reference("(^\\s*>{1,})");
std::regex re_hyperlink("\\[(.*)\\]\\((.*)\\)");
std::regex re_imagelink("!\\[(.*)\\]\\((.*)\\)");

std::regex re_unorderlist("(^\\s{0,3}[\\*\\+-])\\s");   //in c++ the blank space alse can be ' ' rather than \\s
std::regex re_orderlist("(^\\s{0,3}[1-9]\\.\\s)");
std::regex re_codeblock("(^\\s{0,3}```\\s*)");
std::regex re_table(".*\\|.*");
std::regex re_table_format("\\s*-*\\s*");
enum multiline_state    //list, code block ...
{
    MUL_START,
    MUL_MIDDLE
};
enum table_state
{
    TABLE_START,
    TABLE_FORMAT,
    TABLE_LINE
};
//global
multiline_state unorder_list = MUL_START;    
multiline_state order_list = MUL_START;
multiline_state code_block = MUL_START;
table_state table = TABLE_START;

std::string TABLE_HEADER_STR;    //save the fisrt line of table
bool BE_CODE_BLOCK = false;

bool Parser::start()    //start parser
{
	std::string oriStr;
    std::string parStr = "";
    file->getWriteStream()<< htmlhead;
    while(getline(file->getReadStream(), oriStr))
    {
        parStr = "";
        //parser multiline syntax : list, code block, table...
        bool bTable = isTable(oriStr,parStr);
        bool bCodeBlock = isCodeBlock(oriStr, parStr);
        bool bUnorderList = isUnorderList(oriStr, parStr);
        bool bOrderList = isOrderList(oriStr, parStr);
        //if(isTable(oriStr, parStr) || isCodeBlock(oriStr, parStr) ||
        //   isUnorderList(oriStr, parStr) || isOrderList(oriStr, parStr)
        // this kind will be optimized 
        if(bTable || bCodeBlock || bUnorderList || bOrderList)
        {
            file->getWriteStream()<<parStr <<"\n";
            continue;
        }
        
        if(oriStr == "")
        {   //a blank line will turn to newline,but "   "will nots
            file->getWriteStream()<<"<br>";
            continue;
        }
        if(isHeader(oriStr, parStr))
        {
            file->getWriteStream()<<parStr<<"\n" ;
            continue;
        }
        if(isUnderline(oriStr, parStr))
        {
            file->getWriteStream()<<parStr<<"\n" ;
            continue;
        }

        if(isReference(oriStr, parStr))
        {
            oriStr = parStr;
            parStr = "";
        }
        if(isImageLink(oriStr, parStr))      //image should before hyper
        {
            oriStr = parStr;
            parStr = "";
        }
        if(isHyperLink(oriStr, parStr))
        {
            oriStr = parStr;
            parStr = "";
        }
        if(isBold(oriStr, parStr))
        {
            oriStr = parStr;
            parStr = "";
        }
        if(isItalic(oriStr, parStr))
        {
            oriStr = parStr;
            parStr = "";
        }
        
        file->getWriteStream()<<oriStr << "\n";


    }
    file->getWriteStream() << "</html>";
    file->closeReadStream();
    file->closeWriteStream();
     
		return true;
    
}

bool Parser::isHeader(const std::string& in, std::string& out)
{
    std::smatch sm;
    if(std::regex_match(in,sm,re_header))
    {
        int i = sm[1].length();
        char s = i + '0';
        out = "<h";
        out += s;
        out += ">";
        out += sm[2];
        out += "</h";
        out += s;
        out += ">";
        return true;
    }
    return false;

}

bool Parser::isReference(const std::string& in, std::string& out)
{
    std::smatch sm;
    if(std::regex_search(in, sm, re_reference))
    {
        int i = sm[1].length();
        std::string rep;
        while(i>0)
        {
            rep += "<blockquote>\n";
            i--;
        }
        rep += "<p>";
        out = std::regex_replace(in, re_reference, rep);
        out += "</p>";
        i = sm[1].length();
        while(i>0)
        {
            out += "</blockquote>\n";
            i--;
        }
        return true;

    }
    return false;
}
bool Parser::isBold(const std::string& in, std::string& out)
{
    std::smatch sm;
    if(std::regex_search(in, sm, re_bold))
    {
        std::string rep;
        rep += "<b>";
        rep += sm[1];
        rep += "</b>";
        out = std::regex_replace(in, re_bold, rep);
        return true;
    }
    return false;
}

bool Parser::isItalic(const std::string& in, std::string& out)
{
    std::smatch sm;
    if(std::regex_search(in, sm, re_italic))
    {
        std::string rep;
        rep += "<i>";
        rep += sm[1];
        rep += "</i>";
        out = std::regex_replace(in, re_italic, rep);
        return true;
    }
    return false;
}

bool Parser::isUnderline(const std::string& in, std::string& out)
{
    std::smatch sm;
    if(std::regex_match(in, sm, re_underline))
    {
        out = "<hr>";
        return true;
    }
    return false;
}

bool Parser::isHyperLink(const std::string& in, std::string& out)
{
    std::smatch sm;
    if(std::regex_search(in, sm, re_hyperlink))
    {
        std::string rep;
        rep += "<a href=\"";
        rep += sm[2];
        rep += "\">";
        rep += sm[1];
        rep += "</a>";
        out = std::regex_replace(in, re_hyperlink, rep);
        return true;
    }
    return false;
}

bool Parser::isImageLink(const std::string& in, std::string& out)
{
    std::smatch sm;
    if(std::regex_search(in, sm, re_imagelink))
    {
        std::string rep;
        rep += "<a href=\"";
        rep += sm[2];
        rep += "\">";
        rep += sm[1];
        rep += "</a>";
        out = std::regex_replace(in, re_imagelink, rep);
        return true;
    }
    return false;
}

bool Parser::isUnorderList(const std::string& in, std::string& out)
{
    std::smatch sm;
    if(std::regex_search(in, sm, re_unorderlist))
    {
        std::string rep;
        if(unorder_list == MUL_START)
        {
            std::string tmp;
            rep += "<li>";
            tmp = std::regex_replace(in, re_unorderlist, rep);
            out = "<ul>\n" + tmp;
            out += "</li>";
            unorder_list = MUL_MIDDLE;
            return true;
        }
        rep += "<li>";
        out = std::regex_replace(in, re_unorderlist, rep);
        out += "</li>";
        return true;
    }
    if(unorder_list == MUL_MIDDLE)
    {
        unorder_list = MUL_START;
        file->getWriteStream()<< "</ul>"<< "\n";
    }
    return false;
}


bool Parser::isOrderList(const std::string& in, std::string& out)
{
    std::smatch sm;
    if(std::regex_search(in, sm, re_orderlist))
    {
        std::string rep;
        if(order_list == MUL_START)
        {
            std::string tmp;
            rep += "<li>";
            tmp = std::regex_replace(in, re_orderlist, rep);
            out = "<ol>\n" + tmp;
            out += "</li>";
            order_list = MUL_MIDDLE;
            return true;
        }
        rep += "<li>";
        out = std::regex_replace(in, re_orderlist, rep);
        out += "</li>";
        return true;
    }
    if(order_list == MUL_MIDDLE)
    {
        order_list = MUL_START;
        file->getWriteStream()<< "</ol>"<< "\n";
    }
    return false;
}

bool Parser::isCodeBlock(const std::string& in, std::string& out)
{
    std::smatch sm;
    if(table == TABLE_START && std::regex_search(in, sm, re_codeblock))
    {
        if(code_block == MUL_START)
        {
            out += "<pre><code>";
            code_block = MUL_MIDDLE;
            return true;
        }

        out += "</code></pre>";
        code_block = MUL_START;

        return true;
    }
    if(code_block == MUL_MIDDLE)
    {
        out = in;
        return true;
    }
    return false;
}

bool Parser::isTable( std::string& in, std::string& out)
{
    std::smatch sm;
    if(code_block == MUL_START && std::regex_search(in, sm, re_table))
    {
        if(table == TABLE_START)
        {
            TABLE_HEADER_STR = in;
            table = TABLE_FORMAT;
            return true;
        }
        else if(table == TABLE_FORMAT)
        {
            int pos=0,format=0;
            size_t idx;
            std::string sstr;
            in = "|" + in + "|";  //for -|- if not add "|" it will be match by unoderlist
            idx = in.find('|',pos);
            while(idx!=std::string::npos)
            {
                sstr = in.substr(pos,idx-pos);
                if(!std::regex_match(sstr,re_table_format))
                {
                    format = 1;   //do not fit to the table format rule
                    break;
                }
                pos = idx+1;
                idx = in.find('|',pos);
            }
            if(format == 0)
            {
                std::vector<std::string> str;
                std::string plit = "|";
                strSplit(TABLE_HEADER_STR, plit, str);
                if(!str.empty())
                {
                    out += "<table>\n";
                    out += "<tr>\n";
                    for(size_t i=0; i<str.size(); i++)
                    {
                        out += "<th>";
                        out += str[i];
                        out += "</th>\n";
                    }
                    out += "<tr>\n";
                    table = TABLE_LINE;
                }
                else
                {
                    table = TABLE_START;
                    out = TABLE_HEADER_STR + "\n<br>";
                    out += in;
                }
               // return true;
                
            }
            else
            {
                table = TABLE_START;
                out = TABLE_HEADER_STR + "\n<br>";
                out += in;
            }

        }
        else      //table_line
        {
            std::vector<std::string> str;
            std::string plit = "|";
            strSplit(in, plit, str);
            if(!str.empty())
            {
                out += "<tr>\n";
                for(size_t i=0; i<str.size(); i++)
                {
                    out += "<td>";
                    out += str[i];
                    out += "</td>\n";
                }
                out += "<tr>\n";
            }
            else
            {
                out = "<br>";
            }

        }
        return true;
    }
    if(table == TABLE_LINE)
    {
        table = TABLE_START;
        file->getWriteStream()<< "</table>"<< "\n";
    }
    
    return false;
}

bool Parser::multilineSyntax(const std::string& in, std::string& out)
{
    /*bool bTable = isTable(in, out);
    bool bCodeBlock = isCodeBlock(in,out);
    bool bOrderList = isOrderList(in,out);
    bool bUnorderList = isUnorderList(in,out);
    if(bTable)
    {
        return true;
    }
    if(bCodeBlock)
    {
        //while a codeblock start at the end of list
        return true;
    }

    if(bOrderList)
    {
        //while different list or block are adjacent
       
        return true;
    }

    if(bUnorderList)
    {
        return true;
    }*/

    return false;

}


void Parser::strSplit(const std::string& ssplit, const std::string& split, std::vector<std::string>& result)
{
    int pos = 0;
    std::string sstr;
    std::string tosplit = ssplit + "|";  //deal with condition: -|-  
    size_t idx = tosplit.find(split,pos);
    while(idx != std::string::npos)
    {
        sstr = tosplit.substr(pos, idx-pos);
        size_t idxtmp = 0;
        while((idxtmp = sstr.find(" ", idxtmp)) != std::string::npos)
        {
            sstr.erase(idxtmp, 1);
        }
        if(!sstr.empty())
            result.push_back(sstr);

        pos = idx + 1;
        idx = tosplit.find(split, pos);
    }
}