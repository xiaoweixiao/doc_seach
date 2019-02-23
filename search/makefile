FLAG=-std=c++11 -lboost_system -lboost_filesystem -ljsoncpp

.PHONY:all 
all:main libsearch.so

test:test.cc search.h search.cc 
	g++ $^ -o $@ ${FLAG}

main:main.cc search.h search.cc 
	g++ $^ -o $@ ${FLAG}

libsearch.so:search.h search.cc 
	g++ search.cc -shared -fPIC -o libsearch.so ${FLAG}
