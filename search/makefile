.PHONY:all 

all:main 

test:test.cc search.h search.cc 
	g++ $^ -o $@ -std=c++11 -lboost_system -lboost_filesystem -ljsoncpp

main:main.cc search.h search.cc 
	g++ $^ -o $@ -std=c++11 -lboost_system -lboost_filesystem -ljsoncpp
