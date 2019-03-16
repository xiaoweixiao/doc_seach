#include"httplib.h"
#include"./search/search.h"
#include<iostream>
int main()
{
	search::Search s;
	bool ret = s.Init("../data/tmp/raw_input");
	if(!ret){
	std::cout << "Saerch Init failed" << std::endl;
	return 1;
	}
	
	using namespace httplib;
	Server server;
	server.Get("/search",[&s](const Request& req,Response& res){
		std::string query = req.get_param_value("query");
		std::string result;
		s.Searchs(query,&result);
		res.set_content(result,"text/plain");
		});
	server.listen("0.0.0.0",9091);
	return 0;
}
