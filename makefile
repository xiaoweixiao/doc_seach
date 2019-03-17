http:http.cc search/libsearch.so -lpthread -lboost_system -lboost_filesystem
	g++ http.cc search/libsearch.so -o http -std=c++11 -lpthread  -lboost_system -lboost_filesystem

clean:
	rm -rf http
