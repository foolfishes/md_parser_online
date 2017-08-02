CMAKE_MINIMUM_REQUIRED(VERSION 2.6) #cmake最低版本需求，不加入此行会受到警告信息
#PROJECT(TEST) #项目名称
#SET(CMAKE_CXX_COMPILER "g++") #默认会使用GCC
#SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++0x -lpthread") #设置C++编译选项
#INCLUDE_DIRECTORIES((./))
AUX_SOURCE_DIRECTORY(. SRC_LIST)  
ADD_LIBRARY(Poollib ${SRC_LIST})
#ADD_EXECUTABLE(${obj} ${SRC_LIST})  #生成应用程序