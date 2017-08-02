#include "server.h"

void deal_fun(void* rfd)
{
  int* fd = (int*)rfd;
  char* buf = (char*)malloc(BUF_SIZE);  //max file size: 1M
  std::string name;
  int readnum;

  readnum = read(*fd, buf, BUF_SIZE);
  if(readnum <=0)
  {
      close(*fd);
      *fd = -1;
      rfd = (void*)fd;
      return;
  }
  int i=0;
  while(buf[i] !='\n')   //read file name: first line
  {
    name += buf[i++];
  }
 
  std::string infilename =  name;
  std::string outfilename = name.substr(0,name.find('.')) + ".html";
  std::ofstream fr(infilename.c_str());  //save the message to file 
  fr.write(buf+i, readnum-i);   //clip the filename
  fr.close();

  Mdfile file = Mdfile(infilename, outfilename);   //start parser
  Parser fileParser = Parser(&file);
  fileParser.start();

  std::fstream fw(outfilename.c_str());
  fw.seekg (0, fw.end);
  int length = fw.tellg();
  fw.seekg (0, fw.beg);
  memset(buf,0,BUF_SIZE); //clear buff
  strcpy(buf,outfilename.c_str ());   //add file name
  strcat(buf,"\n");
  fw.read(buf+outfilename.size()+1, length);

  write(*fd,buf, length+outfilename.size()+1);
  free(buf);

  return;
}